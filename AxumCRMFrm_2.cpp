//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AxumCRMFrm_2.h"
#include "SurfaceFrm.h"
#include "LoginFrm.h"

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

TAxumCRMForm_2 *AxumCRMForm_2;

extern void mError(struct mbn_handler *mbn, int code, char *msg);
extern void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid);
extern int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data);

//---------------------------------------------------------------------------
__fastcall TAxumCRMForm_2::TAxumCRMForm_2(TComponent* Owner, char *url, char *port, char TCP, form_node_info *node_info)
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

  BackgroundATImage = AxumCRMBackgroundATImage;

  Caption = node_info->name;

  MambaNetAddress = 0x00000000;
  Valid = 0;
  LoginBySoftware = false;

  SmallSwitchFontSize = 96;

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
  sprintf(thisnode.Description, "Axum-CRM Software node");
  strcpy(thisnode.Name, node_info->name);
  thisnode.ManufacturerID = 1;
  thisnode.ProductID = 1002;
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
                                 MBN_DATATYPE_OCTETS, 0, 32, 0, 127, "",
                                 MBN_DATATYPE_OCTETS, 32, 0, 127, 0, "");

  sprintf(obj_desc, "Chipcard password");
  objects[cntObject++] = MBN_OBJ(obj_desc,
                                 MBN_DATATYPE_OCTETS, 0, 16, 0, 127, "",
                                 MBN_DATATYPE_OCTETS, 16, 0, 127, 0, "");

  thisnode.NumberOfObjects = cntObject;

  mbn = mbnInit(&thisnode, objects, itf, err);

  mbnSetErrorCallback(mbn, mError);
  mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
  mbnSetSetActuatorDataCallback(mbn, mSetActuatorData);
}
//---------------------------------------------------------------------------

__fastcall TAxumCRMForm_2::~TAxumCRMForm_2()
{
  int ObjectNr;
  union mbn_data data;

  if (mbn != NULL) {
    if (LoginBySoftware)
    {
      ObjectNr = 1086;
      data.State = 0;
      mbnUpdateSensorData(mbn, ObjectNr, data);
    }
    mbnFree(mbn);
  }
}

void __fastcall TAxumCRMForm_2::SwitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dImage", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm_2::SwitchMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Switch%dImage", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------


void TAxumCRMForm_2::MambaNetError(int code, char *msg) {
}

void TAxumCRMForm_2::MambaNetOnlineStatus(unsigned long addr, char valid) {
  char CaptionString[128];

  MambaNetAddress = addr;
  Valid = valid;

  sprintf(CaptionString, "%s (0x%08X)", thisnode.Name, addr);
  Caption = CaptionString;
}

TPicture *TAxumCRMForm_2::GetSmallSwitchPicture(unsigned char Color)
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

void TAxumCRMForm_2::UpdateSwitch(unsigned char SwitchNr)
{
  char ObjectName[32];

  sprintf(ObjectName, "Switch%dImage", SwitchNr+1);
  //FindChildControl doesn't work, create own...
  //TControl *SwitchImage = FindChildControl(ObjectName);
  TATImage *SwitchImage = (TATImage *)FindFormControl(ObjectName);

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

int TAxumCRMForm_2::MambaNetSetActuatorData(unsigned short object, union mbn_data data)
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
      TATImage *LedImage = (TATImage *)FindFormControl(ObjectName);

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
  return 0;
}

void __fastcall TAxumCRMForm_2::Encoder_DownMouseDown(TObject *Sender,
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


void __fastcall TAxumCRMForm_2::Encoder_DownDblClick(TObject *Sender)
{
  if (Valid)
  {
    int ObjectNr = 1078;
    union mbn_data data;

    data.SInt = -10;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm_2::Encoder_UpMouseDown(TObject *Sender,
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

void __fastcall TAxumCRMForm_2::Encoder_UpDblClick(TObject *Sender)
{
  if (Valid)
  {
    int ObjectNr = 1078;
    union mbn_data data;

    data.SInt = 10;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm_2::Encoder_ResetMouseDown(TObject *Sender,
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

void __fastcall TAxumCRMForm_2::Encoder_ResetMouseUp(TObject *Sender,
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






void __fastcall TAxumCRMForm_2::PotentioMeter1KnobMouseMove(TObject *Sender,
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

void __fastcall TAxumCRMForm_2::FormResize(TObject *Sender)
{
  int cntSwitch;
  char ObjectName[32];
  TLabel *DisplayLabel;

  TMambaNetForm::FormResize(this);

  CalculateFontSizes();
}
//---------------------------------------------------------------------------

void TAxumCRMForm_2::ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description)
{
  int SwitchNr;
  char ObjectName[32];
  int MaxFontSize;

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

      DisplayLabel->Canvas->Font = DisplayLabel->Font;
      MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);

      if (MaxFontSize<SmallSwitchFontSize)
      {
      }
      else
      {
        DisplayLabel->Font->Size = SmallSwitchFontSize;
      }
    }
  }
}

void TAxumCRMForm_2::StartCommunication()
{
  char err[MBN_ERRSIZE];

  mbnStartInterface(mbn->itf, err);
}

void __fastcall TAxumCRMForm_2::LabelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Shift.Contains(ssLeft)) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Label%d", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TAxumCRMForm_2::LabelMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int SwitchNr;
  char tempText[32];

  if ((Button == Controls::mbLeft) && (Valid))
  {
    strcpy(tempText, ((TATImage *)Sender)->Name.c_str());
    sscanf(tempText, "Label%d", &SwitchNr);
    SwitchNr--;

    int ObjectNr = 1024+SwitchNr;
    union mbn_data data;

    data.State = 0;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void TAxumCRMForm_2::CalculateFontSizes()
{
  int cntSwitch;
  char ObjectName[32];
  TLabel *DisplayLabel;
  int MaxFontSize;

  Encoder_Up->Canvas->Font = Encoder_Up->Font;
  MaxFontSize = MaximalFontSizeToExtents(Encoder_Up->Canvas, Encoder_Up->Caption, Encoder_Up->Width, Encoder_Up->Height, 80);
  Encoder_Up->Font->Size = MaxFontSize;

  Encoder_Down->Canvas->Font = Encoder_Down->Font;
  MaxFontSize = MaximalFontSizeToExtents(Encoder_Down->Canvas, Encoder_Down->Caption, Encoder_Down->Width, Encoder_Down->Height, 80);
  Encoder_Down->Font->Size = MaxFontSize;

  SmallSwitchFontSize=96;
  for (cntSwitch=0; cntSwitch<54; cntSwitch++)
  {
    sprintf(ObjectName, "Label%d", cntSwitch+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      DisplayLabel->Canvas->Font = DisplayLabel->Font;
      MaxFontSize = MaximalFontSizeToExtents(DisplayLabel->Canvas, DisplayLabel->Caption, DisplayLabel->Width, DisplayLabel->Height, 80);

      if (MaxFontSize<SmallSwitchFontSize)
      {
        SmallSwitchFontSize = MaxFontSize;
      }
    }
  }

  for (cntSwitch=0; cntSwitch<54; cntSwitch++)
  {
    sprintf(ObjectName, "Label%d", cntSwitch+1);
    DisplayLabel = (TLabel *)FindFormControl(ObjectName);
    if (DisplayLabel != NULL)
    {
      DisplayLabel->Font->Size = SmallSwitchFontSize;
    }
  }
}

void TAxumCRMForm_2::PrintLabels(TCanvas *Canvas, float *xMm, float *yMm, float xPixelPerMm, float yPixelPerMm, float PageWidth, float PageHeight)
{
  float PageBorderMm = 20;

  Canvas->Font->Style = TFontStyles()<< fsBold;

  float KnobHeightMm = 9.4;
  float KnobWidthMm = 9.4;

  int FontSize = 96;
  for (int cntSwitch=0; cntSwitch<54; cntSwitch++)
  {
    char ObjectName[32];
    int Height = (KnobHeightMm*yPixelPerMm)+0.5;
    int Width = (KnobWidthMm*xPixelPerMm)+0.5;

    sprintf(ObjectName, "Label%d", cntSwitch+1);
    TLabel *SwitchLabel = (TLabel *)FindFormControl(ObjectName);

    int MaxFontSize = MaximalFontSizeToExtents(Canvas, SwitchLabel->Caption, Width, Height, 80);
    if (MaxFontSize < FontSize)
    {
      FontSize = MaxFontSize;
    }
  }

  for (int cntSwitch=0; cntSwitch<54; cntSwitch++)
  {
    char ObjectName[32];

    sprintf(ObjectName, "Label%d", cntSwitch+1);
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
      if ((*xMm+KnobWidthMm) > (PageWidth-PageBorderMm))
      {
        *xMm = PageBorderMm;
        *yMm += KnobHeightMm;
      }
    }
  }
}

bool TAxumCRMForm_2::PrintLabelsAvailable()
{
  return true;
}

void __fastcall TAxumCRMForm_2::ChipcardPaintBoxClick(TObject *Sender)
{
  if (Valid)
  {
    TLoginForm *LoginForm = new TLoginForm(this);

    if (LoginForm->ShowModal() !=  mrOk)
      return;

    char Username[33];
    char Password[17];
    int ObjectNr;
    union mbn_data data;

    strncpy(Username, LoginForm->UsernameEdit->Text.c_str(), 32);
    strncpy(Password, LoginForm->PasswordEdit->Text.c_str(), 16);

    ObjectNr = 1087;
    data.Octets = Username;
    mbnUpdateSensorData(mbn, ObjectNr, data);

    ObjectNr = 1088;
    data.Octets = Password;
    mbnUpdateSensorData(mbn, ObjectNr, data);

    ObjectNr = 1086;
    data.State = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);

    delete LoginForm;

    LoginBySoftware = true;
  }
}
//---------------------------------------------------------------------------


