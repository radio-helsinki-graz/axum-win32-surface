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
#include <Dialogs.hpp>
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
  char Name[32];
};

//---------------------------------------------------------------------------
class TSurfaceForm : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *SurfaceMainMenu;
  TMenuItem *FileMenuItem;
  TMenuItem *ConnecttoAXUMMenuItem;
  TMenuItem *Window1;
  TMenuItem *Reorder1;
  TMenuItem *AlwaysOnTopMenuItem;
  TStatusBar *StatusBar;
  TMenuItem *DisconnectMenuItem;
  TMenuItem *Help1;
  TMenuItem *AboutMenuItem;
  TMenuItem *N1;
  TMenuItem *GoToWebsiteMenuItem;
  TMenuItem *N2;
  TMenuItem *PrintAllMenuItem;
  TPrintDialog *PrintDialog;
  TPrinterSetupDialog *PrinterSetupDialog;
  TMenuItem *PrintSelectedMenuItem;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall ConnecttoAXUMMenuItemClick(TObject *Sender);
  void __fastcall Reorder1Click(TObject *Sender);
  void __fastcall AlwaysOnTopMenuItemClick(TObject *Sender);
  void __fastcall DisconnectMenuItemClick(TObject *Sender);
  void __fastcall AboutMenuItemClick(TObject *Sender);
  void __fastcall StatusBarClick(TObject *Sender);
  void __fastcall GoToWebsiteMenuItemClick(TObject *Sender);
  void __fastcall PrintAllMenuItemClick(TObject *Sender);
  void __fastcall FileMenuItemClick(TObject *Sender);
  void __fastcall PrintSelectedMenuItemClick(TObject *Sender);
private:	// User declarations
    surface_info *surfaces;
    char url[256];
    char port[16];
    char TCP;
    struct mbn_handler *mbn;
    struct mbn_node_info thisnode;
    void __fastcall ReorderSurfaceNodes();
    unsigned char StayOnTop;
    void WMStatusMessage(TMessage &msg);
    BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(WM_STATUS_MESSAGE,TMessage,WMStatusMessage);
    END_MESSAGE_MAP(TForm);
public:		// User declarations
   __fastcall TSurfaceForm(TComponent* Owner);
   surface_node SurfaceNodes[16];
    PGconn *sql_conn;
   void __fastcall SQL_exec(char *Query);
   bool __fastcall CopyConfiguration(unsigned short ToManID, unsigned short ToProductID, unsigned short ToID, unsigned int FromAddr, unsigned char FirmwareMajor);
   int CreateSurfaceNodeAndForm(int cntSurfaceNode, node_info *NodeInfo);
   void PrintHeader(TCanvas *Canvas, TMambaNetForm *MambaNetForm, float *xMm, float *yMm, float xPixelPerMm, float yPixelPerMm, float PageWidthMm, float PageHeightMm);
   int MambaNetNodeActive(unsigned short ManufacturerID, unsigned short ProductID, unsigned short UniqueIDPerProduct);
};
//---------------------------------------------------------------------------
extern PACKAGE TSurfaceForm *SurfaceForm;
//---------------------------------------------------------------------------
#endif
