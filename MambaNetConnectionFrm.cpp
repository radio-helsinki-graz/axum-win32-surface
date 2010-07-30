//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MambaNetConnectionFrm.h"
#include "PPMMeterFrm.h"
#include "mbn.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TMambaNetConnectionForm *MambaNetConnectionForm;
//---------------------------------------------------------------------------
__fastcall TMambaNetConnectionForm::TMambaNetConnectionForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMambaNetConnectionForm::AboutButtonClick(
      TObject *Sender)
{
  ShowMessage(mbnVersion());  
}
//---------------------------------------------------------------------------

