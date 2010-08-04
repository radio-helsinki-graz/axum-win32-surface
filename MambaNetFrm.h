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
#define MBN_VARARG
#include "mbn.h"
#include "axum_form.h"

//---------------------------------------------------------------------------
class TMambaNetForm : public TForm
{
__published:	// IDE-managed Components
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
public:		// User declarations
  TImage *BackgroundImage;
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
  void ResizeLabelFontToExtents(TLabel *DisplayLabel, int Percent);
};
//---------------------------------------------------------------------------
extern PACKAGE TMambaNetForm *MambaNetForm;
//---------------------------------------------------------------------------
#endif
