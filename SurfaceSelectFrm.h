//---------------------------------------------------------------------------

#ifndef SurfaceSelectFrmH
#define SurfaceSelectFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TSurfaceSelectForm : public TForm
{
__published:	// IDE-managed Components
  TButton *Button1;
  TButton *Button2;
  TTreeView *TreeView1;
private:	// User declarations
  TForm *SurfaceNodes[16];
public:		// User declarations
  __fastcall TSurfaceSelectForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSurfaceSelectForm *SurfaceSelectForm;
//---------------------------------------------------------------------------
#endif
