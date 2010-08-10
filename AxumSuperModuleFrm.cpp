//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AxumSuperModuleFrm.h"
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
#pragma link "PanoramaPanel"
#pragma resource "*.dfm"

TAxumSuperModuleForm *AxumSuperModuleForm;

extern void mError(struct mbn_handler *mbn, int code, char *msg);
extern void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid);
extern int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data);

//---------------------------------------------------------------------------
__fastcall TAxumSuperModuleForm::TAxumSuperModuleForm(TComponent* Owner, char *url, form_node_info *node_info)
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
  int cntBand, cntBuss;

  BackgroundImage = AxumSuperModuleBackgroundImage;

  Caption = node_info->name;

  MambaNetAddress = 0x00000000;
  Valid = 0;

  DisplayFontSize =96;
  SmallSwitchFontSize = 96;
  MiddleSwitchFontSize = 96;
  LargeSwitchFontSize = 96;
  KnobFontSize = 96;
  LabelFontSize = 96;
  LowCutFontSize = 96;

  if((itf = mbnUDPOpen(url, "34848", NULL, err)) == NULL)
  {
    SurfaceForm->StatusBar->Panels->Items[1]->Text = err;
    return;
  }

  thisnode.MambaNetAddr = 0;
  thisnode.Services = 0;
  sprintf(thisnode.Description, "Axum-SuperModule Software node");
  strcpy(thisnode.Name, node_info->name);
  thisnode.ManufacturerID = 1;
  thisnode.ProductID = 1004;
  thisnode.UniqueIDPerProduct = node_info->id;
  thisnode.HardwareMajorRevision = 0;
  thisnode.HardwareMinorRevision = 0;
  thisnode.FirmwareMajorRevision = 0;
  thisnode.FirmwareMinorRevision = 0;
  thisnode.FPGAFirmwareMajorRevision = 0;
  thisnode.FPGAFirmwareMinorRevision = 0;
  thisnode.NumberOfObjects = 0;
  thisnode.DefaultEngineAddr = 0;
  thisnode.HardwareParent[0] = node_info->parent.man_id;
  thisnode.HardwareParent[1] = node_info->parent.prod_id;
  thisnode.HardwareParent[2] = node_info->parent.id;
  thisnode.ServiceRequest = 0;

  sprintf(obj_desc, "Module select");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_SINT, 1, 1, -128, 127, 0,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  sprintf(obj_desc, "Source select");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_SINT, 1, 1, -128, 127, 0,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  sprintf(obj_desc, "Phantom");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "PAD");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "Source gain knob");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                 MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);

  sprintf(obj_desc, "Source gain info");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  for (cntSwitch=0; cntSwitch<4; cntSwitch++)
  {
    sprintf(obj_desc, "Module preset %dA", cntSwitch+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

    sprintf(obj_desc, "Module preset %dB", cntSwitch+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  }

  sprintf(obj_desc, "Phase");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "Mono");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "DSP gain");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                 MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);

  sprintf(obj_desc, "DSP gain info");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  sprintf(obj_desc, "Insert");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "Low cut");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "Low cut frequency");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_SINT, 1, 1, -128, 127, 0,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  sprintf(obj_desc, "EQ");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  for (cntBand=0; cntBand<6; cntBand++)
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
                                 MBN_DATATYPE_SINT, 1, 1, -128, 127, 0,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");
  }

  sprintf(obj_desc, "Voice processing");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "D-Exp threshold");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                 MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);

  sprintf(obj_desc, "D-Exp threshold info");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  sprintf(obj_desc, "AGC threshold knob");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                 MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);

  sprintf(obj_desc, "AGC threshold info");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  sprintf(obj_desc, "AGC ratio");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                 MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);

  sprintf(obj_desc, "AGC ratio info");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  sprintf(obj_desc, "Pan");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_UINT, 1, 2, 0, 1023, 512,
                                 MBN_DATATYPE_UINT, 2, 0, 1023, 512, 512);

  sprintf(obj_desc, "On");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

  sprintf(obj_desc, "Fader");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                 MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);

  sprintf(obj_desc, "Phase meter");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, 0.0, 2.0, 0.0, 0.0);

  sprintf(obj_desc, "Left level meter");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, -50.0, 10.0, 0.0, 0.0);

  sprintf(obj_desc, "Right level meter");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, -50.0, 10.0, 0.0, 0.0);

  for (cntBuss=0; cntBuss<16; cntBuss++)
  {
    sprintf(obj_desc, "Buss %d/%d label", (cntBuss<<1)+1, (cntBuss<<1)+2);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_NODATA,
                                   MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

    sprintf(obj_desc, "Buss %d/%d pan", (cntBuss<<1)+1, (cntBuss<<1)+2);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_UINT, 1, 2, 0, 1023, 512,
                                   MBN_DATATYPE_UINT, 2, 0, 1023, 512, 512);

    sprintf(obj_desc, "Buss %d/%d pre", (cntBuss<<1)+1, (cntBuss<<1)+2);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

    sprintf(obj_desc, "Buss %d/%d on", (cntBuss<<1)+1, (cntBuss<<1)+2);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);

    sprintf(obj_desc, "Buss %d/%d level", (cntBuss<<1)+1, (cntBuss<<1)+2);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_UINT, 1, 2, 0, 1023, 0,
                                   MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);

    sprintf(obj_desc, "Buss %d/%d level info", (cntBuss<<1)+1, (cntBuss<<1)+2);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_NODATA,
                                   MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");

  }

  thisnode.NumberOfObjects = cntObject;

  mbn = mbnInit(&thisnode, objects, itf, err);

  mbnSetErrorCallback(mbn, mError);
  mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
  mbnSetSetActuatorDataCallback(mbn, mSetActuatorData);
}
//---------------------------------------------------------------------------

__fastcall TAxumSuperModuleForm::~TAxumSuperModuleForm()
{
  if (mbn != NULL)
    mbnFree(mbn);
}

void __fastcall TAxumSuperModuleForm::SwitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dImage", &SwitchNr);
    SwitchNr--;

//    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
//    union mbn_data data;
//
//    data.State = 1;
//    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumSuperModuleForm::SwitchMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dImage", &SwitchNr);
    SwitchNr--;

//    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
//    union mbn_data data;
//
//    data.State = 0;
//    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

TPicture *TAxumSuperModuleForm::GetSmallSwitchPicture(unsigned char Color)
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

TPicture *TAxumSuperModuleForm::GetLargeSwitchPicture(unsigned char Color)
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


void TAxumSuperModuleForm::MambaNetError(int code, char *msg) {
}

void TAxumSuperModuleForm::MambaNetOnlineStatus(unsigned long addr, char valid) {
  char CaptionString[128];

  MambaNetAddress = addr;
  Valid = valid;

  sprintf(CaptionString, "%s (0x%08X)", thisnode.Name, addr);
  Caption = CaptionString;
}

int TAxumSuperModuleForm::MambaNetSetActuatorData(unsigned short object, union mbn_data data)
{
  return 0;
}

void TAxumSuperModuleForm::CalculateFontSizes()
{
  int cntModule;
  int cntLine;
  char ObjectName[32];
  TLabel *DisplayLabel;
  TEdit *LCEdit;
  int cntSwitch;
  int MaxFontSize;
  int cntLabel;
  int cntKnob;

  //recalculate fontsizes
  DisplayFontSize =96;
  SmallSwitchFontSize = 96;
  MiddleSwitchFontSize = 96;
  LargeSwitchFontSize = 96;
  KnobFontSize = 96;
  LabelFontSize = 96;
  LowCutFontSize = 96;

  for (cntLine=0; cntLine<2; cntLine++)
  {
    sprintf(ObjectName, "Display%dLabel", cntLine+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);

    if (DisplayLabel != NULL)
    {
      MaxFontSize = MaximalFontSizeToLabelExtents(DisplayLabel, 80);
      if (MaxFontSize < DisplayFontSize)
      {
        DisplayFontSize = MaxFontSize;
      }
    }
  }
  for (cntSwitch=0; cntSwitch<49; cntSwitch++)
  {
    sprintf(ObjectName, "Switch%dLabel", cntSwitch+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      MaxFontSize = MaximalFontSizeToLabelExtents(DisplayLabel, 80);

      if (cntSwitch==16)
      { //large switches
        if (MaxFontSize < LargeSwitchFontSize)
        {
          LargeSwitchFontSize = MaxFontSize;
        }
      }
      else if ((cntSwitch>=2) && (cntSwitch<10))
      { //small switches
        if (MaxFontSize < SmallSwitchFontSize)
        {
          SmallSwitchFontSize = MaxFontSize;
        }
      }
      else
      { //middle switches
        if (MaxFontSize < MiddleSwitchFontSize)
        {
          MiddleSwitchFontSize = MaxFontSize;
        }
      }
    }
  }
  for (cntLabel=0; cntLabel<29; cntLabel++)
  {
    sprintf(ObjectName, "Label%d", cntLabel+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);

    if (DisplayLabel != NULL)
    {
      MaxFontSize = MaximalFontSizeToLabelExtents(DisplayLabel, 80);
      if (MaxFontSize < LabelFontSize)
      {
        LabelFontSize = MaxFontSize;
      }
    }
  }
  for (cntKnob=0; cntKnob<1; cntKnob++)
  {
    sprintf(ObjectName, "Knob%dLabel", cntKnob+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      MaxFontSize = MaximalFontSizeToLabelExtents(DisplayLabel, 80);

      if (MaxFontSize < KnobFontSize)
      {
        KnobFontSize = MaxFontSize;
      }
    }
  }
  sprintf(ObjectName, "LowCutLabel");
  DisplayLabel = (TLabel *)FindFormControl(ObjectName);
  if (DisplayLabel != NULL)
  {
    MaxFontSize = MaximalFontSizeToLabelExtents(DisplayLabel, 80);

    if (MaxFontSize < LowCutFontSize)
    {
      LowCutFontSize = MaxFontSize;
    }
  }


  for (cntLine=0; cntLine<2; cntLine++)
  {
    sprintf(ObjectName, "Display%dLabel", cntLine+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);

    if (DisplayLabel != NULL)
    {
      DisplayLabel->Font->Size = DisplayFontSize;
    }
  }
  for (cntSwitch=0; cntSwitch<49; cntSwitch++)
  {
    sprintf(ObjectName, "Switch%dLabel", cntSwitch+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      if (cntSwitch==16)
      { //large switch
        DisplayLabel->Font->Size = LargeSwitchFontSize;
      }
      else if ((cntSwitch>=2) && (cntSwitch<10))
      { //large switches
        DisplayLabel->Font->Size = SmallSwitchFontSize;
      }
      else
      { //middle switches
        DisplayLabel->Font->Size = MiddleSwitchFontSize;
      }
    }
  }
  for (cntLabel=0; cntLabel<29; cntLabel++)
  {
    sprintf(ObjectName, "Label%d", cntLabel+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      DisplayLabel->Font->Size = LabelFontSize;
    }
  }
  for (cntKnob=0; cntKnob<21; cntKnob++)
  {
    sprintf(ObjectName, "Knob%dLabel", cntKnob+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      DisplayLabel->Font->Size = KnobFontSize;
    }
  }
  sprintf(ObjectName, "LowCutLabel");
  DisplayLabel = (TLabel *)FindFormControl(ObjectName);
  if (DisplayLabel != NULL)
  {
    DisplayLabel->Font->Size = LowCutFontSize;
  }
}

void __fastcall TAxumSuperModuleForm::FormResize(TObject *Sender)
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


void __fastcall TAxumSuperModuleForm::SwitchLabelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dLabel", &SwitchNr);
    SwitchNr--;

//    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
//    union mbn_data data;
//
//    data.State = 1;
//    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxumSuperModuleForm::SwitchLabelMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dLabel", &SwitchNr);
    SwitchNr--;

//    int ObjectNr = 1040+(SwitchNr*4)+ModuleNr;
//    union mbn_data data;
//
//    data.State = 0;
//    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void TAxumSuperModuleForm::ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description)
{
  int SwitchNr;
  int LedNr;
  char ObjectName[32];
  int MaxFontSize;
  TLabel *DisplayLabel;
  TImage *Image;

/*
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

      MaxFontSize = MaximalFontSizeToLabelExtents(DisplayLabel, 80);

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
    TImage *Image = (TImage *)FindFormControl(ObjectName);

    if ((DisplayLabel!=NULL) && (Image!=NULL))
    {
      DisplayLabel->Caption = Label;
      DisplayLabel->Hint = Description;
      DisplayLabel->ShowHint = true;
      Image->Hint = Description;
      Image->ShowHint = true;

      MaxFontSize = MaximalFontSizeToLabelExtents(DisplayLabel, 80);

      if (MaxFontSize<LedFontSize)
      {
      }
      else
      {
        DisplayLabel->Font->Size = LedFontSize;
      }
    }
  }*/
}

void TAxumSuperModuleForm::StartCommunication()
{
  char err[MBN_ERRSIZE];

  mbnStartInterface(mbn->itf, err);
}




