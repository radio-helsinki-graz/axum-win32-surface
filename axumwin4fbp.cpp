//---------------------------------------------------------------------------

#include <vcl.h>
#include "MambaNetConnectionFrm.h"

#pragma hdrstop
USERES("axumwin4fbp.res");
USEFORM("SurfaceFrm.cpp", SurfaceForm);
USEFORM("SurfaceSelectFrm.cpp", SurfaceSelectForm);
USEFORM("MambaNetConnectionFrm.cpp", MambaNetConnectionForm);
USEFORM("MambaNetFrm.cpp", MambaNetForm);
USELIB("libpq.lib");
USELIB("mbn.lib");
USEFORM("AxumSuperModuleFrm.cpp", AxumSuperModuleForm);
USEFORM("EQWindowDlg.cpp", EQWindowDialog);
USEFORM("Axum4FBPFrm_1.cpp", Axum4FBPForm_1);
USEFORM("Axum4FBPFrm_2.cpp", Axum4FBPForm_2);
USEFORM("AxumMeterFrm_2.cpp", AxumMeterForm_2);
USEFORM("AxumMeterFrm_3.cpp", AxumMeterForm_3);
USEFORM("AxumCRMFrm_1.cpp", AxumCRMForm_1);
USEFORM("AxumCRMFrm_2.cpp", AxumCRMForm_2);
USEFORM("AxumMeterFrm_4.cpp", AxumMeterForm_4);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->Title = "Axum Remote Control Surface";

       Application->CreateForm(__classid(TSurfaceForm), &SurfaceForm);
     Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
