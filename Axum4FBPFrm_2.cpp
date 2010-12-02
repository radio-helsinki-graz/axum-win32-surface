//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Axum4FBPFrm_2.h"
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
#pragma link "ATImage"
#pragma resource "*.dfm"

TAxum4FBPForm_2 *Axum4FBPForm_2;

extern void mError(struct mbn_handler *mbn, int code, char *msg);
extern void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid);
extern int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data);
extern void mWriteLogMessage(struct mbn_handler *mbn, char *msg);

//---------------------------------------------------------------------------
__fastcall TAxum4FBPForm_2::TAxum4FBPForm_2(TComponent* Owner, char *url, char *port, char TCP, form_node_info *node_info)
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

  BackgroundATImage = Axum4FBPBackgroundATImage;

  Caption = node_info->name;

  MambaNetAddress = 0x00000000;
  Valid = 0;

  DisplayFontSize = 96;
  SmallFontSize = 96;
  LargeFontSize = 96;
  LedFontSize = 96;

  for (cnt=0; cnt<4; cnt++)
  {
    for (cntSwitch=0; cntSwitch<8; cntSwitch++)
    {
      OnColor[cnt][cntSwitch] = 2;
      OffColor[cnt][cntSwitch] = 0;
    }
  }

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
  sprintf(thisnode.Description, "Axum-4FBP Software node");
  strcpy(thisnode.Name, node_info->name);
  thisnode.ManufacturerID = 1;
  thisnode.ProductID = 1000;
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

  for (cnt=0; cnt<4; cnt++) {
    sprintf(obj_desc, "Display %d Line 1", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_NODATA,
                                   MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");
  }
  for (cnt=0; cnt<4; cnt++) {
    sprintf(obj_desc, "Display %d Line 2", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_NODATA,
                                   MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");
  }
  for (cnt=0; cnt<4; cnt++) {
    sprintf(obj_desc, "Encoder %d", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_SINT, 1, 1, -128, 127, 0,
                                   MBN_DATATYPE_NODATA);
  }
  for (cnt=0; cnt<4; cnt++) {
    sprintf(obj_desc, "Encoder %d Switch", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_NODATA);
  }
  for (cntSwitch=0; cntSwitch<8; cntSwitch++) {
    for (cnt=0; cnt<4; cnt++) {
      sprintf(obj_desc, "Switch %d.%d", cnt+1, cntSwitch+1);
      objects[cntObject++] = MBN_OBJ(obj_desc,
                                     MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                     MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
    }
  }
  for (cntSwitch=0; cntSwitch<8; cntSwitch++) {
    for (cnt=0; cnt<4; cnt++) {
      sprintf(obj_desc, "LED %d.%d", cnt+1, cntSwitch+1);
      objects[cntObject++] = MBN_OBJ(obj_desc,
                                     MBN_DATATYPE_NODATA,
                                     MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
    }
  }
  for (cnt=0; cnt<4; cnt++) {
    sprintf(obj_desc, "Fader %d", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                   MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);
  }
  for (cnt=0; cnt<4; cnt++) {
    sprintf(obj_desc, "Fader %d touch", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_NODATA);
  }

  for (cntSwitch=4; cntSwitch<8; cntSwitch++) {
    for (cnt=0; cnt<4; cnt++) {
      sprintf(obj_desc, "Switch %d.%d on color", cnt+1, cntSwitch+1);
      objects[cntObject++] = MBN_OBJ(obj_desc,
                                     MBN_DATATYPE_NODATA,
                                     MBN_DATATYPE_STATE, 1, 0, 3, 2, 2);
    }
  }
  for (cntSwitch=4; cntSwitch<8; cntSwitch++) {
    for (cnt=0; cnt<4; cnt++) {
      sprintf(obj_desc, "Switch %d.%d off color", cnt+1, cntSwitch+1);
      objects[cntObject++] = MBN_OBJ(obj_desc,
                                     MBN_DATATYPE_NODATA,
                                     MBN_DATATYPE_STATE, 1, 0, 3, 0, 0);
    }
  }
  thisnode.NumberOfObjects = cntObject;

  mbn = mbnInit(&thisnode, objects, itf, err);

  mbnSetErrorCallback(mbn, mError);
  mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
  mbnSetSetActuatorDataCallback(mbn, mSetActuatorData);
  mbnSetWriteLogMessageCallback(mbn, mWriteLogMessage);
}
//---------------------------------------------------------------------------

__fastcall TAxum4FBPForm_2::~TAxum4FBPForm_2()
{
  if (mbn != NULL)
    mbnFree(mbn);
}

void __fastcall TAxum4FBPForm_2::SwitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr, SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%d_%dImage", &ModuleNr, &SwitchNr);
    ModuleNr--;
    SwitchNr--;

    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxum4FBPForm_2::SwitchMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr, SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%d_%dImage", &ModuleNr, &SwitchNr);
    ModuleNr--;
    SwitchNr--;

    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

TPicture *TAxum4FBPForm_2::GetSmallSwitchPicture(unsigned char Color)
{
  TPicture *SwitchPicture;

  switch (Color)
  {
    case 0:
    {
      SwitchPicture = SmallOffImage->Picture;
    }
    break;
    case 1:
    {
      SwitchPicture = SmallGreenImage->Picture;
    }
    break;
    case 2:
    {
      SwitchPicture = SmallRedImage->Picture;
    }
    break;
    case 3:
    {
      SwitchPicture = SmallYellowImage->Picture;
    }
    break;
  }

  return SwitchPicture;
}

TPicture *TAxum4FBPForm_2::GetLargeSwitchPicture(unsigned char Color)
{
  TPicture *SwitchPicture;

  switch (Color)
  {
    case 0:
    {
      SwitchPicture = LargeOffImage->Picture;
    }
    break;
    case 1:
    {
      SwitchPicture = LargeGreenImage->Picture;
    }
    break;
    case 2:
    {
      SwitchPicture = LargeRedImage->Picture;
    }
    break;
    case 3:
    {
      SwitchPicture = LargeYellowImage->Picture;
    }
    break;
  }

  return SwitchPicture;
}

void TAxum4FBPForm_2::UpdateSwitch(unsigned char ModuleNr, unsigned char SwitchNr)
{
  char ObjectName[32];

  sprintf(ObjectName, "Switch%d_%dImage", ModuleNr+1, SwitchNr+1);
  //FindChildControl doesn't work, create own...
  //TControl *SwitchImage = FindChildControl(ObjectName);
  TATImage *SwitchImage = (TATImage *)FindFormControl(ObjectName);

  if (SwitchImage != NULL)
  {
    if (SwitchNr < 4)
    { //small switches
      if (SwitchState[ModuleNr][SwitchNr])
      {
        SwitchImage->Picture = GetSmallSwitchPicture(OnColor[ModuleNr][SwitchNr]);
      }
      else
      {
        SwitchImage->Picture = GetSmallSwitchPicture(OffColor[ModuleNr][SwitchNr]);
      }
    }
    else
    { //large switches
      if (SwitchState[ModuleNr][SwitchNr])
      {
        SwitchImage->Picture = GetLargeSwitchPicture(OnColor[ModuleNr][SwitchNr]);
      }
      else
      {
        SwitchImage->Picture = GetLargeSwitchPicture(OffColor[ModuleNr][SwitchNr]);
      }
    }
  }
}

void TAxum4FBPForm_2::MambaNetError(int code, char *msg) {
}

void TAxum4FBPForm_2::MambaNetOnlineStatus(unsigned long addr, char valid) {
  char CaptionString[128];

  MambaNetAddress = addr;
  Valid = valid;

  sprintf(CaptionString, "%s (0x%08X)", thisnode.Name, addr);
  Caption = CaptionString;
}



int TAxum4FBPForm_2::MambaNetSetActuatorData(unsigned short object, union mbn_data data)
{
  unsigned char ModuleNr;
  unsigned char SwitchNr;
  unsigned char DisplayNr;
  unsigned char LedNr;
  char ObjectName[32];
  char OctetString[9];

  if ((object >= 1024) && (object < 1032))
  {
    ModuleNr = (object-1024)%4;
    DisplayNr = (object-1024)/4;

    sprintf(ObjectName, "Display%d_Line%d", ModuleNr+1, DisplayNr+1);
    TLabel *DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      strncpy(OctetString, data.Octets, 8);
      OctetString[8]=0;
      trim(OctetString);
      DisplayLabel->Caption = OctetString;
    }
  }
  else if ((object >= 1040) && (object < 1072))
  {
    ModuleNr = (object-1040)%4;
    SwitchNr = (object-1040)/4;
    char ObjectName[32];

    SwitchState[ModuleNr][SwitchNr] = data.State;

    UpdateSwitch(ModuleNr, SwitchNr);
  }
  else if ((object>=1072) && (object<1104))
  {
    ModuleNr = (object-1072)%4;
    LedNr = (object-1072)/4;

    sprintf(ObjectName, "Led%d_%dImage", ModuleNr+1, LedNr+1);
    TATImage *LedImage = (TATImage *)FindFormControl(ObjectName);

    if (LedImage != NULL)
    {
      if (data.State)
      {
        if ((LedNr == 0) || (LedNr == 2) || (LedNr == 3))
        {
          LedImage->Picture = LedRedImage->Picture;
        }
        else
        {
          LedImage->Picture = LedGreenImage->Picture;
        }
      }
      else
      {
        LedImage->Picture = LedOffImage->Picture;
      }
    }
  }
  else if ((object>=1104) && (object<1108))
  {
    ModuleNr = (object-1104);

    sprintf(ObjectName, "FaderPanel%d", ModuleNr+1);
    TFaderPanel *FaderPanel = (TFaderPanel *)FindFormControl(ObjectName);
    if (FaderPanel != NULL)
    {
      FaderPanel->Position = data.UInt;
    }
  }
  else if ((object>=1112) && (object<1128))
  {
    ModuleNr = (object-1112)%4;
    SwitchNr = 4+((object-1112)/4);
    OnColor[ModuleNr][SwitchNr] = data.State;

    UpdateSwitch(ModuleNr, SwitchNr);
  }
  else if ((object>=1128) && (object<1144))
  {
    ModuleNr = (object-1128)%4;
    SwitchNr = 4+((object-1128)/4);
    OffColor[ModuleNr][SwitchNr] = data.State;

    UpdateSwitch(ModuleNr, SwitchNr);
  }
  return 0;
}

void __fastcall TAxum4FBPForm_2::Encoder_DownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TLabel *)Sender)->Name.c_str());
    sscanf(tempText, "Encoder%d_Down", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1032+ModuleNr;
    union mbn_data data;

    data.SInt = -1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxum4FBPForm_2::Encoder_DownDblClick(TObject *Sender)
{
  int ModuleNr;
  char tempText[32];

  if (Valid)
  {
    strcpy(tempText, ((TLabel *)Sender)->Name.c_str());
    sscanf(tempText, "Encoder%d_Down", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1032+ModuleNr;
    union mbn_data data;

    data.SInt = -10;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxum4FBPForm_2::Encoder_UpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TLabel *)Sender)->Name.c_str());
    sscanf(tempText, "Encoder%d_Up", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1032+ModuleNr;
    union mbn_data data;

    data.SInt = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxum4FBPForm_2::Encoder_UpDblClick(TObject *Sender)
{
  int ModuleNr;
  char tempText[32];

  if (Valid)
  {
    strcpy(tempText, ((TLabel *)Sender)->Name.c_str());
    sscanf(tempText, "Encoder%d_Up", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1032+ModuleNr;
    union mbn_data data;

    data.SInt = 10;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxum4FBPForm_2::Encoder_ResetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TLabel *)Sender)->Name.c_str());
    sscanf(tempText, "Encoder%d_Reset", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1036+ModuleNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxum4FBPForm_2::Encoder_ResetMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TLabel *)Sender)->Name.c_str());
    sscanf(tempText, "Encoder%d_Reset", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1036+ModuleNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxum4FBPForm_2::FaderPanelMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  int ModuleNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TFaderPanel *)Sender)->Name.c_str());
    sscanf(tempText, "FaderPanel%d", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1104+ModuleNr;
    union mbn_data data;

    data.UInt = ((TFaderPanel *)Sender)->GetPositionFromY(Y);
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxum4FBPForm_2::FaderPanelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "FaderPanel%d", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1108+ModuleNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxum4FBPForm_2::FaderPanelMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "FaderPanel%d", &ModuleNr);
    ModuleNr--;

    int ObjectNr = 1108+ModuleNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void TAxum4FBPForm_2::CalculateFontSizes()
{
  int cntModule;
  int cntLine;
  char ObjectName[32];
  TLabel *DisplayLabel;
  int cntSwitch;
  int MaxFontSize;
  int cntLED;

  //recalculate fontsizes
  DisplayFontSize = 96;
  SmallFontSize = 96;
  LargeFontSize = 96;
  LedFontSize = 96;
  for (cntModule=0; cntModule<4; cntModule++)
  {
    for (cntLine=0; cntLine<2; cntLine++)
    {
      sprintf(ObjectName, "Display%d_Line%d", cntModule+1, cntLine+1);
      DisplayLabel = (TLabel *)FindFormControl(ObjectName);

      if (DisplayLabel != NULL)
      {
        DisplayLabel->Canvas->Font = DisplayLabel->Font;
        MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);
        if (MaxFontSize < DisplayFontSize)
        {
          DisplayFontSize = MaxFontSize;
        }
      }
    }
    for (cntSwitch=0; cntSwitch<8; cntSwitch++)
    {
      sprintf(ObjectName, "Label%d_%d", cntModule+1, cntSwitch+1);
      DisplayLabel = (TLabel *)FindFormControl(ObjectName);
      if (DisplayLabel != NULL)
      {
        DisplayLabel->Canvas->Font = DisplayLabel->Font;
        MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);

        if ((cntSwitch>=0) && (cntSwitch<4))
        { //small switches
          if (MaxFontSize < SmallFontSize)
          {
            SmallFontSize = MaxFontSize;
          }
        }
        else
        { //large switches
          if (MaxFontSize < LargeFontSize)
          {
            LargeFontSize = MaxFontSize;
          }
        }
      }
    }
    for (cntLED=0; cntLED<8; cntLED++)
    {
      sprintf(ObjectName, "Led%d_%dLabel", cntModule+1, cntLED+1);
      DisplayLabel = (TLabel *)FindFormControl(ObjectName);
      if (DisplayLabel != NULL)
      {
        DisplayLabel->Canvas->Font = DisplayLabel->Font;
        MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);

        if (MaxFontSize < LedFontSize)
        {
          LedFontSize = MaxFontSize;
        }
      }
    }
  }

  //Check for smalles font size in small-switches
  for (cntModule=0; cntModule<4; cntModule++)
  {
    for (cntLine=0; cntLine<2; cntLine++)
    {
      sprintf(ObjectName, "Display%d_Line%d", cntModule+1, cntLine+1);
      DisplayLabel = (TLabel *)FindFormControl(ObjectName);

      if (DisplayLabel != NULL)
      {
        DisplayLabel->Font->Size = DisplayFontSize;
      }
    }
    sprintf(ObjectName, "Encoder%d_Up", cntModule+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      DisplayLabel->Canvas->Font = DisplayLabel->Font;
      MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);
      DisplayLabel->Font->Size = MaxFontSize;
    }
    sprintf(ObjectName, "Encoder%d_Down", cntModule+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      DisplayLabel->Canvas->Font = DisplayLabel->Font;
      MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);
      DisplayLabel->Font->Size = MaxFontSize;
    }
    for (cntSwitch=0; cntSwitch<8; cntSwitch++)
    {
      sprintf(ObjectName, "Label%d_%d", cntModule+1, cntSwitch+1);
      DisplayLabel = (TLabel *)FindFormControl(ObjectName);
      if (DisplayLabel != NULL)
      {
        if ((cntSwitch>=0) && (cntSwitch<4))
        { //small switches
          DisplayLabel->Font->Size = SmallFontSize;
        }
        else
        { //large switches
          DisplayLabel->Font->Size = LargeFontSize;
        }
      }
    }
    for (cntLED=0; cntLED<8; cntLED++)
    {
      sprintf(ObjectName, "Led%d_%dLabel", cntModule+1, cntLED+1);
      DisplayLabel = (TLabel *)FindFormControl(ObjectName);
      if (DisplayLabel != NULL)
      {
        DisplayLabel->Font->Size = LedFontSize;
      }
    }
  }
}


void __fastcall TAxum4FBPForm_2::FormResize(TObject *Sender)
{
  int cntLine;
  int cntModule;
  int cntSwitch;
  int cntChar;
  char ObjectName[32];
  TLabel *DisplayLabel;

  TMambaNetForm::FormResize(this);

  CalculateFontSizes();
}
//---------------------------------------------------------------------------


void __fastcall TAxum4FBPForm_2::SwitchLabelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr, SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Label%d_%d", &ModuleNr, &SwitchNr);
    ModuleNr--;
    SwitchNr--;

    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxum4FBPForm_2::SwitchLabelMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int ModuleNr, SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Label%d_%d", &ModuleNr, &SwitchNr);
    ModuleNr--;
    SwitchNr--;

    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void TAxum4FBPForm_2::ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description)
{
  int ModuleNr;
  int SwitchNr;
  int LedNr;
  char ObjectName[32];
  int MaxFontSize;
  TLabel *DisplayLabel;
  TATImage *Image;

  if ((object>=1040) && (object<1072))
  {
    ModuleNr=(object-1040)%4;
    SwitchNr=(object-1040)/4;
    sprintf(ObjectName, "Label%d_%d", ModuleNr+1, SwitchNr+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);

    if (DisplayLabel!=NULL)
    {
      DisplayLabel->Caption = Label;
      DisplayLabel->Hint = Description;
      DisplayLabel->ShowHint = true;

      DisplayLabel->Canvas->Font = DisplayLabel->Font;
      MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);

      if ((SwitchNr>=0) && (SwitchNr<4))
      { //small switch
        if (MaxFontSize<SmallFontSize)
        {
        }
        else
        {
          DisplayLabel->Font->Size = SmallFontSize;
        }
      }
      else
      { //large switch
        if (MaxFontSize<LargeFontSize)
        {
        }
        else
        {
          DisplayLabel->Font->Size = LargeFontSize;
        }
      }
    }
  }
  if ((object>=1072) && (object<1104))
  {
    ModuleNr=(object-1072)%4;
    LedNr=(object-1072)/4;
    sprintf(ObjectName, "Led%d_%dLabel", ModuleNr+1, LedNr+1);
    TLabel *DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    sprintf(ObjectName, "Led%d_%dImage", ModuleNr+1, LedNr+1);
    TATImage *Image = (TATImage *)FindFormControl(ObjectName);

    if ((DisplayLabel!=NULL) && (Image!=NULL))
    {
      DisplayLabel->Caption = Label;
      DisplayLabel->Hint = Description;
      DisplayLabel->ShowHint = true;
      Image->Hint = Description;
      Image->ShowHint = true;

      DisplayLabel->Canvas->Font = DisplayLabel->Font;
      MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);

      if (MaxFontSize<LedFontSize)
      {
      }
      else
      {
        DisplayLabel->Font->Size = LedFontSize;
      }
    }
  }
}

void TAxum4FBPForm_2::StartCommunication()
{
  char err[MBN_ERRSIZE];

  mbnStartInterface(mbn->itf, err);
}

void TAxum4FBPForm_2::PrintLabels(TCanvas *Canvas, float *xMm, float *yMm, float xPixelPerMm, float yPixelPerMm, float PageWidthMm, float PageHeightMm)
{
  char CustomText[256];

  Canvas->Font->Style = TFontStyles()<< fsBold;

  //Large switches
  float KnobHeightMm = 14.5;
  float KnobWidthMm = 14.5;

  int FontSize = 96;
  for (int cntSwitch=0; cntSwitch<4; cntSwitch++)
  {
    for (int cntModule=0; cntModule<4; cntModule++)
    {
      char ObjectName[32];
      int Height = (KnobHeightMm*yPixelPerMm)+0.5;
      int Width = (KnobWidthMm*xPixelPerMm)+0.5;

      sprintf(ObjectName, "Label%d_%d", cntModule+1, cntSwitch+4+1);
      TLabel *SwitchLabel = (TLabel *)FindFormControl(ObjectName);

      int MaxFontSize = MaximalFontSizeToExtents(Canvas, SwitchLabel->Caption, Width, Height, 80);
      if (MaxFontSize < FontSize)
      {
        FontSize = MaxFontSize;
      }
    }
  }

  for (int cntSwitch=0; cntSwitch<4; cntSwitch++)
  {
    for (int cntModule=0; cntModule<4; cntModule++)
    {
      char ObjectName[32];

      sprintf(ObjectName, "Label%d_%d", cntModule+1, cntSwitch+4+1);
      TLabel *SwitchLabel = (TLabel *)FindFormControl(ObjectName);

      if (SwitchLabel != NULL)
      {
        int Left = (*xMm*xPixelPerMm)+0.5;
        int Top = (*yMm*yPixelPerMm)+0.5;
        int Height = (KnobHeightMm*yPixelPerMm)+0.5;
        int Width = (KnobWidthMm*xPixelPerMm)+0.5;
        TRect TheRect;

        TheRect = Rect(Left, Top, Left+Width, Top+Height);
        Canvas->Pen->Color = clBlack;
        Canvas->Pen->Style = psSolid;
        Canvas->Brush->Style = bsClear;
        Canvas->Font->Size = FontSize;
        Canvas->Rectangle(TheRect);

        //multi line?
        char LabelText[64];
        char TextLines[32][32];
        char cntLine = 0;
        char cntCharLine = 0;

        strcpy(LabelText, SwitchLabel->Caption.c_str());

        for (int cntChar=0; cntChar<strlen(LabelText); cntChar++)
        {
          if (LabelText[cntChar] == '\n')
          {
            TextLines[cntLine][cntCharLine++] = 0;
            cntLine++;
            cntCharLine = 0;
          }
          else
          {
            TextLines[cntLine][cntCharLine++] = LabelText[cntChar];
          }
        }
        TextLines[cntLine][cntCharLine++] = 0;
        cntLine++;

        for (int line=0; line<cntLine; line++)
        {
          int TextPosX = (Left+(Width/2))-(Canvas->TextWidth(TextLines[line])/2);
          int TextPosY = (Top+((Height*(line+1))/(cntLine+1)))-(Canvas->TextHeight(TextLines[line])/2);
          Canvas->TextRect(TheRect, TextPosX, TextPosY, TextLines[line]);
        }

        *xMm += KnobWidthMm;
        if ((*xMm+KnobWidthMm) > (PageWidthMm-20))
        {
          *xMm = 20;
          *yMm += KnobHeightMm;
        }
      }
    }
  }

  //Small switches
  KnobHeightMm = 9.4;
  KnobWidthMm = 9.4;
  *xMm = 20;
  *yMm += 20;

  FontSize = 96;
  for (int cntSwitch=0; cntSwitch<4; cntSwitch++)
  {
    for (int cntModule=0; cntModule<4; cntModule++)
    {
      char ObjectName[32];
      int Height = (KnobHeightMm*yPixelPerMm)+0.5;
      int Width = (KnobWidthMm*xPixelPerMm)+0.5;

      sprintf(ObjectName, "Label%d_%d", cntModule+1, cntSwitch+1);
      TLabel *SwitchLabel = (TLabel *)FindFormControl(ObjectName);

      int MaxFontSize = MaximalFontSizeToExtents(Canvas, SwitchLabel->Caption, Width, Height, 80);
      if (MaxFontSize < FontSize)
      {
        FontSize = MaxFontSize;
      }
    }
  }
  for (int cntSwitch=0; cntSwitch<4; cntSwitch++)
  {
    for (int cntModule=0; cntModule<4; cntModule++)
    {
      char ObjectName[32];

      sprintf(ObjectName, "Label%d_%d", cntModule+1, cntSwitch+1);
      TLabel *SwitchLabel = (TLabel *)FindFormControl(ObjectName);

      if (SwitchLabel != NULL)
      {
        int Left = (*xMm*xPixelPerMm)+0.5;
        int Top = (*yMm*yPixelPerMm)+0.5;
        int Height = (KnobHeightMm*yPixelPerMm)+0.5;
        int Width = (KnobWidthMm*xPixelPerMm)+0.5;
        TRect TheRect;

        TheRect = Rect(Left, Top, Left+Width, Top+Height);
        Canvas->Pen->Color = clBlack;
        Canvas->Pen->Style = psSolid;
        Canvas->Brush->Style = bsClear;
        Canvas->Font->Size = FontSize;
        Canvas->Rectangle(TheRect);

        char LabelText[64];
        char TextLines[32][32];
        char cntLine = 0;
        char cntCharLine = 0;

        strcpy(LabelText, SwitchLabel->Caption.c_str());

        for (int cntChar=0; cntChar<strlen(LabelText); cntChar++)
        {
          if (LabelText[cntChar] == '\n')
          {
            TextLines[cntLine][cntCharLine++] = 0;
            cntLine++;
            cntCharLine = 0;
          }
          else
          {
            TextLines[cntLine][cntCharLine++] = LabelText[cntChar];
          }
        }
        TextLines[cntLine][cntCharLine++] = 0;
        cntLine++;

        for (int line=0; line<cntLine; line++)
        {
          int TextPosX = (Left+(Width/2))-(Canvas->TextWidth(TextLines[line])/2);
          int TextPosY = (Top+((Height*(line+1))/(cntLine+1)))-(Canvas->TextHeight(TextLines[line])/2);
          Canvas->TextRect(TheRect, TextPosX, TextPosY, TextLines[line]);
        }

        *xMm += KnobWidthMm;
        if ((*xMm+KnobWidthMm) > (PageWidthMm-20))
        {
          *xMm = 20;
          *yMm += KnobHeightMm;
        }
      }
    }
  }
}

bool TAxum4FBPForm_2::PrintLabelsAvailable()
{
  return true;
}

