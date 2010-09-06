//---------------------------------------------------------------------------

#ifndef AxumCRMFrm1H
#define AxumCRMFrm1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "AnalogClock.h"
#include "MeterPanel.h"
#include <Graphics.hpp>
#include "GradientFill.h"
#include <Menus.hpp>
#include "ExtendedPanel.h"
#include "VUMeter.h"
#include "BitmapButton.h"
#include <ScktComp.hpp>
#include "EQPanel.h"
#include "PhaseMeter.h"
#include <jpeg.hpp>
#include "libpq-fe.h"
#include "FaderPanel.h"
#include "MambaNetFrm.h"
#include "Knob.h"
#include "ATImage.hpp"
#define MBN_VARARG
#include "mbn.h"
#include "axum_form.h"
#include "MambaNetFrm.h"

//---------------------------------------------------------------------------
class TAxumCRMForm_1 : public TMambaNetForm
{
__published:    // IDE-managed Components
  TATImage *SmallOffImage;
  TATImage *SmallRedImage;
  TATImage *SmallGreenImage;
  TATImage *SmallYellowImage;
  TATImage *Switch1Image;
  TATImage *Switch3Image;
  TATImage *Switch2Image;
  TATImage *Switch4Image;
  TLabel *Encoder_Up;
  TLabel *Encoder_Down;
  TLabel *Encoder_Reset;
  TATImage *LedOffImage;
  TATImage *LedGreenImage;
  TATImage *LedRedImage;
  TATImage *Led1Image;
  TATImage *Led2Image;
  TATImage *Led3Image;
  TATImage *Led4Image;
  TATImage *Led5Image;
  TATImage *Led6Image;
  TATImage *Led7Image;
  TATImage *Switch5Image;
  TATImage *Switch6Image;
  TATImage *Switch7Image;
  TATImage *Switch8Image;
  TATImage *Switch12Image;
  TATImage *Switch11Image;
  TATImage *Switch10Image;
  TATImage *Switch9Image;
  TATImage *Switch13Image;
  TATImage *Switch14Image;
  TATImage *Switch15Image;
  TATImage *Switch16Image;
  TATImage *Switch17Image;
  TATImage *Switch18Image;
  TATImage *Switch19Image;
  TATImage *Switch20Image;
  TATImage *Switch21Image;
  TATImage *Switch22Image;
  TATImage *Switch23Image;
  TATImage *Switch24Image;
  TATImage *Switch37Image;
  TATImage *Switch38Image;
  TATImage *Switch40Image;
  TATImage *Switch39Image;
  TATImage *Switch26Image;
  TATImage *Switch25Image;
  TATImage *Switch42Image;
  TATImage *Switch41Image;
  TATImage *Switch28Image;
  TATImage *Switch27Image;
  TATImage *Switch44Image;
  TATImage *Switch43Image;
  TATImage *Switch30Image;
  TATImage *Switch29Image;
  TATImage *Switch46Image;
  TATImage *Switch45Image;
  TATImage *Switch32Image;
  TATImage *Switch31Image;
  TATImage *Switch48Image;
  TATImage *Switch47Image;
  TATImage *Switch34Image;
  TATImage *Switch33Image;
  TATImage *Switch35Image;
  TATImage *Switch36Image;
  TATImage *Switch49Image;
  TATImage *Switch50Image;
  TATImage *Switch51Image;
  TATImage *Switch52Image;
  TATImage *Switch53Image;
  TATImage *Switch54Image;
  TKnob *PotentioMeter1Knob;
  TKnob *PotentioMeter2Knob;
  TKnob *PotentioMeter3Knob;
  TKnob *PotentioMeter4Knob;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label7;
  TLabel *Label8;
  TLabel *Label9;
  TLabel *Label10;
  TLabel *Label11;
  TLabel *Label12;
  TLabel *Label13;
  TLabel *Label14;
  TLabel *Label15;
  TLabel *Label16;
  TLabel *Label17;
  TLabel *Label18;
  TLabel *Label19;
  TLabel *Label20;
  TLabel *Label21;
  TLabel *Label22;
  TLabel *Label23;
  TLabel *Label24;
  TLabel *Label37;
  TLabel *Label38;
  TLabel *Label25;
  TLabel *Label26;
  TLabel *Label39;
  TLabel *Label40;
  TLabel *Label27;
  TLabel *Label28;
  TLabel *Label41;
  TLabel *Label42;
  TLabel *Label29;
  TLabel *Label30;
  TLabel *Label43;
  TLabel *Label44;
  TLabel *Label31;
  TLabel *Label32;
  TLabel *Label45;
  TLabel *Label46;
  TLabel *Label33;
  TLabel *Label34;
  TLabel *Label47;
  TLabel *Label48;
  TLabel *Label35;
  TLabel *Label36;
  TLabel *Label49;
  TLabel *Label50;
  TLabel *Label51;
  TLabel *Label53;
  TLabel *Label54;
  TLabel *Label52;
  TATImage *AxumCRMBackgroundATImage;
   void __fastcall SwitchMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall SwitchMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall Encoder_DownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall Encoder_UpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall Encoder_ResetMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall Encoder_ResetMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall PotentioMeter1KnobMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall LabelMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall LabelMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:    // User declarations
  TPicture *GetSmallSwitchPicture(unsigned char Color);
  void UpdateSwitch(unsigned char SwitchNr);
  int SmallSwitchFontSize;
public:     // User declarations
  unsigned char SwitchState[54];
  unsigned char OnColor[54];
  unsigned char OffColor[54];
   __fastcall TAxumCRMForm_1(TComponent* Owner, char *url, form_node_info *node_info);
   __fastcall ~TAxumCRMForm_1();
  void MambaNetError(int code, char *msg);
  void MambaNetOnlineStatus(unsigned long addr, char valid);
  int MambaNetSetActuatorData(unsigned short object, union mbn_data data);
  void ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description);
  void StartCommunication();
  void CalculateFontSizes();
  void PrintLabels(TCanvas *Canvas, float *xMm, float *yMm, float xPixelPerMm, float yPixelPerMm, float PageWidthMm, float PageHeightMm);
  bool PrintLabelsAvailable();
};
//---------------------------------------------------------------------------
extern PACKAGE TAxumCRMForm_1 *AxumCRMForm_1;
//---------------------------------------------------------------------------
#endif
