//---------------------------------------------------------------------------

#ifndef AxumSuperModuleFrmH
#define AxumSuperModuleFrmH
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
class TAxumSuperModuleForm : public TMambaNetForm
{
__published:    // IDE-managed Components
  TImage *AxumSuperModuleBackgroundImage;
  TImage *SmallOffImage;
  TImage *SmallRedImage;
  TImage *SmallGreenImage;
  TImage *SmallYellowImage;
  TImage *LargeOffImage;
  TImage *LargeRedImage;
  TImage *LargeGreenImage;
  TImage *LargeYellowImage;
  TImage *Switch1Image;
  TImage *Switch2Image;
  TLabel *Display1;
  TLabel *Display2;
  TLabel *Encoder_Up;
  TLabel *Encoder_Down;
  TLabel *Encoder1_Reset;
  TLabel *Encoder2_Reset;
  TLabel *Encoder3_Reset;
  TLabel *Encoder4_Reset;
  TImage *LedOffImage;
  TImage *LedGreenImage;
  TImage *LedRedImage;
  TImage *Led1Image;
  TFaderPanel *FaderPanel1;
  TLabel *Switch2Label;
  TLabel *Switch1Label;
  TLabel *Led1Label;
   void __fastcall SwitchMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall SwitchMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall SwitchLabelMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall SwitchLabelMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:    // User declarations
  TPicture *GetSmallSwitchPicture(unsigned char Color);
  TPicture *GetLargeSwitchPicture(unsigned char Color);
  int DisplayFontSize;
  int SmallFontSize;
  int LargeFontSize;
  int LedFontSize;
public:     // User declarations
   __fastcall TAxumSuperModuleForm(TComponent* Owner, char *url, form_node_info *node_info);
   __fastcall ~TAxumSuperModuleForm();
  void MambaNetError(int code, char *msg);
  void MambaNetOnlineStatus(unsigned long addr, char valid);
  int MambaNetSetActuatorData(unsigned short object, union mbn_data data);
  void ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description);
  void StartCommunication();
  void CalculateFontSizes();
};
//---------------------------------------------------------------------------
extern PACKAGE TAxumSuperModuleForm *AxumSuperModuleForm;
//---------------------------------------------------------------------------
#endif
