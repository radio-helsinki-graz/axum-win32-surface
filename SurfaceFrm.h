//---------------------------------------------------------------------------

#ifndef SurfaceFrmH
#define SurfaceFrmH
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
#include <ComCtrls.hpp>
#define MBN_VARARG
#include "mbn.h"
#include "axum_form.h"
#include "MambaNetFrm.h"

struct node_info
{
  unsigned int addr;
  char name[32];
  unsigned short man_id;
  unsigned short prod_id;
  unsigned short id;
  unsigned char firm_major;
  node_info *next;
};

struct surface_info
{
  char name[32];
  node_info *nodes;
  surface_info *next;
};

struct surface_node {
  TMambaNetForm *MambaNetForm;
  unsigned char ConfigurationCopied;
  unsigned char FromAddr;
  unsigned int number_of_objects;
};

//---------------------------------------------------------------------------
class TSurfaceForm : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *SurfaceMainMenu;
  TMenuItem *File1;
  TMenuItem *ConnecttoAXUMMenuItem;
  TMenuItem *Window1;
  TMenuItem *Reorder1;
  TMenuItem *AlwaysOnTopMenuItem;
  TStatusBar *StatusBar;
  TMenuItem *DisconnectMenuItem;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall ConnecttoAXUMMenuItemClick(TObject *Sender);
  void __fastcall Reorder1Click(TObject *Sender);
  void __fastcall AlwaysOnTopMenuItemClick(TObject *Sender);
  void __fastcall DisconnectMenuItemClick(TObject *Sender);
private:	// User declarations
    surface_info *surfaces;
    char url[256];
    struct mbn_handler *mbn;
    struct mbn_node_info thisnode;
    void __fastcall ReorderSurfaceNodes();
    unsigned char StayOnTop;
public:		// User declarations
   __fastcall TSurfaceForm(TComponent* Owner);
   surface_node SurfaceNodes[16];
    PGconn *sql_conn;
   bool __fastcall CopyConfiguration(unsigned short ToManID, unsigned short ToProductID, unsigned short ToID, unsigned int FromAddr, unsigned char FirmwareMajor);
   int CreateSurfaceNodeAndForm(int cntSurfaceNode, node_info *NodeInfo);
};
//---------------------------------------------------------------------------
extern PACKAGE TSurfaceForm *SurfaceForm;
//---------------------------------------------------------------------------
#endif
