//---------------------------------------------------------------------------

#ifndef MambaNetConnectionFrmH
#define MambaNetConnectionFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TMambaNetConnectionForm : public TForm
{
__published:	// IDE-managed Components
   TButton *OpenButton;
   TGroupBox *TCPIPGroupBox;
   TEdit *URLEdit;
  TButton *AboutButton;
  TRadioButton *UDPRadioButton;
  TRadioButton *TCPRadioButton;
  void __fastcall AboutButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TMambaNetConnectionForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMambaNetConnectionForm *MambaNetConnectionForm;
//---------------------------------------------------------------------------
#endif
