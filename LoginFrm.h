//---------------------------------------------------------------------------

#ifndef LoginFrmH
#define LoginFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TLoginForm : public TForm
{
__published:	// IDE-managed Components
  TEdit *UsernameEdit;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *PasswordEdit;
  TButton *LoginButtom;
  TButton *Button2;
private:	// User declarations
public:		// User declarations
  __fastcall TLoginForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLoginForm *LoginForm;
//---------------------------------------------------------------------------
#endif
