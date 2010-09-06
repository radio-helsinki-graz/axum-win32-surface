//---------------------------------------------------------------------------

#ifndef AxumMeterFrm3H
#define AxumMeterFrm3H
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
class TAxumMeterForm_3 : public TMambaNetForm
{
__published:    // IDE-managed Components
  TMeterPanel *LeftMeterPanel1;
  TMeterPanel *RightMeterPanel1;
  TATImage *AxumMeterBackgroundImage;
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
  void UpdateSwitch(unsigned char SwitchNr);
  float MeterData[4];
public:     // User declarations
  unsigned char SwitchState[54];
  unsigned char OnColor[54];
  unsigned char OffColor[54];
   __fastcall TAxumMeterForm_3(TComponent* Owner, char *url, form_node_info *node_info);
   __fastcall ~TAxumMeterForm_3();
  void MambaNetError(int code, char *msg);
  void MambaNetOnlineStatus(unsigned long addr, char valid);
  int MambaNetSetActuatorData(unsigned short object, union mbn_data data);
  void ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description);
  void StartCommunication();
};
//---------------------------------------------------------------------------
extern PACKAGE TAxumMeterForm_3 *AxumMeterForm_3;
//---------------------------------------------------------------------------
#endif
