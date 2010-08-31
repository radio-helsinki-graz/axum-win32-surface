//---------------------------------------------------------------------------

#ifndef MambaNetFrmH
#define MambaNetFrmH
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
#include <Dialogs.hpp>
#define MBN_VARARG
#include "mbn.h"
#include "axum_form.h"
#include "ATImage.hpp"
#include <printers.hpp>

//---------------------------------------------------------------------------
class TMambaNetForm : public TForm
{
__published:	// IDE-managed Components
  TPrintDialog *PrintDialog1;
  void __fastcall FormActivate(TObject *Sender);
  void __fastcall FormCanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
  int ControlOriginalTop[1024];
  int ControlOriginalLeft[1024];
  int ControlOriginalHeight[1024];
  int ControlOriginalWidth[1024];
  void WMSetActuatorData(TMessage &msg);
  void WMConfigInfo(TMessage &msg);
  void WMResizeFonts(TMessage &msg);
  void WMOnlineStatus(TMessage &msg);
  void WMMambaNetError(TMessage &msg);
  BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_SET_ACTUATOR_DATA,TMessage,WMSetActuatorData);
  MESSAGE_HANDLER(WM_CONFIG_INFO,TMessage,WMConfigInfo);
  MESSAGE_HANDLER(WM_RESIZE_FONTS,TMessage,WMResizeFonts);
  MESSAGE_HANDLER(WM_ONLINE_STATUS,TMessage,WMOnlineStatus);
  MESSAGE_HANDLER(WM_MAMBANET_ERROR,TMessage,WMMambaNetError);
  END_MESSAGE_MAP(TForm);
public:		// User declarations
  TATImage *BackgroundATImage;
  struct mbn_node_info thisnode;
  struct mbn_handler *mbn;
  unsigned int MambaNetAddress;
  unsigned char Valid;
   __fastcall TMambaNetForm(TComponent* Owner);
   __fastcall ~TMambaNetForm();
  virtual void MambaNetError(int code, char *msg);
  virtual void MambaNetOnlineStatus(unsigned long addr, char valid);
  virtual int MambaNetSetActuatorData(unsigned short object, union mbn_data data);
  virtual void ConfigurationInformation(unsigned short object, char func_type, int func_seq, int func_nr, char *Label, char *Description);
  virtual void StartCommunication();
  virtual void CalculateFontSizes();
  int MaximalFontSizeToExtents(TCanvas *Canvas, AnsiString Text, int Width, int Height, int Percent);
  TControl *FindFormControl(char *Name);
  virtual void PrintLabels(TCanvas *Canvas, float *xMm, float *yMm, float xPixelPerMm, float yPixelPerMm, float PageWidthMm, float PageHeightMm);
  virtual bool PrintLabelsAvailable();
};
//---------------------------------------------------------------------------
extern PACKAGE TMambaNetForm *MambaNetForm;
//---------------------------------------------------------------------------
#endif
