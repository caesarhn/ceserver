#include "lib/cesocket.h"
#include <stdio.h>
#include <stdlib.h>

extern int runKernel();

void testRoute(){
    char data[1000] = "{\"data\": [{\"nama\": \"Caesar\", \"membership\": \"Premium\"}, {\"nama\": \"Alvonso\", \"membership\": \"Silver\"}]}";
    responseJsonFunc(data);
}

int main (){
    printf("nilai-kz: %zu \n", runKernel());
    // // Load the DLL
    // HINSTANCE hDll = LoadLibrary("cuda_kernel.dll");
    // if (!hDll) {
    //     printf("Could not load DLL!\n");
    // }

    // // Get the address of the function
    // KernelFunc kernel = (KernelFunc)GetProcAddress(hDll, "runKernel");
    // if (!kernel) {
    //     printf("Could not find the function!\n");
    //     FreeLibrary(hDll);
    // }

    // kernel();

    initCesocket();
    addDefaultHeader();
    addRoute("/test", testRoute);
    addRoute("/kenapa", testRoute);
    checkRoute();

    while(true){
        if(breakSignal == 1){
            break;
        }
        startListen();
        //stopListen();
    }

    return 0;
}