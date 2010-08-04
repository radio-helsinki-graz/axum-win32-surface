//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AxumCRMFrm.h"
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

TAxumCRMForm *AxumCRMForm;

extern void mError(struct mbn_handler *mbn, int code, char *msg);
extern void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid);
extern int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data);

//---------------------------------------------------------------------------
__fastcall TAxumCRMForm::TAxumCRMForm(TComponent* Owner, char *url, form_node_info *node_info)
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

  BackgroundImage = AxumCRMBackgroundImage;

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
  sprintf(thisnode.Description, "Axum-CRM Software node");
  strcpy(thisnode.Name, node_info->name);
  thisnode.ManufacturerID = 1;
  thisnode.ProductID = 1002;
  thisnode.UniqueIDPerProduct = node_info->id;
  thisnode.HardwareMajorRevision = 0;
  thisnode.HardwareMinorRevision = 0;
  thisnode.FirmwareMajorRevision = 1;
  thisnode.FirmwareMinorRevision = 0;
  thisnode.FPGAFirmwareMajorRevision = 0;
  thisnode.FPGAFirmwareMinorRevision = 0;
  thisnode.NumberOfObjects = 0;
  thisnode.DefaultEngineAddr = 0;
  thisnode.HardwareParent[0] = node_info->parent.man_id;
  thisnode.HardwareParent[1] = node_info->parent.prod_id;
  thisnode.HardwareParent[2] = node_info->parent.id;
  thisnode.ServiceRequest = 0;

  for (cnt=0; cnt<54; cnt++) {
    sprintf(obj_desc, "Switch %d", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  }

  sprintf(obj_desc, "Encoder 1");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_SINT, 1, 1, -128, 127, 0,
                                 MBN_DATATYPE_NODATA);

  sprintf(obj_desc, "Encoder 1 Switch");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_NODATA);

  sprintf(obj_desc, "LED Bar");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_BITS, 1, 7, 7, 0, "");

  for (cnt=0; cnt<4; cnt++) {
    sprintf(obj_desc, "Potential Meter %d", cnt+1);
    objects[cntObject++] = MBN_OBJ(obj_desc,
                                   MBN_DATATYPE_UINT, 3, 2, 0, 1023, 0,
                                   MBN_DATATYPE_UINT, 2, 0, 1023, 0, 0);
  }

  sprintf(obj_desc, "Jumper");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_NODATA);

  sprintf(obj_desc, "Chipcard switch");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                 MBN_DATATYPE_NODATA);

  sprintf(obj_desc, "Chipcard username");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_OCTETS, 32, 0, 127, "",
                                 MBN_DATATYPE_OCTETS, 32, 0, 127, 0, "");

  sprintf(obj_desc, "Chipcard password");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_OCTETS, 16, 0, 127, "",
                                 MBN_DATATYPE_OCTETS, 16, 0, 127, 0, "");

  thisnode.NumberOfObjects = cntObject;

  mbn = mbnInit(&thisnode, objects, itf, err);

  mbnSetErrorCallback(mbn, mError);
  mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
  mbnSetSetActuatorDataCallback(mbn, mSetActuatorData);
}
//---------------------------------------------------------------------------

__fastcall TAxumCRMForm::~TAxumCRMForm()
{
  if (mbn != NULL)
    mbnFree(mbn);
}

void __fastcall TAxumCRMForm::SwitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dImage", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm::SwitchMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dImage", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void TAxumCRMForm::MambaNetError(int code, char *msg) {
  printf(msg);
}

void TAxumCRMForm::MambaNetOnlineStatus(unsigned long addr, char valid) {
  MambaNetAddress = addr;
  Valid = valid;
}

TPicture *TAxumCRMForm::GetSmallSwitchPicture(unsigned char Color)
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

TControl *TAxumCRMForm::FindFormControl(char *Name)
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

void TAxumCRMForm::UpdateSwitch(unsigned char SwitchNr)
{
  char ObjectName[32];

  sprintf(ObjectName, "Switch%dImage", SwitchNr+1);
  //FindChildControl doesn't work, create own...
  //TControl *SwitchImage = FindChildControl(ObjectName);
  TImage *SwitchImage = (TImage *)FindFormControl(ObjectName);

  if (SwitchImage != NULL)
  {
    if (SwitchState[SwitchNr])
    {
      SwitchImage->Picture = GetSmallSwitchPicture(OnColor[SwitchNr]);
    }
    else
    {
      SwitchImage->Picture = GetSmallSwitchPicture(OffColor[SwitchNr]);
    }
  }
}

int TAxumCRMForm::MambaNetSetActuatorData(unsigned short object, union mbn_data data)
{
  unsigned char SwitchNr;
  unsigned char KnobNr;
  unsigned char cnt;
  char ObjectName[32];

  if ((object >= 1024) && (object < 1078))
  {
    SwitchNr = object-1024;

    SwitchState[SwitchNr] = data.State;
    UpdateSwitch(SwitchNr);
  }
  else if (object == 1080)
  {
    for (cnt=0; cnt<7; cnt++)
    {
      sprintf(ObjectName, "Led%dImage", cnt+1);
      TImage *LedImage = (TImage *)FindFormControl(ObjectName);

      if (LedImage != NULL)
      {
        if (data.Bits[0] & (0x40>>cnt))
          LedImage->Picture = LedGreenImage->Picture;
        else
          LedImage->Picture = LedOffImage->Picture;
      }
    }
  }
  else if ((object>=1081) && (object<1085))
  {
    KnobNr = object-1081;

    sprintf(ObjectName, "PotentioMeter%dKnob", KnobNr+1);
    TKnob *Knob = (TKnob *)FindFormControl(ObjectName);

    if (Knob != NULL)
    {
      Knob->Position = data.UInt;
    }
  }
  mbnUpdateActuatorData(mbn, object, data);
  return 0;
}

void __fastcall TAxumCRMForm::Encoder_DownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ((Button == Controls::mbLeft) && (Valid))
  {
    int ObjectNr = 1078;
    union mbn_data data;

    data.SInt = -1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxumCRMForm::Encoder_UpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ((Button == Controls::mbLeft) && (Valid))
  {
    int ObjectNr = 1078;
    union mbn_data data;

    data.SInt = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAxumCRMForm::Encoder_ResetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ((Button == Controls::mbLeft) && (Valid))
  {
    int ObjectNr = 1079;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm::Encoder_ResetMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ((Button == Controls::mbLeft) && (Valid))
  {
    int ObjectNr = 1079;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------






void __fastcall TAxumCRMForm::PotentioMeter1KnobMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  int KnobNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TKnob *)Sender)->Name.c_str());
    sscanf(tempText, "PotentioMeter%dKnob", &KnobNr);
    KnobNr--;

    int ObjectNr = 1081+KnobNr;
    union mbn_data data;


    data.UInt = ((TKnob *)Sender)->GetPositionFromXY(X, Y);
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm::FormResize(TObject *Sender)
{
  int cntSwitch;
  char ObjectName[32];
  TLabel *DisplayLabel;

  TMambaNetForm::FormResize(this);

  ResizeLabelFontToExtents(Encoder_Up, 80);
  ResizeLabelFontToExtents(Encoder_Down, 80);

  for (cntSwitch=0; cntSwitch<54; cntSwitch++)
  {
    sprintf(ObjectName, "Label%d", cntSwitch+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      ResizeLabelFontToExtents(DisplayLabel, 80);
    }
  }
}
//---------------------------------------------------------------------------

void TAxumCRMForm::ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description)
{
  int SwitchNr;
  char ObjectName[32];

  if ((object>=1024) && (object<1078))
  {
    SwitchNr=object-1024;
    sprintf(ObjectName, "Label%d", SwitchNr+1);
    TLabel *DisplayLabel = (TLabel *)FindFormControl(ObjectName);

    if (DisplayLabel!=NULL)
    {
      DisplayLabel->Caption = Label;
      DisplayLabel->Hint = Description;
      DisplayLabel->ShowHint = true;
      ResizeLabelFontToExtents(DisplayLabel, 80);
    }
  }
}

void TAxumCRMForm::StartCommunication()
{
  char err[MBN_ERRSIZE];

  mbnStartInterface(mbn->itf, err);
}

void __fastcall TAxumCRMForm::LabelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Label%d", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm::LabelMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TImage *)Sender)->Name.c_str());
    sscanf(tempText, "Label%d", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

