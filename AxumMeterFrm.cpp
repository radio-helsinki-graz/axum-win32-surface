//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AxumMeterFrm.h"
#include "SurfaceFrm.h"

#include <stdio.h>
#include <dos.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AnalogClock"
#pragma link "MeterPanel"
#pragma link "GradientFill"
#pragma link "ExtendedPanel"
#pragma link "VUMeter"
#pragma link "BitmapButton"
#pragma link "EQPanel"
#pragma link "PhaseMeter"
#pragma link "FaderPanel"
#pragma link "MambaNetFrm"
#pragma link "Knob"
#pragma resource "*.dfm"

TAxumMeterForm *AxumMeterForm;

extern void mError(struct mbn_handler *mbn, int code, char *msg);
extern void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid);
extern int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data);

//---------------------------------------------------------------------------
__fastcall TAxumMeterForm::TAxumMeterForm(TComponent* Owner, char *url, form_node_info *node_info)
   : TMambaNetForm(Owner)
{
  char err[MBN_ERRSIZE];
  struct mbn_interface *itf;
  PGresult *res;
  char *Query;
  char SurfaceText[16][1024];
  int cnt, cntSwitch;
  struct mbn_object objects[200];
  int cntObject = 0;
  char obj_desc[32];

  BackgroundImage = AxumMeterBackgroundImage;

  Caption = node_info->name;

  MambaNetAddress = 0x00000000;
  Valid = 0;

  for (cnt=0; cnt<54; cnt++)
  {
    if (((cnt>=22) && (cnt<34) || (cnt==34)) ||
        ((cnt>=36) && (cnt<48) || (cnt==48)))
    {
      OnColor[cnt] = 1;
    }
    else
    {
      OnColor[cnt] = 2;
    }
    OffColor[cnt] = 0;
  }

  if((itf = mbnUDPOpen(url, "34848", NULL, err)) == NULL)
  {
    ShowMessage(err);
    return;
  }

  thisnode.MambaNetAddr = 0;
  thisnode.Services = 0;
  sprintf(thisnode.Description, "Axum-Meter Software node");
  strcpy(thisnode.Name, node_info->name);
  thisnode.ManufacturerID = 1;
  thisnode.ProductID = 1003;
  thisnode.UniqueIDPerProduct = node_info->id;
  thisnode.HardwareMajorRevision = 0;
  thisnode.HardwareMinorRevision = 0;
  thisnode.FirmwareMajorRevision = 2;
  thisnode.FirmwareMinorRevision = 0;
  thisnode.FPGAFirmwareMajorRevision = 0;
  thisnode.FPGAFirmwareMinorRevision = 0;
  thisnode.NumberOfObjects = 0;
  thisnode.DefaultEngineAddr = 0;
  thisnode.HardwareParent[0] = node_info->parent.man_id;
  thisnode.HardwareParent[1] = node_info->parent.prod_id;
  thisnode.HardwareParent[2] = node_info->parent.id;
  thisnode.ServiceRequest = 0;

  objects[cntObject++] = MBN_OBJ( (char *)"Meter 1 Left dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 1 Right dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 1 Label A",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "Mon. 1");
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 1 Label B",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "-");
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 2 Left dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 2 Right dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 2 Label A",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "Mon. 2");
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 2 Label B",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "-");
  objects[cntObject++] = MBN_OBJ( (char *)"On Air",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  thisnode.NumberOfObjects = cntObject;

  mbn = mbnInit(&thisnode, objects, itf, err);

  mbnSetErrorCallback(mbn, mError);
  mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
  mbnSetSetActuatorDataCallback(mbn, mSetActuatorData);

  mbnStartInterface(itf, err);
}
//---------------------------------------------------------------------------

__fastcall TAxumMeterForm::~TAxumMeterForm()
{
  if (mbn != NULL)
    mbnFree(mbn);
}

void TAxumMeterForm::MambaNetError(int code, char *msg) {
  printf(msg);
}

void TAxumMeterForm::MambaNetOnlineStatus(unsigned long addr, char valid) {
  MambaNetAddress = addr;
  Valid = valid;
}

TControl *TAxumMeterForm::FindFormControl(char *Name)
{
  TControl *FoundControl = NULL;

  for (int cnt=0; cnt<ControlCount; cnt++)
  {
    if (strcmp(Name, Controls[cnt]->Name.c_str()) == 0)
    {
      FoundControl = Controls[cnt];
    }
  }
  return FoundControl;
}

int TAxumMeterForm::MambaNetSetActuatorData(unsigned short object, union mbn_data data)
{
  unsigned char SwitchNr;
  unsigned char KnobNr;
  unsigned char cnt;
  char ObjectName[32];

  switch (object)
  {
    case 1024:
    {
      LeftMeterPanel1->dBPosition=data.Float+20;
    }
    break;
    case 1025:
    {
      RightMeterPanel1->dBPosition=data.Float+20;
    }
    break;
    case 1026:
    {
      Label1Meter1->Caption = (char *)data.Octets;
    }
    break;
    case 1027:
    {
      Label2Meter1->Caption = (char *)data.Octets;
    }
    break;
    case 1028:
    {
      LeftMeterPanel2->dBPosition=data.Float+20;
    }
    break;
    case 1029:
    {
      RightMeterPanel2->dBPosition=data.Float+20;
    }
    break;
    case 1030:
    {
      Label1Meter2->Caption = (char *)data.Octets;
    }
    break;
    case 1031:
    {
      Label2Meter2->Caption = (char *)data.Octets;
    }
    break;
    case 1032:
    {
      if (data.State)
      {
        OnAirLabel->Font->Color = 0x000000FF;
      }
      else
      {
        OnAirLabel->Font->Color = 0x00000048;
      }
    }
    break;
  }
  mbnUpdateActuatorData(mbn, object, data);
  return 0;
}

void ResizeLabelFontToHeight(TLabel *DisplayLabel)
{
  DisplayLabel->Canvas->Font = DisplayLabel->Font;
  DisplayLabel->Canvas->Font->Size=1;
  TSize Size = DisplayLabel->Canvas->TextExtent(DisplayLabel->Caption);
  while ((Size.cy<DisplayLabel->Height*0.8))
  {
    DisplayLabel->Canvas->Font->Size++;
    Size = DisplayLabel->Canvas->TextExtent(DisplayLabel->Caption);
  }
  DisplayLabel->Font->Size=DisplayLabel->Canvas->Font->Size;
}

void __fastcall TAxumMeterForm::FormResize(TObject *Sender)
{
  char ObjectName[32];
  TLabel *DisplayLabel;

  TMambaNetForm::FormResize(this);

  ResizeLabelFontToHeight(OnAirLabel);
  ResizeLabelFontToHeight(Label1Meter1);
  ResizeLabelFontToHeight(Label2Meter1);
  ResizeLabelFontToHeight(Label1Meter2);
  ResizeLabelFontToHeight(Label2Meter2);
}
//---------------------------------------------------------------------------
