#include "lib/cesocket.h"

void testRoute(){
    char data[1000] = "{\"data\": [{\"nama\": \"Caesar\", \"membership\": \"Premium\"}, {\"nama\": \"Alvonso\", \"membership\": \"Silver\"}]}";
    responseJsonFunc(data);
    printf("kenapa\n");
}

void main (){
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
}