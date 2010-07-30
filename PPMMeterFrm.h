//---------------------------------------------------------------------------

#ifndef PPMMeterFrmH
#define PPMMeterFrmH
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
//---------------------------------------------------------------------------
class TPPMMeterForm : public TForm
{
__published:	// IDE-managed Components
   TPanel *PPMPanel;
   TImage *BBCPPMMeterBackgroundImage;
   TImage *DigitalPPMMeterBackgroundImage;
   TImage *AnalogClockPPMMeterBackgroundImage;
   TImage *AnalogPPMMeterBackgroundImage;
   TMeterPanel *BigLeftMeterPanel1;
   TMeterPanel *BigRightMeterPanel1;
   TMeterPanel *BigLeftMeterPanel2;
   TMeterPanel *BigRightMeterPanel2;
   TAnalogClock *BigAnalogClock;
   TStaticText *BigMasterMeter1StaticText;
   TStaticText *BigMasterMeter2StaticText;
   TGradientFill *GradientFill1;
   TPopupMenu *BigMeter1SourcePopupMenu;
   TMenuItem *BigMeter1MeterSourceProgMenuItem;
   TMenuItem *BigMeter1MeterSourceMasterMenuItem;
   TMenuItem *BigMeter1MeterSourceSubMenuItem;
   TMenuItem *BigMeter1MeterSourceAux1MenuItem;
   TMenuItem *BigMeter1MeterSourceAux2MenuItem;
   TMenuItem *BigMeter1MeterSourceCUEMenuItem;
   TPopupMenu *BigMeter2SourcePopupMenu;
   TMenuItem *BigMeter2MeterSourceProgMenuItem;
   TMenuItem *BigMeter2MeterSourceMasterMenuItem;
   TMenuItem *BigMeter2MeterSourceSubMenuItem;
   TMenuItem *BigMeter2MeterSourceAux1MenuItem;
   TMenuItem *BigMeter2MeterSourceAux2MenuItem;
   TMenuItem *BigMeter2MeterSourceCUEMenuItem;
   TMenuItem *BigMeter1MeterSourceCRMMenuItem;
   TMenuItem *BigMeter1MeterSourceStudio1MenuItem;
   TMenuItem *BigMeter1MeterSourceStudio2MenuItem;
   TMenuItem *BigMeter1MeterSourceStudio3MenuItem;
   TMenuItem *BigMeter1MeterSourceStudio4MenuItem;
   TMenuItem *BigMeter1MeterSourceStudio5MenuItem;
   TMenuItem *BigMeter1MeterSourceStudio6MenuItem;
   TMenuItem *BigMeter1MeterSourceStudio7MenuItem;
   TMenuItem *BigMeter1MeterSourceStudio8MenuItem;
   TMenuItem *BigMeter2MeterSourceCRMMenuItem;
   TMenuItem *BigMeter2MeterSourceStudio1MenuItem;
   TMenuItem *BigMeter2MeterSourceStudio2MenuItem;
   TMenuItem *BigMeter2MeterSourceStudio3MenuItem;
   TMenuItem *BigMeter2MeterSourceStudio4MenuItem;
   TMenuItem *BigMeter2MeterSourceStudio5MenuItem;
   TMenuItem *BigMeter2MeterSourceStudio6MenuItem;
   TMenuItem *BigMeter2MeterSourceStudio7MenuItem;
   TMenuItem *BigMeter2MeterSourceStudio8MenuItem;
   TTimer *MeterReleaseTimer;
   TTimer *Timer2;
  TPanel *Panel1;
  TLabel *Label2;
  TLabel *Label1;
  TLabel *Switch1;
  TLabel *Switch2;
  TLabel *Switch3;
  TLabel *Switch4;
  TLabel *Switch5;
  TLabel *Switch6;
  TLabel *Switch7;
  TLabel *Switch8;
  TLabel *Switch9;
  TLabel *Switch10;
  TLabel *Switch11;
  TLabel *Switch12;
  TLabel *Switch14;
  TLabel *Switch13;
  TLabel *Switch15;
  TLabel *Switch16;
  TEdit *Edit1;
  TLabel *Switch17;
  TLabel *Switch18;
  TLabel *Switch19;
  TLabel *Switch20;
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
  TLabel *Switch30;
  TLabel *Switch29;
  TLabel *Switch28;
  TLabel *Switch27;
  TLabel *Switch26;
  TLabel *Switch25;
  TLabel *Switch24;
  TLabel *Switch23;
  TLabel *Switch22;
  TLabel *Switch21;
  TLabel *Switch40;
  TLabel *Switch39;
  TLabel *Switch38;
  TLabel *Switch37;
  TLabel *Switch36;
  TLabel *Switch35;
  TLabel *Switch34;
  TLabel *Switch33;
  TLabel *Switch32;
  TLabel *Switch31;
  TLabel *Switch61;
  TLabel *Switch51;
  TLabel *Switch41;
  TLabel *Switch42;
  TLabel *Switch52;
  TLabel *Switch62;
  TLabel *Switch63;
  TLabel *Switch53;
  TLabel *Switch43;
  TLabel *Switch44;
  TLabel *Switch54;
  TLabel *Switch64;
  TLabel *Switch65;
  TLabel *Switch55;
  TLabel *Switch45;
  TLabel *Switch46;
  TLabel *Switch56;
  TLabel *Switch66;
  TLabel *Switch67;
  TLabel *Switch57;
  TLabel *Switch47;
  TLabel *Switch48;
  TLabel *Switch58;
  TLabel *Switch68;
  TLabel *Switch69;
  TLabel *Switch59;
  TLabel *Switch49;
  TLabel *Switch50;
  TLabel *Switch60;
  TLabel *Switch70;
  TEQPanel *EQPanel1;
  TButton *ButtonUp;
  TButton *ButtonDown;
  TPhaseMeter *PhaseMeter1;
  TPhaseMeter *PhaseMeter2;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall MeterReleaseTimerTimer(TObject *Sender);
   void __fastcall Timer2Timer(TObject *Sender);
   void __fastcall SwitchMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall SwitchMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall Edit1Change(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall EQPanel1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall EQPanel1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall EQPanel1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall ButtonUpClick(TObject *Sender);
  void __fastcall ButtonDownClick(TObject *Sender);
private:	// User declarations
    EQAnchor DragEQ;
public:		// User declarations
  float MeterData[4];
  float PhaseData[2];
  struct mbn_handler *mbn;
  struct mbn_if_ethernet *iflist;
  unsigned int MambaNetAddress;
  unsigned char Valid;
   __fastcall TPPMMeterForm(TComponent* Owner);
   void __fastcall StopNetwork();
};
//---------------------------------------------------------------------------
extern PACKAGE TPPMMeterForm *PPMMeterForm;
//---------------------------------------------------------------------------
#endif
