//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MambaNetFrm.h"
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
#pragma resource "*.dfm"

TMambaNetForm *MambaNetForm;

//---------------------------------------------------------------------------
__fastcall TMambaNetForm::TMambaNetForm(TComponent* Owner)
   : TForm(Owner)
{
  mbn = NULL;
}
//---------------------------------------------------------------------------

__fastcall TMambaNetForm::~TMambaNetForm()
{
}

void TMambaNetForm::MambaNetError(int code, char *msg)
{
}

void TMambaNetForm::MambaNetOnlineStatus(unsigned long addr, char valid)
{
}

int TMambaNetForm::MambaNetSetActuatorData(unsigned short object, union mbn_data data)
{
  return 0;
}

void __fastcall TMambaNetForm::FormActivate(TObject *Sender)
{
  FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TMambaNetForm::FormCanResize(TObject *Sender,
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

void __fastcall TMambaNetForm::FormResize(TObject *Sender)
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
}
//---------------------------------------------------------------------------

void __fastcall TMambaNetForm::FormShow(TObject *Sender)
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

