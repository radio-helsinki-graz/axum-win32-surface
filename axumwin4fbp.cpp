//---------------------------------------------------------------------------

#include <vcl.h>
#include "MambaNetConnectionFrm.h"

#pragma hdrstop
USERES("axumwin4fbp.res");
USEFORM("SurfaceFrm.cpp", SurfaceForm);
USEFORM("MambaNetConnectionFrm.cpp", MambaNetConnectionForm);
USELIB("libpq.lib");
USELIB("mbn.lib");
USEFORM("SurfaceSelectFrm.cpp", SurfaceSelectForm);
USEFORM("MambaNetFrm.cpp", MambaNetForm);
USEFORM("Axum4FBPFrm.cpp", Axum4FBPForm);
USEFORM("AxumCRMFrm.cpp", AxumCRMForm);
USEFORM("AxumMeterFrm.cpp", AxumMeterForm);
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
