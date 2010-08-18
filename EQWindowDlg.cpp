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
#pragma link "ATImage"
#pragma resource "*.dfm"
TEQWindowDialog *EQWindowDialog;
//---------------------------------------------------------------------------
__fastcall TEQWindowDialog::TEQWindowDialog(TComponent* Owner)
   : TForm(Owner)
{
  LabelFontSize = 96;
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
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
  TLabel *PopupLabel = (TLabel *)EQTypePopupMenu->PopupComponent;

  strcpy(tempText, PopupLabel->Name.c_str());
  sscanf(tempText, "EQ%dTypeLabel", &LabelNr);
  LabelNr--;

  ObjectNr = 1051+(LabelNr*4);

  data.State = 3;//Peaking

  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  mbnUpdateSensorData(mbn, ObjectNr, data);
}
//---------------------------------------------------------------------------


void __fastcall TEQWindowDialog::EQOnLabelMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  union mbn_data data;

  data.State = 1;

  mbnUpdateSensorData(mbn, 1047, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::EQOnLabelButtonMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  mbn_handler *mbn = ((TAxumSuperModuleForm *)Owner)->mbn;
  union mbn_data data;

  data.State = 0;

  mbnUpdateSensorData(mbn, 1047, data);
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::FormCanResize(TObject *Sender,
      int &NewWidth, int &NewHeight, bool &Resize)
{
  float DifferenceHeight = Height-ClientHeight;
  float DifferenceWidth = Width-ClientWidth;
  float OriginalHeight = BackgroundImage->Picture->Height;
  float OriginalWidth = BackgroundImage->Picture->Width;

  float Ratio = OriginalWidth/OriginalHeight;

//  float CalculatedHeight = (((float)NewWidth-DifferenceWidth)/Ratio)+DifferenceHeight;
  float CalculatedWidth = (((float)NewHeight-DifferenceHeight)*Ratio)+DifferenceWidth;

  NewWidth = CalculatedWidth+0.5;
}
//---------------------------------------------------------------------------

void __fastcall TEQWindowDialog::FormResize(TObject *Sender)
{
  float Factor = (float)ClientHeight/BackgroundImage->Picture->Height;
  TControl *ChildControl;

  for (int cnt=0; cnt<ControlCount; cnt++)
  {
    ChildControl = Controls[cnt];
    ChildControl->Top = (Factor*ControlOriginalTop[cnt])+0.5;
    ChildControl->Left = (Factor*ControlOriginalLeft[cnt])+0.5;
    ChildControl->Height = (Factor*ControlOriginalHeight[cnt])+0.5;
    ChildControl->Width = (Factor*ControlOriginalWidth[cnt])+0.5;
  }

  CalculateFontSizes();
}
//---------------------------------------------------------------------------


int TEQWindowDialog::MaximalFontSizeToLabelExtents(TLabel *DisplayLabel, int Percent)
{
  int MaximumFontSize;
  int HeightSize;
  int WidthSize;
  unsigned int cntChar;
  unsigned int cntCharLine = 0;
  char LabelText[32];
  char TextLines[32][32];
  char cntLine = 0;
  char LongestLine = 0;
  AnsiString SelectedLine;
  float PercentHeight;
  float PercentWidth;

  strcpy(LabelText,DisplayLabel->Caption.c_str());

  for (cntChar=0; cntChar<strlen(LabelText); cntChar++)
  {
    TextLines[cntLine][cntCharLine++] = LabelText[cntChar];
    if (LabelText[cntChar] == '\n')
    {
      if (strlen(TextLines[LongestLine]) < cntCharLine)
      {
        LongestLine = cntLine;
      }
      cntLine++;
      cntCharLine = 0;
    }
  }
  TextLines[LongestLine][cntCharLine+1] = 0;
  PercentHeight = ((float)Percent/100)/(cntLine+1);
  PercentWidth = ((float)Percent/100);

  SelectedLine = TextLines[LongestLine];

  DisplayLabel->Canvas->Font = DisplayLabel->Font;
  DisplayLabel->Canvas->Font->Size=1;
  TSize Size = DisplayLabel->Canvas->TextExtent(SelectedLine);
  while ((Size.cy<(DisplayLabel->Height*PercentHeight)))
  {
    DisplayLabel->Canvas->Font->Size++;
    Size = DisplayLabel->Canvas->TextExtent(SelectedLine);
  }
  HeightSize=DisplayLabel->Canvas->Font->Size;

  DisplayLabel->Canvas->Font->Size=1;
  Size = DisplayLabel->Canvas->TextExtent(SelectedLine);
  while ((Size.cx<(DisplayLabel->Width*PercentWidth)))
  {
    DisplayLabel->Canvas->Font->Size++;
    Size = DisplayLabel->Canvas->TextExtent(SelectedLine);
  }
  WidthSize=DisplayLabel->Canvas->Font->Size;

  if (WidthSize<HeightSize)
    MaximumFontSize = WidthSize;
  else
    MaximumFontSize = HeightSize;

  return MaximumFontSize;
}

void TEQWindowDialog::CalculateFontSizes()
{
  int cntBand;
  char tempText[32];

  LabelFontSize = 96;
  for (cntBand=0; cntBand<6; cntBand++)
  {
    sprintf(tempText, "EQ%dLevelLabel", cntBand+1);
    TLabel *LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      int MaxFontSize = MaximalFontSizeToLabelExtents(LevelLabel, 80);
      if (MaxFontSize<LabelFontSize)
      {
        LabelFontSize = MaxFontSize;
      }
    }

    sprintf(tempText, "EQ%dFrequencyLabel", cntBand+1);
    LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      int MaxFontSize = MaximalFontSizeToLabelExtents(LevelLabel, 80);
      if (MaxFontSize<LabelFontSize)
      {
        LabelFontSize = MaxFontSize;
      }
    }

    sprintf(tempText, "EQ%dBandwidthLabel", cntBand+1);
    LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      int MaxFontSize = MaximalFontSizeToLabelExtents(LevelLabel, 80);
      if (MaxFontSize<LabelFontSize)
      {
        LabelFontSize = MaxFontSize;
      }
    }

    sprintf(tempText, "EQ%dTypeLabel", cntBand+1);
    LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      int MaxFontSize = MaximalFontSizeToLabelExtents(LevelLabel, 80);
      if (MaxFontSize<LabelFontSize)
      {
        LabelFontSize = MaxFontSize;
      }
    }

    int MaxFontSize = MaximalFontSizeToLabelExtents(EQOnLabel, 80);
    if (MaxFontSize<LabelFontSize)
    {
      LabelFontSize = MaxFontSize;
    }
  }

  for (cntBand=0; cntBand<6; cntBand++)
  {
    sprintf(tempText, "EQ%dLevelLabel", cntBand+1);
    TLabel *LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      LevelLabel->Font->Size = LabelFontSize;
    }

    sprintf(tempText, "EQ%dFrequencyLabel", cntBand+1);
    LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      LevelLabel->Font->Size = LabelFontSize;
    }

    sprintf(tempText, "EQ%dBandwidthLabel", cntBand+1);
    LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      LevelLabel->Font->Size = LabelFontSize;
    }

    sprintf(tempText, "EQ%dTypeLabel", cntBand+1);
    LevelLabel = (TLabel *)FindFormControl(tempText);
    if (LevelLabel != NULL)
    {
      LevelLabel->Font->Size = LabelFontSize;
    }

    EQOnLabel->Font->Size = LabelFontSize;
  }

  EQWindow->Font->Size = LabelFontSize;
  EQWindow->AxisBorderWidth = EQ1LevelLabel->Height+4;
  EQWindow->AxisLeftMargin = (EQ1LevelLabel->Width/2);

  float Size = EQ1LevelLabel->Height/4;
  if (Size<1)
  {
    Size = 1;
  }
  else if (Size>4)
  {
    Size = 4;
  }
  EQWindow->AnchorSize = Size;
}

void __fastcall TEQWindowDialog::FormShow(TObject *Sender)
{
  TControl *ChildControl;

  for (int cnt=0; cnt<ControlCount; cnt++)
  {
    ChildControl = Controls[cnt];
    ControlOriginalTop[cnt] = ChildControl->Top;
    ControlOriginalLeft[cnt] = ChildControl->Left;
    ControlOriginalHeight[cnt] = ChildControl->Height;
    ControlOriginalWidth[cnt] = ChildControl->Width;
  }
}
//---------------------------------------------------------------------------



