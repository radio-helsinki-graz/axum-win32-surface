//---------------------------------------------------------------------------

#ifndef Axum4FBPFrmH
#define Axum4FBPFrmH
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
#define MBN_VARARG
#include "mbn.h"
#include "axum_form.h"
#include "MambaNetFrm.h"

//---------------------------------------------------------------------------
class TAxum4FBPForm : public TMambaNetForm
{
__published:	// IDE-managed Components
  TImage *Axum4FBPBackgroundImage;
  TImage *SmallOffImage;
  TImage *SmallRedImage;
  TImage *SmallGreenImage;
  TImage *SmallYellowImage;
  TImage *LargeOffImage;
  TImage *LargeRedImage;
  TImage *LargeGreenImage;
  TImage *LargeYellowImage;
  TImage *Switch1_1Image;
  TImage *Switch2_1Image;
  TImage *Switch3_1Image;
  TImage *Switch4_1Image;
  TImage *Switch1_2Image;
  TImage *Switch2_2Image;
  TImage *Switch3_2Image;
  TImage *Switch4_2Image;
  TImage *Switch1_3Image;
  TImage *Switch1_4Image;
  TImage *Switch2_3Image;
  TImage *Switch2_4Image;
  TImage *Switch3_3Image;
  TImage *Switch3_4Image;
  TImage *Switch4_3Image;
  TImage *Switch4_4Image;
  TImage *Switch2_5Image;
  TImage *Switch3_5Image;
  TImage *Switch4_5Image;
  TImage *Switch1_6Image;
  TImage *Switch2_6Image;
  TImage *Switch3_6Image;
  TImage *Switch4_6Image;
  TImage *Switch1_7Image;
  TImage *Switch2_7Image;
  TImage *Switch3_7Image;
  TImage *Switch4_7Image;
  TImage *Switch4_8Image;
  TImage *Switch3_8Image;
  TImage *Switch2_8Image;
  TImage *Switch1_8Image;
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
  TImage *LedOffImage;
  TImage *LedGreenImage;
  TImage *LedRedImage;
  TImage *Led1_1Image;
  TImage *Led2_1Image;
  TImage *Led3_1Image;
  TImage *Led4_1Image;
  TImage *Led1_2Image;
  TImage *Led2_2Image;
  TImage *Led3_2Image;
  TImage *Led4_2Image;
  TImage *Led1_3Image;
  TImage *Led2_3Image;
  TImage *Led3_3Image;
  TImage *Led4_3Image;
  TImage *Led1_4Image;
  TImage *Led2_4Image;
  TImage *Led3_4Image;
  TImage *Led4_4Image;
  TImage *Led1_5Image;
  TImage *Led2_5Image;
  TImage *Led3_5Image;
  TImage *Led4_5Image;
  TImage *Led1_6Image;
  TImage *Led2_6Image;
  TImage *Led3_6Image;
  TImage *Led4_6Image;
  TImage *Led1_7Image;
  TImage *Led2_7Image;
  TImage *Led3_7Image;
  TImage *Led4_7Image;
  TImage *Led1_8Image;
  TImage *Led2_8Image;
  TImage *Led3_8Image;
  TImage *Led4_8Image;
  TFaderPanel *FaderPanel1;
  TFaderPanel *FaderPanel2;
  TFaderPanel *FaderPanel3;
  TFaderPanel *FaderPanel4;
  TImage *Switch1_5Image;
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
private:	// User declarations
//  struct mbn_node_info thisnode;
  TPicture *GetSmallSwitchPicture(unsigned char Color);
  TPicture *GetLargeSwitchPicture(unsigned char Color);
  TControl *FindFormControl(char *Name);
  void UpdateSwitch(unsigned char ModuleNr, unsigned char SwitchNr);
  int DisplayFontSize;
  int SmallFontSize;
  int LargeFontSize;
public:		// User declarations
  unsigned char SwitchState[4][8];
  unsigned char OnColor[4][8];
  unsigned char OffColor[4][8];
   __fastcall TAxum4FBPForm(TComponent* Owner, char *url, form_node_info *node_info);
   __fastcall ~TAxum4FBPForm();
  void MambaNetError(int code, char *msg);
  void MambaNetOnlineStatus(unsigned long addr, char valid);
  int MambaNetSetActuatorData(unsigned short object, union mbn_data data);
  void ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description);
  void StartCommunication();
  void CalculateFontSizes();
};
//---------------------------------------------------------------------------
extern PACKAGE TAxum4FBPForm *Axum4FBPForm;
//---------------------------------------------------------------------------
#endif
