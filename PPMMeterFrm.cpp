//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PPMMeterFrm.h"
#include "EthernetAdapterSelectionFrm.h"
#define MBN_VARARG
#include "mbn.h"
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
#pragma resource "*.dfm"

TPPMMeterForm *PPMMeterForm;

#define PCB_MAJOR_VERSION        1
#define PCB_MINOR_VERSION        0

#define FIRMWARE_MAJOR_VERSION   2
#define FIRMWARE_MINOR_VERSION   0

#define MANUFACTURER_ID          1      //D&R
#define PRODUCT_ID               21     //Axum-PPM-Meter

struct mbn_node_info thisnode = {
  0, 0,
  "Axum PPM Meters DSP (Windows)", "Axum-PPM-Meter",
  MANUFACTURER_ID, PRODUCT_ID, 0,
  0, 0,
  FIRMWARE_MAJOR_VERSION, FIRMWARE_MINOR_VERSION,
};

void mError(struct mbn_handler *mbn, int code, char *msg);
void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid);
int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data);

//---------------------------------------------------------------------------
__fastcall TPPMMeterForm::TPPMMeterForm(TComponent* Owner)
   : TForm(Owner)
{
  MeterData[0] = -50;
  MeterData[1] = -50;
  MeterData[2] = -50;
  MeterData[3] = -50;
  MambaNetAddress = 0x00000000;
  Valid = 0;
}
//---------------------------------------------------------------------------


void __fastcall TPPMMeterForm::FormShow(TObject *Sender)
{
  char err[MBN_ERRSIZE], ifdesc[512];
  struct mbn_if_ethernet *n;
  struct mbn_interface *itf;
  struct mbn_object objects[200];
  int cntObject = 0;

  objects[cntObject++] = MBN_OBJ("Meter 1 Left dB",
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, -50.0, 10.0, 0.0, 0.0);
  objects[cntObject++] = MBN_OBJ("Meter 1 Right dB",
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, -50.0, 10.0, 0.0, 0.0);
  objects[cntObject++] = MBN_OBJ("Meter 2 Left dB",
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, -50.0, 10.0, 0.0, 0.0);
  objects[cntObject++] = MBN_OBJ("Meter 2 Right dB",
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, -50.0, 10.0, 0.0, 0.0);
  objects[cntObject++] = MBN_OBJ("Display 1",
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_OCTETS, 8, 0, 127, 0, "");
  objects[cntObject++] = MBN_OBJ("Float edit",
                                 MBN_DATATYPE_NODATA,
                                 MBN_DATATYPE_FLOAT, 2, -140.0, 10.0, -140.0, -140.0);

  for (int cntSwitch=0; cntSwitch<70; cntSwitch++)
  {
    char desc[32];
    sprintf(desc,"Switch %d", cntSwitch+1);

    objects[cntObject++] = MBN_OBJ(desc,
                                   MBN_DATATYPE_STATE, 1, 1, 0, 1, 0,
                                   MBN_DATATYPE_STATE, 1, 0, 1, 0, 0);
  }

  for (int cntEQBand=0; cntEQBand<6; cntEQBand++)
  {
    char desc[32];
    sprintf(desc,"EQ %d level", cntEQBand+1);
    objects[cntObject++] = MBN_OBJ(desc,
                                   MBN_DATATYPE_FLOAT, 1, 2, -18.0, 18.0, 0.0,
                                   MBN_DATATYPE_FLOAT, 2, -18.0, 18.0, 0.0, 0.0);
    sprintf(desc,"EQ %d frequency", cntEQBand+1);
    objects[cntObject++] = MBN_OBJ(desc,
                                   MBN_DATATYPE_UINT, 1, 2, 10, 24000, 1000,
                                   MBN_DATATYPE_UINT, 2, 10, 24000, 1000);
    sprintf(desc,"EQ %d bandwidth", cntEQBand+1);
    objects[cntObject++] = MBN_OBJ(desc,
                                   MBN_DATATYPE_FLOAT, 1, 2, 0.1, 10.0, 1.0,
                                   MBN_DATATYPE_FLOAT, 2, 0.1, 10.0, 1.0, 1.0);
    sprintf(desc,"EQ %d type", cntEQBand+1);
    objects[cntObject++] = MBN_OBJ(desc,
                                   MBN_DATATYPE_STATE, 1, 2, 0, 7, 3,
                                   MBN_DATATYPE_STATE, 2, 0, 7, 3);
  }
  objects[cntObject++] = MBN_OBJ("Module select",
                                 MBN_DATATYPE_SINT, 1, 2, -10, 10, 0,
                                 MBN_DATATYPE_NODATA);

  thisnode.NumberOfObjects = cntObject;

  iflist = mbnEthernetIFList(err);

  for (n=iflist; n!=NULL; n=n->next)
  {
    EthernetAdapterSelectionForm->AdapterComboBox->Items->AddObject(n->desc, (TObject *)n->name);
  }

  if (EthernetAdapterSelectionForm->ShowModal() ==  mrOk)
  {
    thisnode.UniqueIDPerProduct = EthernetAdapterSelectionForm->CSpinEdit1->Value;

    if (EthernetAdapterSelectionForm->EthernetRadioButton->Checked)
    {
      if(EthernetAdapterSelectionForm->AdapterComboBox->ItemIndex < 0)
        return;
      if((itf = mbnEthernetOpen((char *)EthernetAdapterSelectionForm->AdapterComboBox->Items->Objects[EthernetAdapterSelectionForm->AdapterComboBox->ItemIndex], err)) == NULL)
        return;
    }
    else
    {  //IP
      if((itf = mbnUDPOpen(EthernetAdapterSelectionForm->URLEdit->Text.c_str(), "34848", NULL, err)) == NULL)
        return;
//      if((itf = mbnTCPOpen(EthernetAdapterSelectionForm->URLEdit->Text.c_str(), NULL, NULL, NULL, err)) == NULL)
//        return;
    }

    mbn = mbnInit(&thisnode, objects, itf, err);

    mbnSetErrorCallback(mbn, mError);
    mbnSetOnlineStatusCallback(mbn, mOnlineStatus);
    mbnSetSetActuatorDataCallback(mbn, mSetActuatorData);

    mbnStartInterface(itf, err);
  }
}

void __fastcall TPPMMeterForm::MeterReleaseTimerTimer(TObject *Sender)
{
   if (BigLeftMeterPanel1->dBPosition>-50)
   {
      if (MeterData[0] < BigLeftMeterPanel1->dBPosition)
      {
         BigLeftMeterPanel1->dBPosition-=0.25;
      }
   }

   if (BigRightMeterPanel1->dBPosition>-50)
   {
      if (MeterData[1] < BigRightMeterPanel1->dBPosition)
      {
         BigRightMeterPanel1->dBPosition-=0.25;
      }
   }

   if (BigLeftMeterPanel2->dBPosition>-50)
   {
      if (MeterData[2] < BigLeftMeterPanel2->dBPosition)
      {
         BigLeftMeterPanel2->dBPosition-=0.25;
      }
   }

   if (BigRightMeterPanel2->dBPosition>-50)
   {
      if (MeterData[3] < BigRightMeterPanel2->dBPosition)
      {
         BigRightMeterPanel2->dBPosition-=0.25;
      }
   }

#define PHASE_STEPSIZE 0.0075
   if (PPMMeterForm->PhaseData[0] > (PPMMeterForm->PhaseMeter1->Position+PHASE_STEPSIZE))
     PPMMeterForm->PhaseMeter1->Position += PHASE_STEPSIZE;
   else if (PhaseData[0] < (PPMMeterForm->PhaseMeter1->Position-PHASE_STEPSIZE))
     PPMMeterForm->PhaseMeter1->Position -= PHASE_STEPSIZE;
}
//---------------------------------------------------------------------------

void __fastcall TPPMMeterForm::Timer2Timer(TObject *Sender)
{
  struct time T;
  gettime(&T);

  const AnsiString DateFormat = "d mmm yyyy";
  const AnsiString DayFormat = "ddd";
  const AnsiString TimeFormat = "hh:mm:ss am/pm";

  BigAnalogClock->Seconds = T.ti_sec;
  BigAnalogClock->Minutes = T.ti_min;
  BigAnalogClock->Hours = T.ti_hour;

  struct date D;
  getdate(&D);

  TDateTime Date;
  Date = EncodeTime(T.ti_hour, T.ti_min, T.ti_sec, 0);
  AnsiString Time = Date.FormatString(TimeFormat);

  Date = EncodeDate(D.da_year, D.da_mon, D.da_day);
  BigAnalogClock->TimeString = Date.FormatString(DayFormat) + " " + Time;
  BigAnalogClock->DateString = Date.FormatString(DateFormat);
  BigAnalogClock->PaintControl();
}
//---------------------------------------------------------------------------


void __fastcall TPPMMeterForm::SwitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if ((Shift.Contains(ssLeft)) && (Valid))
   {
      int ObjectNr = 1030+((TLabel *)Sender)->Caption.ToInt()-1;
      union mbn_data data;

      data.State = 1;
      mbnUpdateSensorData(mbn, ObjectNr, data);
   }
}
//---------------------------------------------------------------------------

void __fastcall TPPMMeterForm::SwitchMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if ((Button == Controls::mbLeft) && (Valid))
   {
      int ObjectNr = 1030+((TLabel *)Sender)->Caption.ToInt()-1;
      union mbn_data data;

      data.State = 0;
      mbnUpdateSensorData(mbn, ObjectNr, data);
   }
}
//---------------------------------------------------------------------------

void __fastcall TPPMMeterForm::Edit1Change(TObject *Sender)
{
  int ObjectNr = 1029;
  float Value = Edit1->Text.ToDouble();
  union mbn_data data;

  if (Valid)
  {
    data.Float = Value;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void mError(struct mbn_handler *mbn, int code, char *msg) {
  printf(msg);
}

void mOnlineStatus(struct mbn_handler *mbn, unsigned long addr, char valid) {
  PPMMeterForm->MambaNetAddress = addr;
  PPMMeterForm->Valid = valid;
}

int mSetActuatorData(struct mbn_handler *mbn, unsigned short object, union mbn_data data) {
  if ((object>=1030) && (object<1030+70))
  {
    AnsiString ControlName = ((AnsiString)"Switch") + ((AnsiString)(object-1029));
    TLabel *SendLabel = (TLabel *)PPMMeterForm->FindComponent(ControlName);

    if (data.State)
    {
      SendLabel->Color = 0x008080FF;
    }
    else
    {
      SendLabel->Color = 0x00E0E0E0;
    }
  }
  if ((object>=1100) && (object<1124))
  {
    int BandNr = (object-1100)/4;
    int Func = (object-1100)%4;

    switch (Func)
    {
      case 0:
      { //level
        switch (BandNr)
        {
          case 0:
          {
            PPMMeterForm->EQPanel1->GainBand1 = data.Float;
          }
          break;
          case 1:
          {
            PPMMeterForm->EQPanel1->GainBand2 = data.Float;
          }
          break;
          case 2:
          {
            PPMMeterForm->EQPanel1->GainBand3 = data.Float;
          }
          break;
          case 3:
          {
            PPMMeterForm->EQPanel1->GainBand4 = data.Float;
          }
          break;
        }
      }
      break;
      case 1:
      { //frequency
        switch (BandNr)
        {
          case 0:
          {
            PPMMeterForm->EQPanel1->FrequencyBand1 = data.UInt;
          }
          break;
          case 1:
          {
            PPMMeterForm->EQPanel1->FrequencyBand2 = data.UInt;
          }
          break;
          case 2:
          {
            PPMMeterForm->EQPanel1->FrequencyBand3 = data.UInt;
          }
          break;
          case 3:
          {
            PPMMeterForm->EQPanel1->FrequencyBand4 = data.UInt;
          }
          break;
        }
      }
      break;
      case 2:
      { //bandwidth
        switch (BandNr)
        {
          case 0:
          {
            PPMMeterForm->EQPanel1->BandwidthBand1 = data.Float;
          }
          break;
          case 1:
          {
            PPMMeterForm->EQPanel1->BandwidthBand2 = data.Float;
          }
          break;
          case 2:
          {
            PPMMeterForm->EQPanel1->BandwidthBand3 = data.Float;
          }
          break;
          case 3:
          {
            PPMMeterForm->EQPanel1->BandwidthBand4 = data.Float;
          }
          break;
        }
      }
      break;
      case 3:
      { //type
        FilterType Type;
        bool On = true;

        switch (data.State)
        {
          case 0:
          {
            On = false;
            Type = PEAKINGEQ;
          }
          break;
          case 1:
          {
            Type = HPF;
          }
          break;
          case 2:
          {
            Type = LOWSHELF;
          }
          break;
          case 3:
          {
            Type = PEAKINGEQ;
          }
          break;
          case 4:
          {
            Type = HIGHSHELF;
          }
          break;
          case 5:
          {
            Type = LPF;
          }
          break;
          case 6:
          {
            Type = BPF;
          }
          break;
          case 7:
          {
            Type = NOTCH;
          }
          break;
        }

        switch (BandNr)
        {
          case 0:
          {
            PPMMeterForm->EQPanel1->OnBand1 = On;
            PPMMeterForm->EQPanel1->TypeBand1 = Type;
          }
          break;
          case 1:
          {
            PPMMeterForm->EQPanel1->OnBand2 = On;
            PPMMeterForm->EQPanel1->TypeBand2 = Type;
          }
          break;
          case 2:
          {
            PPMMeterForm->EQPanel1->OnBand3 = On;
            PPMMeterForm->EQPanel1->TypeBand3 = Type;
          }
          break;
          case 3:
          {
            PPMMeterForm->EQPanel1->TypeBand4 = Type;
            PPMMeterForm->EQPanel1->OnBand4 = On;
          }
          break;
        }
      }
      break;
    }
  }
  else
  {
    switch (object)
    {
      case 1024:
      {
        float dB = data.Float+20;

        PPMMeterForm->MeterData[0] = dB;
        if (dB>PPMMeterForm->BigLeftMeterPanel1->dBPosition)
        {
          PPMMeterForm->BigLeftMeterPanel1->dBPosition = dB;
        }
      }
      break;
      case 1025:
      {
        float dB = data.Float+20;

        PPMMeterForm->MeterData[1] = dB;
        if (dB>PPMMeterForm->BigRightMeterPanel1->dBPosition)
        {
          PPMMeterForm->BigRightMeterPanel1->dBPosition = dB;
        }
      }
      break;
      case 1026:
      {
        float dB = data.Float+20;

        PPMMeterForm->MeterData[2] = dB;
        if (dB>PPMMeterForm->BigLeftMeterPanel2->dBPosition)
        {
          PPMMeterForm->BigLeftMeterPanel2->dBPosition = dB;
        }
      }
      break;
      case 1027:
      {
        float dB = data.Float+20;

        PPMMeterForm->MeterData[3] = dB;
        if (dB>PPMMeterForm->BigRightMeterPanel2->dBPosition)
        {
          PPMMeterForm->BigRightMeterPanel2->dBPosition = dB;
        }

        if ((data.Float>=0) && (data.Float<=2))
          PPMMeterForm->PhaseData[0] = data.Float;
      }
      break;
      case 1028:
      {
        PPMMeterForm->Label1->Caption = (char *)data.Octets;
      }
      break;
      case 1029:
      {
        char TempString[32];
        sprintf(TempString, "%5.1f", data.Float);
        PPMMeterForm->Edit1->Text = TempString;
      }
      break;
    }
  }
  mbnUpdateActuatorData(mbn, object, data);

  return 0;
}


void __fastcall TPPMMeterForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  mbnFree(mbn);

  Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TPPMMeterForm::EQPanel1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Shift.Contains(ssLeft))
   {
      DragEQ = EQPanel1->GetAnchorFromXY(X,Y);
   }
   else
   {
      DragEQ = NoEQAnchor;
   }
}
//---------------------------------------------------------------------------

void __fastcall TPPMMeterForm::EQPanel1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
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

            EQPanel1->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQPanel1->GainBand1 != Gain)
            {
              ObjectNr = 1100;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->GainBand1 = Gain;
            }
            if (EQPanel1->FrequencyBand1 != Frequency)
            {
              ObjectNr = 1101;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->FrequencyBand1 = Frequency;
            }
         }
         break;
         case Band2:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQPanel1->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQPanel1->GainBand2 != Gain)
            {
              ObjectNr = 1104;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->GainBand2 = Gain;
            }
            if (EQPanel1->FrequencyBand2 != Frequency)
            {
              ObjectNr = 1105;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->FrequencyBand2 = Frequency;
            }
         }
         break;
         case Band3:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQPanel1->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQPanel1->GainBand3 != Gain)
            {
              ObjectNr = 1108;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->GainBand3 = Gain;
            }
            if (EQPanel1->FrequencyBand3 != Frequency)
            {
              ObjectNr = 1109;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->FrequencyBand3 = Frequency;
            }
         }
         break;
         case Band4:
         {
            float Gain;
            int Frequency;
            int ObjectNr;
            union mbn_data data;

            EQPanel1->GetGainFreqFromXY(&Gain, &Frequency, X, Y);

            if (EQPanel1->GainBand4 != Gain)
            {
              ObjectNr = 1112;
              data.Float = Gain;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->GainBand4 = Gain;
            }
            if (EQPanel1->FrequencyBand4 != Frequency)
            {
              ObjectNr = 1113;
              data.UInt = Frequency;
              mbnUpdateSensorData(mbn, ObjectNr, data);
              EQPanel1->FrequencyBand4 = Frequency;
            }
         }
         break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TPPMMeterForm::EQPanel1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  DragEQ = NoEQAnchor;
}
//---------------------------------------------------------------------------




void __fastcall TPPMMeterForm::ButtonUpClick(TObject *Sender)
{
  if (Valid)
  {
    int ObjectNr = 1124;
    union mbn_data data;

    data.SInt = 1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

void __fastcall TPPMMeterForm::ButtonDownClick(TObject *Sender)
{
  if (Valid)
  {
    int ObjectNr = 1124;
    union mbn_data data;

    data.SInt = -1;
    mbnUpdateSensorData(mbn, ObjectNr, data);
  }
}
//---------------------------------------------------------------------------

