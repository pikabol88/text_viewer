#include "readtext.h"


char* DLL_EXPORT ReadTextFromFile(PCHAR fileName, int *numOfSymbolsInText){
    char *text;
    FILE *file = fopen(fileName, "rb");
    if(file){
        fseek(file, 0 , SEEK_END);
        int numOfSymbols = ftell(file);
        rewind(file);
        text = (PCHAR)calloc(sizeof(char), numOfSymbols + 1);
        if(text){
            fread(text, sizeof(char), numOfSymbols, file);
            fclose(file);
            (*numOfSymbolsInText)=numOfSymbols;
             return text;
        } else return nullptr;
    }
    return nullptr;
}


extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
