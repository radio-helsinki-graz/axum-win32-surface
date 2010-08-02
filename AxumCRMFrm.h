//---------------------------------------------------------------------------

#ifndef AxumCRMFrmH
#define AxumCRMFrmH
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
#define MBN_VARARG
#include "mbn.h"
#include "axum_form.h"
#include "MambaNetFrm.h"

//---------------------------------------------------------------------------
class TAxumCRMForm : public TMambaNetForm
{
__published:    // IDE-managed Components
  TImage *SmallOffImage;
  TImage *SmallRedImage;
  TImage *SmallGreenImage;
  TImage *SmallYellowImage;
  TImage *Switch1Image;
  TImage *Switch3Image;
  TImage *Switch2Image;
  TImage *Switch4Image;
  TLabel *Encoder_Up;
  TLabel *Encoder_Down;
  TLabel *Encoder_Reset;
  TImage *LedOffImage;
  TImage *LedGreenImage;
  TImage *LedRedImage;
  TImage *Led1Image;
  TImage *Led2Image;
  TImage *Led3Image;
  TImage *Led4Image;
  TImage *Led5Image;
  TImage *Led6Image;
  TImage *Led7Image;
  TImage *AxumCRMBackgroundImage;
  TImage *Switch5Image;
  TImage *Switch6Image;
  TImage *Switch7Image;
  TImage *Switch8Image;
  TImage *Switch12Image;
  TImage *Switch11Image;
  TImage *Switch10Image;
  TImage *Switch9Image;
  TImage *Switch13Image;
  TImage *Switch14Image;
  TImage *Switch15Image;
  TImage *Switch16Image;
  TImage *Switch17Image;
  TImage *Switch18Image;
  TImage *Switch19Image;
  TImage *Switch20Image;
  TImage *Switch21Image;
  TImage *Switch22Image;
  TImage *Switch23Image;
  TImage *Switch24Image;
  TImage *Switch37Image;
  TImage *Switch38Image;
  TImage *Switch40Image;
  TImage *Switch39Image;
  TImage *Switch26Image;
  TImage *Switch25Image;
  TImage *Switch42Image;
  TImage *Switch41Image;
  TImage *Switch28Image;
  TImage *Switch27Image;
  TImage *Switch44Image;
  TImage *Switch43Image;
  TImage *Switch30Image;
  TImage *Switch29Image;
  TImage *Switch46Image;
  TImage *Switch45Image;
  TImage *Switch32Image;
  TImage *Switch31Image;
  TImage *Switch48Image;
  TImage *Switch47Image;
  TImage *Switch34Image;
  TImage *Switch33Image;
  TImage *Switch35Image;
  TImage *Switch36Image;
  TImage *Switch49Image;
  TImage *Switch50Image;
  TImage *Switch51Image;
  TImage *Switch52Image;
  TImage *Switch53Image;
  TImage *Switch54Image;
  TKnob *PotentioMeter1Knob;
  TKnob *PotentioMeter2Knob;
  TKnob *PotentioMeter3Knob;
  TKnob *PotentioMeter4Knob;
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
private:    // User declarations
  TPicture *GetSmallSwitchPicture(unsigned char Color);
  TControl *FindFormControl(char *Name);
  void UpdateSwitch(unsigned char SwitchNr);
public:     // User declarations
  unsigned char SwitchState[54];
  unsigned char OnColor[54];
  unsigned char OffColor[54];
   __fastcall TAxumCRMForm(TComponent* Owner, char *url, form_node_info *node_info);
   __fastcall ~TAxumCRMForm();
  void MambaNetError(int code, char *msg);
  void MambaNetOnlineStatus(unsigned long addr, char valid);
  int MambaNetSetActuatorData(unsigned short object, union mbn_data data);
  void ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description);
  void StartCommunication();
};
//---------------------------------------------------------------------------
extern PACKAGE TAxumCRMForm *AxumCRMForm;
//---------------------------------------------------------------------------
#endif
