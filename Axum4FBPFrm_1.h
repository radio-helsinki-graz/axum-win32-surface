//---------------------------------------------------------------------------

#ifndef Axum4FBPFrm1H
#define Axum4FBPFrm1H
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
#include "ATImage.hpp"
#define MBN_VARARG
#include "mbn.h"
#include "axum_form.h"
#include "MambaNetFrm.h"

//---------------------------------------------------------------------------
class TAxum4FBPForm_1 : public TMambaNetForm
{
__published:    // IDE-managed Components
  TATImage *SmallOffImage;
  TATImage *SmallRedImage;
  TATImage *SmallGreenImage;
  TATImage *SmallYellowImage;
  TATImage *LargeOffImage;
  TATImage *LargeRedImage;
  TATImage *LargeGreenImage;
  TATImage *LargeYellowImage;
  TATImage *Switch1_1Image;
  TATImage *Switch2_1Image;
  TATImage *Switch3_1Image;
  TATImage *Switch4_1Image;
  TATImage *Switch1_2Image;
  TATImage *Switch2_2Image;
  TATImage *Switch3_2Image;
  TATImage *Switch4_2Image;
  TATImage *Switch1_3Image;
  TATImage *Switch1_4Image;
  TATImage *Switch2_3Image;
  TATImage *Switch2_4Image;
  TATImage *Switch3_3Image;
  TATImage *Switch3_4Image;
  TATImage *Switch4_3Image;
  TATImage *Switch4_4Image;
  TATImage *Switch2_5Image;
  TATImage *Switch3_5Image;
  TATImage *Switch4_5Image;
  TATImage *Switch1_6Image;
  TATImage *Switch2_6Image;
  TATImage *Switch3_6Image;
  TATImage *Switch4_6Image;
  TATImage *Switch1_7Image;
  TATImage *Switch2_7Image;
  TATImage *Switch3_7Image;
  TATImage *Switch4_7Image;
  TATImage *Switch4_8Image;
  TATImage *Switch3_8Image;
  TATImage *Switch2_8Image;
  TATImage *Switch1_8Image;
  TLabel *Display1_Line1;
  TLabel *Display1_Line2;
  TLabel *Display2_Line1;
  TLabel *Display2_Line2;
  TLabel *Display3_Line1;
  TLabel *Display3_Line2;
  TLabel *Display4_Line1;
  TLabel *Display4_Line2;
  TLabel *Encoder1_Up;
  TLabel *Encoder1_Down;
  TLabel *Encoder1_Reset;
  TLabel *Encoder2_Down;
  TLabel *Encoder2_Reset;
  TLabel *Encoder2_Up;
  TLabel *Encoder3_Up;
  TLabel *Encoder3_Reset;
  TLabel *Encoder3_Down;
  TLabel *Encoder4_Up;
  TLabel *Encoder4_Reset;
  TLabel *Encoder4_Down;
  TATImage *LedOffImage;
  TATImage *LedGreenImage;
  TATImage *LedRedImage;
  TATImage *Led1_1Image;
  TATImage *Led2_1Image;
  TATImage *Led3_1Image;
  TATImage *Led4_1Image;
  TATImage *Led1_2Image;
  TATImage *Led2_2Image;
  TATImage *Led3_2Image;
  TATImage *Led4_2Image;
  TATImage *Led1_3Image;
  TATImage *Led2_3Image;
  TATImage *Led3_3Image;
  TATImage *Led4_3Image;
  TATImage *Led1_4Image;
  TATImage *Led2_4Image;
  TATImage *Led3_4Image;
  TATImage *Led4_4Image;
  TATImage *Led1_5Image;
  TATImage *Led2_5Image;
  TATImage *Led3_5Image;
  TATImage *Led4_5Image;
  TATImage *Led1_6Image;
  TATImage *Led2_6Image;
  TATImage *Led3_6Image;
  TATImage *Led4_6Image;
  TATImage *Led1_7Image;
  TATImage *Led2_7Image;
  TATImage *Led3_7Image;
  TATImage *Led4_7Image;
  TATImage *Led1_8Image;
  TATImage *Led2_8Image;
  TATImage *Led3_8Image;
  TATImage *Led4_8Image;
  TFaderPanel *FaderPanel1;
  TFaderPanel *FaderPanel2;
  TFaderPanel *FaderPanel3;
  TFaderPanel *FaderPanel4;
  TATImage *Switch1_5Image;
  TLabel *Label1_6;
  TLabel *Label1_5;
  TLabel *Label1_2;
  TLabel *Label1_4;
  TLabel *Label1_3;
  TLabel *Label1_7;
  TLabel *Label1_8;
  TLabel *Label2_1;
  TLabel *Label2_2;
  TLabel *Label2_3;
  TLabel *Label2_4;
  TLabel *Label2_5;
  TLabel *Label2_6;
  TLabel *Label2_7;
  TLabel *Label2_8;
  TLabel *Label3_1;
  TLabel *Label3_2;
  TLabel *Label3_3;
  TLabel *Label3_4;
  TLabel *Label3_5;
  TLabel *Label3_6;
  TLabel *Label3_7;
  TLabel *Label3_8;
  TLabel *Label4_2;
  TLabel *Label4_1;
  TLabel *Label4_3;
  TLabel *Label4_4;
  TLabel *Label4_5;
  TLabel *Label4_6;
  TLabel *Label4_7;
  TLabel *Label4_8;
  TLabel *Label1_1;
  TLabel *Led1_1Label;
  TLabel *Led1_2Label;
  TLabel *Led1_3Label;
  TLabel *Led1_4Label;
  TLabel *Led1_8Label;
  TLabel *Led1_7Label;
  TLabel *Led1_6Label;
  TLabel *Led1_5Label;
  TLabel *Led2_1Label;
  TLabel *Led2_2Label;
  TLabel *Led2_3Label;
  TLabel *Led2_4Label;
  TLabel *Led2_5Label;
  TLabel *Led2_6Label;
  TLabel *Led2_7Label;
  TLabel *Led2_8Label;
  TLabel *Led3_1Label;
  TLabel *Led3_2Label;
  TLabel *Led3_3Label;
  TLabel *Led3_4Label;
  TLabel *Led3_5Label;
  TLabel *Led3_6Label;
  TLabel *Led3_7Label;
  TLabel *Led3_8Label;
  TLabel *Led4_1Label;
  TLabel *Led4_2Label;
  TLabel *Led4_3Label;
  TLabel *Led4_4Label;
  TLabel *Led4_5Label;
  TLabel *Led4_6Label;
  TLabel *Led4_7Label;
  TLabel *Led4_8Label;
  TATImage *Axum4FBPBackgroundATImage;
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
  void __fastcall FaderPanelMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall FaderPanelMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall FaderPanelMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall SwitchLabelMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall SwitchLabelMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:    // User declarations
  TPicture *GetSmallSwitchPicture(unsigned char Color);
  TPicture *GetLargeSwitchPicture(unsigned char Color);
  void UpdateSwitch(unsigned char ModuleNr, unsigned char SwitchNr);
  int DisplayFontSize;
  int SmallFontSize;
  int LargeFontSize;
  int LedFontSize;
public:     // User declarations
  unsigned char SwitchState[4][8];
  unsigned char OnColor[4][8];
  unsigned char OffColor[4][8];
   __fastcall TAxum4FBPForm_1(TComponent* Owner, char *url, form_node_info *node_info);
   __fastcall ~TAxum4FBPForm_1();
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
extern PACKAGE TAxum4FBPForm_1 *Axum4FBPForm_1;
//---------------------------------------------------------------------------
#endif
