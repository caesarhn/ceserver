#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include "operation.h"

int main(){
    char asu[] = "GET /localhost/ajg.png HTTP/1.1 Host: localhost";
    //char kenapa = process2(asu);
    process(asu);

    std::cout << "asu : " << asu << std::endl;
    //std::cout << "kenapa : " << kenapa << std::endl;
}