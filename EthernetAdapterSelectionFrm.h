//---------------------------------------------------------------------------

#ifndef EthernetAdapterSelectionFrmH
#define EthernetAdapterSelectionFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TEthernetAdapterSelectionForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *EthernetGroupBox;
   TComboBox *AdapterComboBox;
   TButton *OpenButton;
   TGroupBox *TCPIPGroupBox;
   TEdit *URLEdit;
   TRadioButton *EthernetRadioButton;
   TRadioButton *TCPIPRadioButton;
  TCSpinEdit *CSpinEdit1;
  TButton *AboutButton;
   void __fastcall TCPIPRadioButtonClick(TObject *Sender);
   void __fastcall EthernetRadioButtonClick(TObject *Sender);
  void __fastcall AboutButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TEthernetAdapterSelectionForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEthernetAdapterSelectionForm *EthernetAdapterSelectionForm;
//---------------------------------------------------------------------------
#endif
