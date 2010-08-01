//---------------------------------------------------------------------------

#ifndef AxumMeterFrmH
#define AxumMeterFrmH
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
class TAxumMeterForm : public TMambaNetForm
{
__published:    // IDE-managed Components
  TMeterPanel *LeftMeterPanel1;
  TMeterPanel *RightMeterPanel1;
  TImage *AxumMeterBackgroundImage;
  TLabel *OnAirLabel;
  TLabel *Label1Meter1;
  TLabel *Label2Meter1;
  TMeterPanel *LeftMeterPanel2;
  TMeterPanel *RightMeterPanel2;
  TLabel *Label1Meter2;
  TLabel *Label2Meter2;
  TTimer *MeterReleaseTimer;
  void __fastcall FormResize(TObject *Sender);
  void __fastcall MeterReleaseTimerTimer(TObject *Sender);
private:    // User declarations
  TPicture *GetSmallSwitchPicture(unsigned char Color);
  TControl *FindFormControl(char *Name);
  void UpdateSwitch(unsigned char SwitchNr);
  float MeterData[4];
public:     // User declarations
  unsigned char SwitchState[54];
  unsigned char OnColor[54];
  unsigned char OffColor[54];
   __fastcall TAxumMeterForm(TComponent* Owner, char *url, form_node_info *node_info);
   __fastcall ~TAxumMeterForm();
  void MambaNetError(int code, char *msg);
  void MambaNetOnlineStatus(unsigned long addr, char valid);
  int MambaNetSetActuatorData(unsigned short object, union mbn_data data);
};
//---------------------------------------------------------------------------
extern PACKAGE TAxumMeterForm *AxumMeterForm;
//---------------------------------------------------------------------------
#endif
