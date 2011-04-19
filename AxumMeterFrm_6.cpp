//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AxumMeterFrm_6.h"
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
#pragma link "ATImage"
#pragma resource "*.dfm"

TAxumMeterForm_6 *AxumMeterForm_6;

extern void mError(struct mbn_handler *mbn, int code, char *msg);
extern void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid);
extern int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data);

//---------------------------------------------------------------------------
__fastcall TAxumMeterForm_6::TAxumMeterForm_6(TComponent* Owner, char *url, char *port, char TCP, form_node_info *node_info)
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

  BackgroundATImage = AxumMeterBackgroundImage;

  Caption = node_info->name;

  MambaNetAddress = 0x00000000;
  Valid = 0;

  for (cnt=0; cnt<8; cnt++)
  {
    Redlight[cnt] = 0;
    MeterData[cnt] = -50;
  }
  PhaseMeterData[0] = 0;
  PhaseMeterData[1] = 0;

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

  MeterData[0] = -50;
  MeterData[1] = -50;
  MeterData[2] = -50;
  MeterData[3] = -50;

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
      SurfaceForm->StatusBar->Panels->Items[1]->Text = err;
      return;
    }
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
  thisnode.FirmwareMajorRevision = 6;
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
  objects[cntObject++] = MBN_OBJ( (char *)"Phase Meter 1",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, 0.0, 2.0, 0.0, 0.0);
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
  objects[cntObject++] = MBN_OBJ( (char *)"Phase Meter 2",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, 0.0, 2.0, 0.0, 0.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 2 Label A",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "Mon. 2");
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 2 Label B",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "-");
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 3 Left dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 3 Right dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 3 Label",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "Mon. 2");
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 4 Left dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 4 Right dB",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, -50.0, 5.0, -50.0, -50.0);
  objects[cntObject++] = MBN_OBJ( (char *)"Meter 4 Label",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "Mon. 2");
  objects[cntObject++] = MBN_OBJ( (char *)"Main/Clock Label",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_OCTETS, 8, 0, 127, 20, "Mon. 2");
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 1",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 2",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 3",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 4",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 5",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 6",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 7",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Redlight 8",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Second dot count down",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Program end time enable",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Program end hour",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_UINT, 1, 0, 59, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Program end minute",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_UINT, 1, 0, 59, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Program end second",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_UINT, 1, 0, 59, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"Count down seconds",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_FLOAT, 2, 0.0, 60.0, 0.0, 0.0);
  objects[cntObject++] = MBN_OBJ( (char *)"EQ on/off",
                                  MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  for (int cntBand=0; cntBand<6; cntBand++)
  {
    sprintf(obj_desc, "EQ%d level", cntBand+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_FLOAT, 1, 2, -18.0, 18.0, 0.0,
                                   MBN_DATATYPE_FLOAT, 2, -18.0, 18.0, 0.0, 0.0);
    sprintf(obj_desc, "EQ%d frequency", cntBand+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_UINT, 1, 2, 10, 20000, 1000,
                                   MBN_DATATYPE_UINT, 2, 10, 20000, 1000, 1000);
    sprintf(obj_desc, "EQ%d Q", cntBand+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_FLOAT, 1, 2, 0.1, 10.0, 1.0,
                                   MBN_DATATYPE_FLOAT, 2, 0.1, 10.0, 1.0, 1.0);
    sprintf(obj_desc, "EQ%d type", cntBand+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 2, 0, 7, 3,
                                   MBN_DATATYPE_STATE, 2, 0, 7, 3);
  }
  objects[cntObject++] = MBN_OBJ( (char *)"Show module parameters",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  objects[cntObject++] = MBN_OBJ( (char *)"MIC active timer",
                                  MBN_DATATYPE_NODATA,
                                  MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  thisnode.NumberOfObjects = cntObject;

  mbn = mbnInit(&thisnode, objects, itf, err);

  mbnSetErrorCallback(mbn, mError);
  mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
  mbnSetSetActuatorDataCallback(mbn, mSetActuatorData);
}
//---------------------------------------------------------------------------

__fastcall TAxumMeterForm_6::~TAxumMeterForm_6()
{
  if (mbn != NULL)
    mbnFree(mbn);
}

void TAxumMeterForm_6::MambaNetError(int code, char *msg) {
}

void TAxumMeterForm_6::MambaNetOnlineStatus(unsigned long addr, char valid) {
  char CaptionString[128];

  MambaNetAddress = addr;
  Valid = valid;

  sprintf(CaptionString, "%s (0x%08X)", thisnode.Name, addr);
  Caption = CaptionString;
}

int TAxumMeterForm_6::MambaNetSetActuatorData(unsigned short object, union mbn_data data)
{
  unsigned char SwitchNr;
  unsigned char KnobNr;
  unsigned char cnt;
  char ObjectName[32];
  float dB;
  char OctetString[9];

  switch (object)
  {
    case 1024:
    {
      dB = data.Float+20;

      MeterData[0] = dB;
      if (dB>LeftMeterPanel1->dBPosition)
      {
        LeftMeterPanel1->dBPosition = dB;
      }
    }
    break;
    case 1025:
    {
      dB = data.Float+20;

      MeterData[1] = dB;
      if (dB>RightMeterPanel1->dBPosition)
      {
        RightMeterPanel1->dBPosition = dB;
      }
    }
    break;
    case 1026:
    { //Phase meter 1
      if ((data.Float>=0) && (data.Float<=2))
      {
        PhaseMeterData[0] = data.Float;
      }
    }
    break;
    case 1027:
    {
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);

      Label1Meter1->Caption = OctetString;
    }
    break;
    case 1028:
    {
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);

      Label2Meter1->Caption = OctetString;
    }
    break;
    case 1029:
    {
      dB = data.Float+20;

      MeterData[2] = dB;
      if (dB>LeftMeterPanel2->dBPosition)
      {
        LeftMeterPanel2->dBPosition = dB;
      }
    }
    break;
    case 1030:
    {
      dB = data.Float+20;

      MeterData[3] = dB;
      if (dB>RightMeterPanel2->dBPosition)
      {
        RightMeterPanel2->dBPosition = dB;
      }
    }
    break;
    case 1031:
    { //Phase Meter 2
      if ((data.Float>=0) && (data.Float<=2))
      {
        PhaseMeterData[1] = data.Float;
      }
    }
    break;
    case 1032:
    {
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);

      Label1Meter2->Caption = OctetString;
    }
    break;
    case 1033:
    {
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);

      Label2Meter2->Caption = OctetString;
    }
    break;
    case 1034:
    { //Meter 3 left
      dB = data.Float+20;

      MeterData[4] = dB;
      if (dB>LeftMeterPanel3->dBPosition)
      {
        LeftMeterPanel3->dBPosition = dB;
      }
    }
    break;
    case 1035:
    { //Meter 3 right
      dB = data.Float+20;

      MeterData[5] = dB;
      if (dB>RightMeterPanel3->dBPosition)
      {
        RightMeterPanel3->dBPosition = dB;
      }
    }
    break;
    case 1036:
    { //Meter 3 label
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);

      Label1Meter3->Caption = OctetString;
    }
    break;
    case 1037:
    { //Meter 4 left
      dB = data.Float+20;

      MeterData[6] = dB;
      if (dB>LeftMeterPanel4->dBPosition)
      {
        LeftMeterPanel4->dBPosition = dB;
      }
    }
    break;
    case 1038:
    { //Meter 4 right
      dB = data.Float+20;

      MeterData[7] = dB;
      if (dB>RightMeterPanel4->dBPosition)
      {
        RightMeterPanel4->dBPosition = dB;
      }
    }
    break;
    case 1039:
    { //Meter 4 label
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);

      Label1Meter4->Caption = OctetString;
    }
    break;
    case 1040:
    { //Main/Clock Labels
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);

      MainLabel->Caption = OctetString;
    }
    break;
    case 1041:
    case 1042:
    case 1043:
    case 1044:
    case 1045:
    case 1046:
    case 1047:
    case 1048:
    {
      int RedlightNr = object-1041;
      char ObjectName[32];

      sprintf(ObjectName, "Redlight%dLabel", RedlightNr+1);
      TLabel *RedlightLabel = (TLabel *)FindFormControl(ObjectName);

      if (RedlightLabel != NULL)
      {
        if (data.State)
        {
          RedlightLabel->Font->Color = (TColor)0x000000FF;
        }
        else
        {
          RedlightLabel->Font->Color = (TColor)0x00000040;
        }
      }

      Redlight[RedlightNr] = data.State;

      bool OnAir = false;
      for (int cntRedlight=0; cntRedlight<8; cntRedlight++)
      {
        if (Redlight[cntRedlight])
        {
          OnAir = true;
        }
      }

      if (OnAir)
      {
        OnAirLabel->Font->Color = (TColor)0x000000FF;
      }
      else
      {
        OnAirLabel->Font->Color = (TColor)0x00000040;
      }
    }
    break;
    case 1049:
    { //Clock count down
    }
    break;
  }
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

void __fastcall TAxumMeterForm_6::FormResize(TObject *Sender)
{
  char ObjectName[32];
  TLabel *DisplayLabel;

  TMambaNetForm::FormResize(this);

  ResizeLabelFontToHeight(OnAirLabel);
  ResizeLabelFontToHeight(Label1Meter1);
  ResizeLabelFontToHeight(Redlight1Label);
  ResizeLabelFontToHeight(Redlight2Label);
  ResizeLabelFontToHeight(Redlight3Label);
  ResizeLabelFontToHeight(Redlight4Label);
  ResizeLabelFontToHeight(Redlight5Label);
  ResizeLabelFontToHeight(Redlight6Label);
  ResizeLabelFontToHeight(Redlight7Label);
  ResizeLabelFontToHeight(Redlight8Label);
  ResizeLabelFontToHeight(Label2Meter1);
  ResizeLabelFontToHeight(Label1Meter2);
  ResizeLabelFontToHeight(Label2Meter2);
  ResizeLabelFontToHeight(Label1Meter3);
  ResizeLabelFontToHeight(Label1Meter4);
}
//---------------------------------------------------------------------------
void __fastcall TAxumMeterForm_6::MeterReleaseTimerTimer(TObject *Sender)
{
   if (LeftMeterPanel1->dBPosition>-50)
   {
      if (MeterData[0] < LeftMeterPanel1->dBPosition)
      {
         LeftMeterPanel1->dBPosition-=0.25;
      }
   }

   if (RightMeterPanel1->dBPosition>-50)
   {
      if (MeterData[1] < RightMeterPanel1->dBPosition)
      {
         RightMeterPanel1->dBPosition-=0.25;
      }
   }

   if (LeftMeterPanel2->dBPosition>-50)
   {
      if (MeterData[2] < LeftMeterPanel2->dBPosition)
      {
         LeftMeterPanel2->dBPosition-=0.25;
      }
   }

   if (RightMeterPanel2->dBPosition>-50)
   {
      if (MeterData[3] < RightMeterPanel2->dBPosition)
      {
         RightMeterPanel2->dBPosition-=0.25;
      }
   }

   if (LeftMeterPanel3->dBPosition>-50)
   {
      if (MeterData[4] < LeftMeterPanel3->dBPosition)
      {
         LeftMeterPanel3->dBPosition-=0.25;
      }
   }

   if (RightMeterPanel3->dBPosition>-50)
   {
      if (MeterData[5] < RightMeterPanel3->dBPosition)
      {
         RightMeterPanel3->dBPosition-=0.25;
      }
   }

   if (LeftMeterPanel4->dBPosition>-50)
   {
      if (MeterData[6] < LeftMeterPanel4->dBPosition)
      {
         LeftMeterPanel4->dBPosition-=0.25;
      }
   }

   if (RightMeterPanel4->dBPosition>-50)
   {
      if (MeterData[7] < RightMeterPanel4->dBPosition)
      {
         RightMeterPanel4->dBPosition-=0.25;
      }
   }

#define PHASE_STEPSIZE 0.0075
   if (PhaseMeterData[0] > (PhaseMeter1->Position+PHASE_STEPSIZE))
     PhaseMeter1->Position += PHASE_STEPSIZE;
   else if (PhaseMeterData[0] < (PhaseMeter1->Position-PHASE_STEPSIZE))
     PhaseMeter1->Position -= PHASE_STEPSIZE;

   if (PhaseMeterData[1] > (PhaseMeter2->Position+PHASE_STEPSIZE))
     PhaseMeter2->Position += PHASE_STEPSIZE;
   else if (PhaseMeterData[1] < (PhaseMeter2->Position-PHASE_STEPSIZE))
     PhaseMeter2->Position -= PHASE_STEPSIZE;
}
//---------------------------------------------------------------------------

void TAxumMeterForm_6::ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description)
{
}

void TAxumMeterForm_6::StartCommunication()
{
  char err[MBN_ERRSIZE];

  mbnStartInterface(mbn->itf, err);
}
