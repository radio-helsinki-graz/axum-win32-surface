//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SurfaceFrm.h"
#include "MambaNetConnectionFrm.h"
#include "SurfaceSelectFrm.h"
#define MBN_VARARG
#include "mbn.h"
#include "Axum4FBPFrm_1.h"
#include "Axum4FBPFrm_2.h"
#include "AxumCRMFrm_1.h"
#include "AxumCRMFrm_2.h"
#include "AxumMeterFrm_2.h"
#include "AxumMeterFrm_3.h"
#include "AxumMeterFrm_4.h"
#include "AxumMeterFrm_6.h"
#include "AxumSuperModuleFrm.h"
#include <Printers.hpp>

#include <stdio.h>
#include <dos.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TSurfaceForm *SurfaceForm;
TCriticalSection *lck;

//---------------------------------------------------------------------------
__fastcall TSurfaceForm::TSurfaceForm(TComponent* Owner)
   : TForm(Owner)
{
  int cnt;

  sql_conn = NULL;

  for (cnt=0; cnt<16; cnt++) {
    SurfaceNodes[cnt].MambaNetForm = NULL;
    SurfaceNodes[cnt].ConfigurationCopied = NULL;
    SurfaceNodes[cnt].FromAddr = 0;
    SurfaceNodes[cnt].Name[0] = 0;
  }

  StayOnTop = false;

  lck = new TCriticalSection();
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  node_info *tempNodeInfo;
  surface_info *tempSurfaceInfo;
  TMambaNetForm *tempMambaNetForm;
  configuration_info *tempConfigInfo;
  int cnt=0;

  for (cnt=0; cnt<16; cnt++) {
    if (SurfaceNodes[cnt].MambaNetForm != NULL)
    {
      tempMambaNetForm = SurfaceNodes[cnt].MambaNetForm;
      SurfaceNodes[cnt].MambaNetForm = NULL;
      //Process messages to prevent a deadlock on meanwhile received data
      Application->ProcessMessages();
      delete tempMambaNetForm;
    }
  }

  while (surfaces != NULL) {
    while (surfaces->nodes != NULL) {
      tempNodeInfo = surfaces->nodes;
      surfaces->nodes = surfaces->nodes->next;
      delete tempNodeInfo;
    }
    tempSurfaceInfo = surfaces;
    surfaces = surfaces->next;
    delete tempSurfaceInfo;
  }

  if (sql_conn != NULL)
  {
    PGconn *tmp = sql_conn;
    sql_conn = NULL;
    PQfinish(tmp);
  }

  Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::SQL_exec(char *Query)
{
  PGresult *res = PQexecParams(sql_conn, Query, 0, NULL, NULL, NULL, NULL, 0);
  PQclear(res);
}

bool __fastcall TSurfaceForm::CopyConfiguration(unsigned short ToManID, unsigned short ToProductID, unsigned short ToID, unsigned int FromAddr, unsigned char FirmwareMajor)
{
  bool Copied = false;
  char Query[1024];
  unsigned int MambaNetAddress;
  PGresult *res;
  char tempText[32];

  sprintf(Query, "SELECT addr FROM addresses WHERE (id).man=%d AND (id).prod=%d AND (id).id=%d", ToManID, ToProductID, ToID);
#ifdef DEBUG_SQL
  StatusBar->Panels->Items[1]->Text = Query;
  StatusBar->Refresh();
#endif
  res = PQexecParams(sql_conn, Query, 0, NULL, NULL, NULL, NULL, 0);
  if ((res != NULL) && (PQntuples(res) == 1))
  { //Copy configuration.
    strcpy(tempText, PQgetvalue(res, 0, 0));
    sscanf(tempText, "%d", &MambaNetAddress);

    SQL_exec("BEGIN;");
    SQL_exec("ALTER TABLE node_config DISABLE TRIGGER node_config_change_notify;");
    SQL_exec("ALTER TABLE defaults DISABLE TRIGGER defaults_change_notify;");

    sprintf(Query, "DELETE FROM node_config WHERE addr = %d AND firm_major=%d", MambaNetAddress, FirmwareMajor);
#ifdef DEBUG_SQL
    StatusBar->Panels->Items[1]->Text = Query;
    StatusBar->Refresh();
#endif
    SQL_exec(Query);

    sprintf(Query, "DELETE FROM defaults WHERE addr = %d AND firm_major=%d", MambaNetAddress, FirmwareMajor);
#ifdef DEBUG_SQL
    StatusBar->Panels->Items[1]->Text = Query;
    StatusBar->Refresh();
#endif
    SQL_exec(Query);

    sprintf(Query, "INSERT INTO node_config (addr, object, func, firm_major, label) SELECT %d, object, func, firm_major, label FROM node_config WHERE addr = %d AND firm_major=%d", MambaNetAddress, FromAddr, FirmwareMajor);
#ifdef DEBUG_SQL
    StatusBar->Panels->Items[1]->Text = Query;
    StatusBar->Refresh();
#endif
    SQL_exec(Query);

    sprintf(Query, "INSERT INTO defaults (addr, object, data, firm_major) SELECT %d, object, data, firm_major FROM defaults WHERE addr = %d AND firm_major=%d", MambaNetAddress, FromAddr, FirmwareMajor);
#ifdef DEBUG_SQL
    StatusBar->Panels->Items[1]->Text = Query;
    StatusBar->Refresh();
#endif
    SQL_exec(Query);

    SQL_exec("ALTER TABLE node_config ENABLE TRIGGER node_config_change_notify;");
    SQL_exec("ALTER TABLE defaults ENABLE TRIGGER defaults_change_notify;");
    SQL_exec("COMMIT;");
    
    Copied = true;
  }
  if (res != NULL)
  {
    PQclear(res);
  }

  return Copied;
}


surface_node *GetSurfaceNode(struct mbn_handler *mbn)
{
  int cnt;
  surface_node *SurfaceNode = NULL;

  for (cnt=0; cnt<16; cnt++)
  {
    TMambaNetForm *Form = (TMambaNetForm *)SurfaceForm->SurfaceNodes[cnt].MambaNetForm;
    if (Form != NULL)
    {
      if (Form->mbn == mbn)
      {
        SurfaceNode = &SurfaceForm->SurfaceNodes[cnt];
      }
    }
  }
  return SurfaceNode;
}

void mDebugError(struct mbn_handler *mbn, int code, char *msg)
{
  OutputDebugString(msg);
}

void mWriteLogMessage(struct mbn_handler *mbn, char *msg)
{
  OutputDebugString(msg);
}

void mError(struct mbn_handler *mbn, int code, char *msg)
{
  lck->Enter();

  surface_node *SurfaceNode = GetSurfaceNode(mbn);
  if ((SurfaceNode != NULL) && (SurfaceNode->MambaNetForm != NULL))
  {
    SendMessage(SurfaceNode->MambaNetForm->Handle, WM_MAMBANET_ERROR, code, (LPARAM)msg);
  }
  lck->Leave();
}

void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid)
{
  char Query[2048];
  PGresult *res;
  int cnt;
  int cntField;
  int NrOfRows;
  char tempText[32];
  unsigned int obj;
  char Label[17];
  char Desc[65];
  unsigned int func_type, func_seq, func_func;
  char StatusText[128];

  lck->Enter();

  surface_node *SurfaceNode = GetSurfaceNode(mbn);
  if ((SurfaceNode != NULL) && (SurfaceNode->MambaNetForm != NULL))
  {
    if (!SurfaceNode->ConfigurationCopied)
    {
        SurfaceNode->ConfigurationCopied = SurfaceForm->CopyConfiguration(SurfaceNode->MambaNetForm->thisnode.ManufacturerID,
                                                                         SurfaceNode->MambaNetForm->thisnode.ProductID,
                                                                         SurfaceNode->MambaNetForm->thisnode.UniqueIDPerProduct,
                                                                         SurfaceNode->FromAddr,
                                                                         SurfaceNode->MambaNetForm->thisnode.FirmwareMajorRevision);

    }

    SendMessage(SurfaceNode->MambaNetForm->Handle, WM_ONLINE_STATUS, addr, valid);

    //Load object, label and description list
    if (SurfaceForm->sql_conn != NULL)
    {
      sprintf(Query, "SELECT t.number, (n.func).type, (n.func).seq, (n.func).func,                                                                                                  \
                             CASE                                                                                                                                                   \
                               WHEN n.label IS NOT NULL THEN n.label                                                                                                                    \
                               ELSE f.label                                                                                                                                         \
                             END,                                                                                                                                                   \
                             f.name                                                                                                                                        \
                      FROM addresses a                                                                                                                                              \
                      JOIN templates t ON (a.id).man = t.man_id AND (a.id).prod = t.prod_id AND a.firm_major = t.firm_major                                                         \
                      LEFT JOIN node_config n ON t.number = n.object AND a.addr=n.addr                                                                                              \
                      LEFT JOIN functions f ON (n.func).type = (f.func).type AND (n.func).func = (f.func).func AND ((f.rcv_type = t.sensor_type) OR (f.xmt_type = t.actuator_type)) \
                      WHERE a.addr=%d                                                                                                                                               \
                      ORDER BY t.number", addr);

      res = PQexecParams(SurfaceForm->sql_conn, Query, 0, NULL, NULL, NULL, NULL, 0);
      if ((res == NULL) || (PQntuples(res) == 0)) {
        sprintf(StatusText, "DB query no result to load 'label' from addr: 0x%08X", addr);
        SendMessage(SurfaceForm->Handle, WM_STATUS_MESSAGE, 0, (LPARAM)StatusText);
      }
      for(cnt=0; cnt<PQntuples(res); cnt++)
      {
        cntField=0;

        strcpy(tempText, PQgetvalue(res, cnt, cntField++));
        if (sscanf(tempText, "%d", &obj)!=1)
        {
          sprintf(StatusText, "Unknown object number");
          SendMessage(SurfaceForm->Handle, WM_STATUS_MESSAGE, 0, (LPARAM)StatusText);
          break;
        }
        strcpy(tempText, PQgetvalue(res, cnt, cntField++));
        if (sscanf(tempText, "%d", &func_type)!=1)
        {
          func_type = 0;
        }
        strcpy(tempText, PQgetvalue(res, cnt, cntField++));
        if (sscanf(tempText, "%d", &func_seq)!=1)
        {
          func_seq = 0;
        }
        strcpy(tempText, PQgetvalue(res, cnt, cntField++));
        if (sscanf(tempText, "%d", &func_func)!=1)
        {
          func_func = 0;
        }
        strcpy(Label, PQgetvalue(res, cnt, cntField++));
        strcpy(Desc, PQgetvalue(res, cnt, cntField++));

        trim(Label);
        trim(Desc);

        space_to_newline(Label);

        if ((SurfaceNode != NULL) && (SurfaceNode->MambaNetForm != NULL))
        {
          config_info *info = new config_info;
          info->obj = obj;
          info->func_type = func_type;
          info->func_seq = func_seq;
          info->func_func = func_func;
          strcpy(info->Label, Label);
          strcpy(info->Desc, Desc);

          //PostMessage(SurfaceNode->MambaNetForm->Handle, WM_CONFIG_INFO, 0, (LPARAM)info);
          //Because we give an pointer, post message cannot be used...
          SendMessage(SurfaceNode->MambaNetForm->Handle, WM_CONFIG_INFO, 0, (LPARAM)info);

          delete info;
        }
      }
      if ((SurfaceNode != NULL) && (SurfaceNode->MambaNetForm != NULL))
      {
        PostMessage(SurfaceNode->MambaNetForm->Handle, WM_RESIZE_FONTS, 0, (LPARAM)0);
      }
      PQclear(res);
    }
    else
    {
      sprintf(StatusText, "No SQL connection!");
      SendMessage(SurfaceForm->Handle, WM_STATUS_MESSAGE, 0, (LPARAM)StatusText);
    }
  }
  lck->Leave();
}

int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data)
{
  mbnUpdateActuatorData(mbn, object, data);
  lck->Enter();
  surface_node *SurfaceNode = GetSurfaceNode(mbn);
  if ((SurfaceNode != NULL) && (SurfaceNode->MambaNetForm != NULL))
  {
    if (mbn != NULL)
    {
      set_actuator_data *actuator_data = new set_actuator_data;
      actuator_data->object = object;

      copy_datatype(mbn->objects[object-1024].ActuatorType,
                    mbn->objects[object-1024].ActuatorSize,
                    &data, &actuator_data->data);

      //PostMessage(SurfaceNode->MambaNetForm->Handle, WM_SET_ACTUATOR_DATA, 0, (LPARAM)actuator_data);
      //Because we give an pointer, post message cannot be used...
      SendMessage(SurfaceNode->MambaNetForm->Handle, WM_SET_ACTUATOR_DATA, 0, (LPARAM)actuator_data);

      free_datatype(mbn->objects[actuator_data->object-1024].ActuatorType, &actuator_data->data);
      delete actuator_data;
    }
  }
  lck->Leave();

  return 0;
}

void __fastcall TSurfaceForm::ConnecttoAXUMMenuItemClick(TObject *Sender)
{
  char err[MBN_ERRSIZE];
  struct mbn_interface *itf;
  char dbstr[256];
  PGresult *res;
  char Query[4096];
  char Parent[32] = "";
  char SurfaceText[16][1024];
  int cnt, cntRow, cntSurfaceNode;
  surface_info *WalkSurfaceInfo;
  node_info *WalkNodeInfo;
  char tempText[32];
  char *url_found;
  char *port_found;

  MambaNetConnectionForm = new TMambaNetConnectionForm(Application);

  if (MambaNetConnectionForm->ShowModal() !=  mrOk)
    return;

  if (MambaNetConnectionForm->URLEdit->Text == "")
  {
    StatusBar->Panels->Items[1]->Text = "No URL given";
  }

  char Temp[256];
  strcpy(Temp, MambaNetConnectionForm->URLEdit->Text.c_str());
  url_found = strtok(Temp, ":");
  port_found = strtok(NULL, ":");
  TCP = 0;
  if (MambaNetConnectionForm->TCPRadioButton->Checked)
    TCP = 1;

  if (url_found == NULL)
    return;

  strcpy(url, url_found);
  sprintf(port, "34848");
  if (port_found != NULL)
  {
    strcpy(port, port_found);
  }

  delete MambaNetConnectionForm;

  sprintf(dbstr, "host='%s' dbname='axum' user='axum'", url);
  sql_conn = PQconnectdb(dbstr);
  if (PQstatus(sql_conn) != CONNECTION_OK) {
    PQfinish(sql_conn);
    return;
  }
  sprintf(Query, "SELECT a.addr, a.name, a.active, a.parent, (a.id).man, (a.id).prod, (a.id).id, a.firm_major,                                                                                                                        \
    (SELECT COUNT(*) FROM templates t WHERE t.man_id = (a.id).man AND t.prod_id = (a.id).prod AND t.firm_major = a.firm_major) AS objects,                                                                                  \
    (SELECT number FROM templates t WHERE t.man_id = (a.id).man AND t.prod_id = (a.id).prod AND t.firm_major = a.firm_major AND t.description = 'Slot number') AS slot_obj,                                               \
    (SELECT name FROM addresses b WHERE (b.id).man = (a.parent).man AND (b.id).prod = (a.parent).prod AND (b.id).id = (a.parent).id) AS parent_name,                                                                        \
    (SELECT COUNT(*) FROM node_config n WHERE a.addr = n.addr AND a.firm_major = n.firm_major) AS config_cnt,                                                                                                               \
    (SELECT COUNT(*) FROM defaults d WHERE a.addr = d.addr AND a.firm_major = d.firm_major) AS default_cnt,                                                                                                                 \
    (SELECT COUNT(*) FROM predefined_node_config p WHERE (a.id).man = p.man_id AND (a.id).prod = p.prod_id AND a.firm_major = p.firm_major) AS predefined_cfg_cnt,                                                          \
    (SELECT COUNT(*) FROM predefined_node_defaults d WHERE (a.id).man = d.man_id AND (a.id).prod = d.prod_id AND a.firm_major = d.firm_major) AS predefined_dflt_cnt                                                        \
    FROM slot_config s                                                                                                                                                                                                      \
    RIGHT JOIN addresses a ON a.addr = s.addr WHERE s.addr IS NULL AND ((a.parent).man != 1 OR (a.parent).prod != 12) AND NOT ((a.id).man=(a.parent).man AND (a.id).prod=(a.parent).prod AND (a.id).id=(a.parent).id) AND (a.parent).man = 1 AND (a.parent).prod = 25 \
    ORDER BY NULLIF((a.parent).man, 0), (a.parent).prod, (a.parent).id, NOT a.active, (a.id).man, (a.id).prod, (a.id).id");

  StatusBar->Panels->Items[0]->Text = ((AnsiString)"URL: "+url);
  StatusBar->Panels->Items[1]->Text = ((AnsiString)"Read surface list from "+url);
  res = PQexecParams(sql_conn, Query, 0, NULL, NULL, NULL, NULL, 0);
  if (res == NULL) {
    StatusBar->Panels->Items[1]->Text = ((AnsiString)"No result after reading surface list from ");
    return;
  }

  if (PQntuples(res) > 0)
  { //Hardware surface found
    surfaces = WalkSurfaceInfo = new surface_info;
    memset(WalkSurfaceInfo, 0, sizeof(surface_info));
    strcpy(WalkSurfaceInfo->name, PQgetvalue(res, 0, 10));
    strcpy(Parent, PQgetvalue(res, 0, 3));

    for (cntRow=0; cntRow<PQntuples(res); cntRow++)
    {
      if (strcmp(Parent, PQgetvalue(res, cntRow, 3)) != 0)
      {
        WalkSurfaceInfo->next = new surface_info;
        memset(WalkSurfaceInfo->next, 0, sizeof(surface_info));
        strcpy(WalkSurfaceInfo->next->name, PQgetvalue(res, cntRow, 10));
        WalkSurfaceInfo = WalkSurfaceInfo->next;
        strcpy(Parent, PQgetvalue(res, cntRow, 3));
      }

      if (WalkSurfaceInfo->nodes == NULL)
      {
        WalkSurfaceInfo->nodes = WalkNodeInfo = new node_info;
      }
      else
      {
        WalkNodeInfo->next = new node_info;
        WalkNodeInfo = WalkNodeInfo->next;
      }
      memset(WalkNodeInfo, 0, sizeof(node_info));


      strcpy(tempText, PQgetvalue(res, cntRow, 0));
      sscanf(tempText, "%d", &WalkNodeInfo->addr);
      strcpy(WalkNodeInfo->name, PQgetvalue(res, cntRow, 1));
      strcpy(tempText, PQgetvalue(res, cntRow, 4));
      sscanf(tempText, "%d", &WalkNodeInfo->man_id);
      strcpy(tempText, PQgetvalue(res, cntRow, 5));
      sscanf(tempText, "%d", &WalkNodeInfo->prod_id);
      strcpy(tempText, PQgetvalue(res, cntRow, 6));
      sscanf(tempText, "%d", &WalkNodeInfo->id);
      strcpy(tempText, PQgetvalue(res, cntRow, 7));
      sscanf(tempText, "%d", &WalkNodeInfo->firm_major);
    }
  }
  if (WalkSurfaceInfo == NULL)
  {
    WalkSurfaceInfo = new surface_info;
  }
  else
  {
    WalkSurfaceInfo->next = new surface_info;
    WalkSurfaceInfo = WalkSurfaceInfo->next;
  }
  memset(WalkSurfaceInfo, 0, sizeof(surface_info));
  strcpy(WalkSurfaceInfo->name, "Super module");
  WalkSurfaceInfo->next = NULL; 
  WalkSurfaceInfo->nodes = WalkNodeInfo = new node_info;
  memset(WalkNodeInfo, 0, sizeof(node_info));
  sprintf(WalkNodeInfo->name, "Super module 1");
  WalkNodeInfo->man_id = 1;
  WalkNodeInfo->prod_id = 1004;
  WalkNodeInfo->id = 1;
  WalkNodeInfo->firm_major = 0;
  WalkNodeInfo->next = NULL;

  SurfaceSelectForm = new TSurfaceSelectForm(Application);
  WalkSurfaceInfo = surfaces;
  while (WalkSurfaceInfo != NULL)
  {
    TTreeNode *SurfaceNode = SurfaceSelectForm->TreeView1->Items->AddObject(NULL, WalkSurfaceInfo->name, WalkSurfaceInfo);
    WalkNodeInfo = WalkSurfaceInfo->nodes;
    while (WalkNodeInfo != NULL)
    {
      SurfaceSelectForm->TreeView1->Items->AddChildObject(SurfaceNode, WalkNodeInfo->name, WalkNodeInfo);
      WalkNodeInfo = WalkNodeInfo->next;
    }
    WalkSurfaceInfo = WalkSurfaceInfo->next;
  }

  if (SurfaceSelectForm->ShowModal() ==  mrOk)
  {
    cntSurfaceNode = 0;
    if (SurfaceSelectForm->TreeView1->Selected->Parent == NULL)
    { //Surface node
      StatusBar->Panels->Items[1]->Text = ((AnsiString)"Start software control surface node");
      StatusBar->Refresh();

      if (TCP)
      {
        if((itf = mbnTCPOpen(url, port, NULL, NULL, err)) == NULL)
        {
          SurfaceForm->StatusBar->Panels->Items[1]->Text = err;
          return;
        }
      }
      else
      {
        if((itf = mbnUDPOpen(url, port, NULL, err)) == NULL)
        {
          StatusBar->Panels->Items[1]->Text = ((AnsiString)"mbnError:"+err);
          StatusBar->Refresh();
          return;
        }
      }
      thisnode.MambaNetAddr = 0;
      thisnode.Services = 0;
      sprintf(thisnode.Description, "AXUM Surface software");
      strcpy(thisnode.Name, "Surface software");
      thisnode.ManufacturerID = 1;
      thisnode.ProductID = 1001;
      thisnode.UniqueIDPerProduct = 1;
      thisnode.HardwareMajorRevision = 0;
      thisnode.HardwareMinorRevision = 0;
      thisnode.FirmwareMajorRevision = 0;
      thisnode.FirmwareMinorRevision = 1;
      thisnode.FPGAFirmwareMajorRevision = 0;
      thisnode.FPGAFirmwareMinorRevision = 0;
      thisnode.NumberOfObjects = 0;
      thisnode.DefaultEngineAddr = 0;
      thisnode.HardwareParent[0] = thisnode.ManufacturerID;
      thisnode.HardwareParent[1] = thisnode.ProductID;
      thisnode.HardwareParent[2] = thisnode.UniqueIDPerProduct;
      thisnode.ServiceRequest = 0;

      if (MambaNetNodeActive(thisnode.ManufacturerID, thisnode.ProductID, thisnode.UniqueIDPerProduct) == 1)
      {
        ShowMessage("Node already active!");
      }

      mbn = mbnInit(&thisnode, NULL, itf, err);

      mbnStartInterface(itf, err);

      WalkSurfaceInfo = (surface_info *)SurfaceSelectForm->TreeView1->Selected->Data;
      WalkNodeInfo = WalkSurfaceInfo->nodes;
      while (WalkNodeInfo != NULL)
      {
        if (CreateSurfaceNodeAndForm(cntSurfaceNode, WalkNodeInfo))
        {
          cntSurfaceNode++;
        }
        WalkNodeInfo = WalkNodeInfo->next;
      }
    }
    else
    {
      WalkNodeInfo = (node_info *)SurfaceSelectForm->TreeView1->Selected->Data;
      if (WalkNodeInfo != NULL)
      {
        if (CreateSurfaceNodeAndForm(cntSurfaceNode, WalkNodeInfo))
        {
          cntSurfaceNode++;
        }
      }
    }
  }
  ReorderSurfaceNodes();

  for (cnt=0; cnt<16; cnt++)
  {
    if (SurfaceNodes[cnt].MambaNetForm != NULL)
    {
      SurfaceNodes[cnt].MambaNetForm->StartCommunication();
    }
  }
  StatusBar->Panels->Items[1]->Text = ((AnsiString)"Communication started...");
  StatusBar->Refresh();

  ConnecttoAXUMMenuItem->Enabled = false;
  DisconnectMenuItem->Enabled = true;
  GoToWebsiteMenuItem->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::ReorderSurfaceNodes()
{
  int cnt;
  int XPos = 0;

  for (cnt=0; cnt<16; cnt++)
  {
    if (SurfaceNodes[cnt].MambaNetForm != NULL)
    {
      SurfaceNodes[cnt].MambaNetForm->Top = 0;
      SurfaceNodes[cnt].MambaNetForm->Height = ClientHeight-(StatusBar->Height+4);
      SurfaceNodes[cnt].MambaNetForm->Left = XPos;
      XPos+=SurfaceNodes[cnt].MambaNetForm->Width;
    }
  }
}
//---------------------------------------------------------------------------




void __fastcall TSurfaceForm::Reorder1Click(TObject *Sender)
{
  ReorderSurfaceNodes();
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::AlwaysOnTopMenuItemClick(TObject *Sender)
{
  StayOnTop = !StayOnTop;
  if (StayOnTop)
  {
    SetWindowPos(Handle, HWND_TOPMOST, 0,0,0,0, SWP_NOACTIVATE+SWP_NOMOVE+SWP_NOSIZE);
  }
  else
  {
    SetWindowPos(Handle, HWND_NOTOPMOST, 0,0,0,0, SWP_NOACTIVATE+SWP_NOMOVE+SWP_NOSIZE);
  }
  AlwaysOnTopMenuItem->Checked = StayOnTop;
}
//---------------------------------------------------------------------------


int TSurfaceForm::CreateSurfaceNodeAndForm(int cntSurfaceNode, node_info *NodeInfo)
{
  unsigned int MambaNetAddress;
  form_node_info node_info;
  char StatusMessage[256];


  strcpy(node_info.name, NodeInfo->name);
  node_info.id = NodeInfo->id;
  node_info.parent.man_id = 1;
  node_info.parent.prod_id = 1001;
  node_info.parent.id = 1;

  if (SurfaceNodes[cntSurfaceNode].MambaNetForm != NULL)
    return 0;

  SurfaceNodes[cntSurfaceNode].FromAddr = NodeInfo->addr;
  strcpy(SurfaceNodes[cntSurfaceNode].Name, NodeInfo->name);

  if ((NodeInfo->man_id == 1) &&
     ((NodeInfo->prod_id == 7) ||
      (NodeInfo->prod_id == 8) ||
      (NodeInfo->prod_id == 26)))
  {
    sprintf(StatusMessage, "Copy configuration of '%s' (0x%08X)", NodeInfo->name, NodeInfo->addr);
    StatusBar->Panels->Items[1]->Text = StatusMessage;
    StatusBar->Refresh();

    switch (NodeInfo->prod_id)
    {
      case 7:
      {
        SurfaceNodes[cntSurfaceNode].ConfigurationCopied = CopyConfiguration(1, 1000, NodeInfo->id, NodeInfo->addr, NodeInfo->firm_major);
      }
      break;
      case 8:
      {
        SurfaceNodes[cntSurfaceNode].ConfigurationCopied = CopyConfiguration(1, 1002, NodeInfo->id, NodeInfo->addr, NodeInfo->firm_major);
      }
      break;
      case 26:
      {
        SurfaceNodes[cntSurfaceNode].ConfigurationCopied = CopyConfiguration(1, 1003, NodeInfo->id, NodeInfo->addr, NodeInfo->firm_major);
      }
      break;
    }

    sprintf(StatusMessage, "Create form for '%s' (0x%08X)", NodeInfo->name, NodeInfo->addr);
    StatusBar->Panels->Items[1]->Text = StatusMessage;
    StatusBar->Refresh();

    switch (NodeInfo->prod_id)
    {
      case 7:
      {
        switch (NodeInfo->firm_major)
        {
          case 1:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxum4FBPForm_1(this, url, port, TCP, &node_info);
          }
          break;
          case 2:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxum4FBPForm_2(this, url, port, TCP, &node_info);
          }
          break;
        }
      }
      break;
      case 8:
      {
        switch (NodeInfo->firm_major)
        {
          case 1:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxumCRMForm_1(this, url, port, TCP, &node_info);
          }
          break;
          case 2:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxumCRMForm_2(this, url, port, TCP, &node_info);
          }
          break;
        }
      }
      break;
      case 26:
      {
        switch (NodeInfo->firm_major)
        {
          case 2:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxumMeterForm_2(this, url, port, TCP, &node_info);
          }
          break;
          case 3:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxumMeterForm_3(this, url, port, TCP, &node_info);
          }
          break;
          case 4:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxumMeterForm_4(this, url, port, TCP, &node_info);
          }
          break;
          case 6:
          {
            SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxumMeterForm_6(this, url, port, TCP, &node_info);
          }
          break;
        }
      }
      break;
    }
    sprintf(StatusMessage, "Form '%s (0x%08X)' created", NodeInfo->name, NodeInfo->addr);
    StatusBar->Panels->Items[1]->Text = StatusMessage;
    return 1;
  }
  if ((NodeInfo->man_id == 1) &&
      (NodeInfo->prod_id == 1004))
  {
    sprintf(StatusMessage, "Create form for '%s'", NodeInfo->name);
    StatusBar->Panels->Items[1]->Text = StatusMessage;
    StatusBar->Refresh();
    
    SurfaceNodes[cntSurfaceNode].ConfigurationCopied = true;
    SurfaceNodes[cntSurfaceNode].MambaNetForm = new TAxumSuperModuleForm(this, url, port, TCP, &node_info);

    sprintf(StatusMessage, "Form '%s (0x%08X)' created", NodeInfo->name, NodeInfo->addr);
    StatusBar->Panels->Items[1]->Text = StatusMessage;
    return 1;
  }
  return 0;
}
void __fastcall TSurfaceForm::DisconnectMenuItemClick(TObject *Sender)
{
  node_info *tempNodeInfo;
  surface_info *tempSurfaceInfo;
  TMambaNetForm *tempMambaNetForm;
  configuration_info *tempConfigInfo;
  int cnt=0;

  if (mbn != NULL)
    mbnFree(mbn);

  for (cnt=0; cnt<16; cnt++) {
    if (SurfaceNodes[cnt].MambaNetForm != NULL)
    {
      tempMambaNetForm = SurfaceNodes[cnt].MambaNetForm;
      SurfaceNodes[cnt].MambaNetForm = NULL;
      //Process messages to prevent a deadlock on meanwhile received data
      Application->ProcessMessages();
      delete tempMambaNetForm;
    }
  }

  while (surfaces != NULL) {
    while (surfaces->nodes != NULL) {
      tempNodeInfo = surfaces->nodes;
      surfaces->nodes = surfaces->nodes->next;
      delete tempNodeInfo;
    }
    tempSurfaceInfo = surfaces;
    surfaces = surfaces->next;
    delete tempSurfaceInfo;
  }

  if (sql_conn != NULL)
  {
    PGconn *tmp = sql_conn;
    sql_conn = NULL;
    PQfinish(tmp);
  }

  ConnecttoAXUMMenuItem->Enabled = true;
  DisconnectMenuItem->Enabled = false;
  GoToWebsiteMenuItem->Enabled = false;
  sprintf(url,"");
  StatusBar->Panels->Items[0]->Text = "URL: None";
  StatusBar->Panels->Items[1]->Text = "Disconnected";
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::AboutMenuItemClick(TObject *Sender)
{
  ShowMessage("Axum control surface application v0.99");   
}
//---------------------------------------------------------------------------

void TSurfaceForm::WMStatusMessage(TMessage &msg)
{
  char *StatusText = (char *)msg.LParam;
  StatusBar->Panels->Items[1]->Text = StatusText;
  StatusBar->Refresh();
}

void __fastcall TSurfaceForm::StatusBarClick(TObject *Sender)
{
  char DestinationURL[256];
  if (strlen(url))
  {
    sprintf(DestinationURL, "http://%s", url);
    ShellExecute(NULL,"open", DestinationURL, NULL,NULL,SW_RESTORE);
  }
  else
  {
    ShowMessage("No URL found");
  }
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::GoToWebsiteMenuItemClick(TObject *Sender)
{
  char DestinationURL[256];
  if (strlen(url))
  {
    sprintf(DestinationURL, "http://%s", url);
    ShellExecute(NULL,"open", DestinationURL, NULL,NULL,SW_RESTORE);
  }
  else
  {
    ShowMessage("No URL found");
  }
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::PrintAllMenuItemClick(TObject *Sender)
{
  bool FirstPageDone = false;
  PrinterSetupDialog->Execute();

  TPrinter *Prntr = Printer();
  float xPixelsPerInch = GetDeviceCaps(Prntr->Handle, LOGPIXELSX);
  float yPixelsPerInch = GetDeviceCaps(Prntr->Handle, LOGPIXELSY);
  float xPixelPerMm = xPixelsPerInch/25.4;
  float yPixelPerMm = yPixelsPerInch/25.4;
  float xMm = 20;
  float yMm = 20;

  Prntr->BeginDoc();
  for (int cnt=0; cnt<MDIChildCount; cnt++)
  {
    TMambaNetForm *ActiveMambaNetForm = (TMambaNetForm *)MDIChildren[cnt];
    if (ActiveMambaNetForm != NULL)
    {
      if (ActiveMambaNetForm->PrintLabelsAvailable())
      {
        if (FirstPageDone)
        {
          Prntr->NewPage();
          yMm = 20;
          xMm = 20;
        }
        PrintHeader(Prntr->Canvas, ActiveMambaNetForm, &xMm, &yMm, xPixelPerMm, yPixelPerMm, Prntr->PageWidth/xPixelPerMm, Prntr->PageHeight/yPixelPerMm);
        yMm += 10;
        ActiveMambaNetForm->PrintLabels(Prntr->Canvas, &xMm, &yMm, xPixelPerMm, yPixelPerMm, Prntr->PageWidth/xPixelPerMm, Prntr->PageHeight/yPixelPerMm);
        FirstPageDone = true;
      }
    }
  }
  Prntr->EndDoc();
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::PrintSelectedMenuItemClick(TObject *Sender)
{
  PrinterSetupDialog->Execute();

  TMambaNetForm *ActiveMambaNetForm = (TMambaNetForm *)ActiveMDIChild;

  if (ActiveMambaNetForm != NULL)
  {
    TPrinter *Prntr = Printer();
    float xPixelsPerInch = GetDeviceCaps(Prntr->Handle, LOGPIXELSX);
    float yPixelsPerInch = GetDeviceCaps(Prntr->Handle, LOGPIXELSY);
    float xPixelPerMm = xPixelsPerInch/25.4;
    float yPixelPerMm = yPixelsPerInch/25.4;
    float xMm = 20;
    float yMm = 20;

    Prntr->BeginDoc();
    PrintHeader(Prntr->Canvas, ActiveMambaNetForm, &xMm, &yMm, xPixelPerMm, yPixelPerMm, Prntr->PageWidth/xPixelPerMm, Prntr->PageHeight/yPixelPerMm);
    yMm += 10;
    ActiveMambaNetForm->PrintLabels(Prntr->Canvas, &xMm, &yMm, xPixelPerMm, yPixelPerMm, Prntr->PageWidth/xPixelPerMm, Prntr->PageHeight/yPixelPerMm);
    Prntr->EndDoc();
  }
}
//---------------------------------------------------------------------------

void __fastcall TSurfaceForm::FileMenuItemClick(TObject *Sender)
{
  bool PrintAllActive = false;
  bool PrintSelectedActive = false;
  for (int cnt=0; cnt<MDIChildCount; cnt++)
  {
    TMambaNetForm *ActiveMambaNetForm = (TMambaNetForm *)MDIChildren[cnt];
    if (ActiveMambaNetForm->PrintLabelsAvailable())
    {
      PrintAllActive = true;
    }
  }

  TMambaNetForm *ActiveMambaNetForm = (TMambaNetForm *)ActiveMDIChild;
  if (ActiveMambaNetForm != NULL)
  {
    if (ActiveMambaNetForm->PrintLabelsAvailable())
    {
      PrintSelectedActive = true;
      PrintSelectedMenuItem->Caption = "Print '"+ActiveMambaNetForm->Caption+"' labels";
    }
  }

  PrintSelectedMenuItem->Enabled = PrintSelectedActive;
  if (!PrintSelectedActive)
  {
    PrintSelectedMenuItem->Caption = "Print selected labels";
  }
  PrintAllMenuItem->Enabled = PrintAllActive;
}
//---------------------------------------------------------------------------

void TSurfaceForm::PrintHeader(TCanvas *Canvas, TMambaNetForm *MambaNetForm, float *xMm, float *yMm, float xPixelPerMm, float yPixelPerMm, float PageWidthMm, float PageHeightMm)
{
  int FormIndex = -1;
  char CustomText[256];

  for (int cnt=0; cnt<16; cnt++)
  {
    if (SurfaceNodes[cnt].MambaNetForm == MambaNetForm)
    {
      FormIndex = cnt;
    }
  }

  Canvas->Font->Style = TFontStyles();

  Canvas->MoveTo(*xMm*xPixelPerMm, *yMm*yPixelPerMm);
  Canvas->LineTo((PageWidthMm-*xMm)*xPixelPerMm, *yMm*yPixelPerMm);
  *yMm += 0.2;

  sprintf(CustomText, "Labels for %s (0x%08X)", SurfaceNodes[FormIndex].Name, SurfaceNodes[FormIndex].FromAddr);
  Canvas->Font->Size = 16;
  Canvas->TextOut(*xMm*xPixelPerMm, *yMm*yPixelPerMm, CustomText);
  *yMm += Canvas->TextHeight(CustomText)/yPixelPerMm;

  *yMm += 0.2;
  Canvas->MoveTo(*xMm*xPixelPerMm, *yMm*yPixelPerMm);
  Canvas->LineTo((PageWidthMm-*xMm)*xPixelPerMm, *yMm*yPixelPerMm);
  *yMm += 0.2;

  sprintf(CustomText, "url: %s", url);
  Canvas->Font->Size = 12;
  Canvas->TextOut(*xMm*xPixelPerMm, *yMm*yPixelPerMm, CustomText);
  *yMm += Canvas->TextHeight(CustomText)/yPixelPerMm;

  TDateTime CurrentTime = CurrentTime.CurrentDateTime();
  AnsiString TextString = CurrentTime.DateTimeString();
  sprintf(CustomText, "date/time: %s", TextString.c_str());
  Canvas->Font->Size = 12;
  Canvas->TextOut(*xMm*xPixelPerMm, *yMm*yPixelPerMm, CustomText);
  *yMm += Canvas->TextHeight(CustomText)/yPixelPerMm;

  *yMm += 0.2;
  Canvas->MoveTo(*xMm*xPixelPerMm, *yMm*yPixelPerMm);
  Canvas->LineTo((PageWidthMm-*xMm)*xPixelPerMm, *yMm*yPixelPerMm);
  *yMm += 0.2;
}

int TSurfaceForm::MambaNetNodeActive(unsigned short ManufacturerID, unsigned short ProductID, unsigned short UniqueIDPerProduct)
{
  char str[3][33];
  const char*params[3];
  int cntParams;
  char Query[1024];
  int ReturnValue;
  PGresult *res;

  for (cntParams=0; cntParams<3; cntParams++)
  {
    params[cntParams] = (const char *)str[cntParams];
  }
  sprintf(str[0], "%d", ManufacturerID);
  sprintf(str[1], "%d", ProductID);
  sprintf(str[2], "%d", UniqueIDPerProduct);

  sprintf(Query, "SELECT active  \
                  FROM addresses \
                  WHERE (id).man = $1 AND (id).prod = $2 AND (id).id = $3");
  res = PQexecParams(sql_conn, Query, 3, NULL, params, NULL, NULL, 0);
  if (res == NULL) {
    ReturnValue = -1;
  }

  if (PQntuples(res) > 0)
  {
    char *tmpPtr = PQgetvalue(res, 0, 0);
    if (strcmp(tmpPtr,"t") == 0)
    {
      ReturnValue = 1;
    }
    else
    {
      ReturnValue = 0;
    }
  }
  else
  {
    ReturnValue = -1;
  }

  if (res != NULL)
    PQclear(res);

  return ReturnValue;
}
//---------------------------------------------------------------------------


