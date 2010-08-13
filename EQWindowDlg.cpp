//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "stdio.h"
#include "EQWindowDlg.h"
#include "AxumSuperModuleFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EQPanel"
#pragma link "Knob"
#pragma link "BitmapButton"
#pragma resource "*.dfm"
TEQWindowDialog *EQWindowDialog;
//---------------------------------------------------------------------------
__fastcall TEQWindowDialog::TEQWindowDialog(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::EQWindowMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;

   if (Shift.Contains(ssLeft))
   {

      switch (DragEQ)
      {
         case Band1:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQWindow->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQWindow->GainBand1 != Gain)
            {
              ObjectNr = 1048;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
            if (EQWindow->FrequencyBand1 != Frequency)
            {
              ObjectNr = 1049;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
         }
         break;
         case Band2:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQWindow->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQWindow->GainBand2 != Gain)
            {
              ObjectNr = 1052;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
            if (EQWindow->FrequencyBand2 != Frequency)
            {
              ObjectNr = 1053;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
         }
         break;
         case Band3:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQWindow->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQWindow->GainBand3 != Gain)
            {
              ObjectNr = 1056;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
            if (EQWindow->FrequencyBand3 != Frequency)
            {
              ObjectNr = 1057;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
         }
         break;
         case Band4:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQWindow->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQWindow->GainBand4 != Gain)
            {
              ObjectNr = 1060;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
            if (EQWindow->FrequencyBand4 != Frequency)
            {
              ObjectNr = 1061;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
         }
         break;
         case Band5:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQWindow->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQWindow->GainBand5 != Gain)
            {
              ObjectNr = 1064;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
            if (EQWindow->FrequencyBand5 != Frequency)
            {
              ObjectNr = 1065;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
         }
         break;
         case Band6:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQWindow->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQWindow->GainBand6 != Gain)
            {
              ObjectNr = 1068;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
            if (EQWindow->FrequencyBand6 != Frequency)
            {
              ObjectNr = 1069;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
            }
         }
         break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::EQWindowMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   DragEQ = NoEQAnchor;
}
//---------------------------------------------------------------------------


TControl *TEQWindowDialog::FindFormControl(char *Name)
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

void __fastcall TEQWindowDialog::EQWindowMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Shift.Contains(ssLeft))
   {
      DragEQ = EQWindow->GetAnchorFromXY(X,Y);
   }
   else
   {
      DragEQ = NoEQAnchor;
   }
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::EQLevelKnobMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  int KnobNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;

  if (Shift.Contains(ssLeft))
  {
    strcpy(tempText, ((TKnob *)Sender)->Name.c_str());
    sscanf(tempText, "EQ%dLevelKnob", &KnobNr);
    KnobNr--;

    ObjectNr = 1048+(KnobNr*4);

    int Position = ((TKnob *)Sender)->GetPositionFromXY(X,Y);
    data.Float = ((Position*36)/1023)-18;

    mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::EQFrequencyKnobMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  int KnobNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;

  if (Shift.Contains(ssLeft))
  {
    strcpy(tempText, ((TKnob *)Sender)->Name.c_str());
    sscanf(tempText, "EQ%dFrequencyKnob", &KnobNr);
    KnobNr--;

    ObjectNr = 1049+(KnobNr*4);

    int Position = ((TKnob *)Sender)->GetPositionFromXY(X,Y);
    data.UInt = ((Position*19990)/1023)+10;

    mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::EQBandwidthKnobMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  int KnobNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;

  if (Shift.Contains(ssLeft))
  {
    strcpy(tempText, ((TKnob *)Sender)->Name.c_str());
    sscanf(tempText, "EQ%dBandwidthKnob", &KnobNr);
    KnobNr--;

    ObjectNr = 1050+(KnobNr*4);

    int Position = ((TKnob *)Sender)->GetPositionFromXY(X,Y);
    data.Float = ((Position*9.9)/1023)+0.1;

    mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::LowShelvingMenuItemClick(TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 2;//LOWSHELF
//  enum FilterType {OFF=0, HPF=1, LOWSHELF=2, PEAKINGEQ=3, HIGHSHELF=4, LPF=5, BPF=6, NOTCH=7};


  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::HighShelvingItemClick(TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 4;//HIGHSHELF

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------


void __fastcall TEQWindowDialog::LowPassFilterMenuItemClick(
      TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 5;//LPF

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::HighPassFilterMenuItemClick(
      TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 1;//HPF

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::BandPassFilterMenuItemClick(
      TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 6;//BPF

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::NotchMenuItemClick(TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 7;//NOTCH

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::OffMenuItemClick(TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 0;//OFF

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------


void __fastcall TEQWindowDialog::PeakingBellMenuItemClick(TObject *Sender)
{
  int LabelNr;
  char tempText[32];
  int ObjectNr;
  union mbn_data data;
  TStaticText *PopupStaticText = (TStaticText *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupStaticText->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 3;//Peaking

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------


void __fastcall TEQWindowDialog::EQOn1BitmapButtonMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  union mbn_data data;

  data.State = 1;

  mbnUpdateSensorData(mbn, 1047, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::EQOn1BitmapButtonMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  union mbn_data data;

  data.State = 0;

  mbnUpdateSensorData(mbn, 1047, data);
}
//---------------------------------------------------------------------------

