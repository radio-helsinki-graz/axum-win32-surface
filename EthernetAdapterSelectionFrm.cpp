//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EthernetAdapterSelectionFrm.h"
#include "PPMMeterFrm.h"
#include "mbn.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TEthernetAdapterSelectionForm *EthernetAdapterSelectionForm;
//---------------------------------------------------------------------------
__fastcall TEthernetAdapterSelectionForm::TEthernetAdapterSelectionForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TEthernetAdapterSelectionForm::TCPIPRadioButtonClick(
      TObject *Sender)
{
   TCPIPGroupBox->Enabled = TCPIPRadioButton->Checked;
   EthernetGroupBox->Enabled = !TCPIPRadioButton->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TEthernetAdapterSelectionForm::EthernetRadioButtonClick(
      TObject *Sender)
{
   EthernetGroupBox->Enabled = EthernetRadioButton->Checked;
   TCPIPGroupBox->Enabled = !EthernetRadioButton->Checked;
}
//---------------------------------------------------------------------------



void __fastcall TEthernetAdapterSelectionForm::AboutButtonClick(
      TObject *Sender)
{
  ShowMessage(mbnVersion());  
}
//---------------------------------------------------------------------------

