//---------------------------------------------------------------------------

#include <vcl.h>
#include "MambaNetConnectionFrm.h"

#pragma hdrstop
USERES("axumwin4fbp.res");
USEFORM("SurfaceFrm.cpp", SurfaceForm);
USEFORM("SurfaceSelectFrm.cpp", SurfaceSelectForm);
USEFORM("Axum4FBPFrm.cpp", Axum4FBPForm);
USEFORM("AxumCRMFrm.cpp", AxumCRMForm);
USEFORM("AxumMeterFrm.cpp", AxumMeterForm);
USEFORM("MambaNetConnectionFrm.cpp", MambaNetConnectionForm);
USEFORM("MambaNetFrm.cpp", MambaNetForm);
USELIB("libpq.lib");
USELIB("mbn.lib");
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
