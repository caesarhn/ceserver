#include <cstring>
#include <queue>
#include <iostream>
#include "operation.h"

void process(char *value){
    static std::string kalimat(value);
    std::cout << kalimat << std::endl;
    int a = kalimat.find("HTTP/1.1");
    std::string kal = kalimat.substr(4,(a-4));

    std::cout << a << std::endl;
    std::cout << kal << std::endl;
    std::strcpy(value, kal.c_str());
}

char process2(char *value){
    static std::string kalimat(value);
    std::cout << kalimat << std::endl;
    int a = kalimat.find("HTTP/1.1");
    std::string kal = kalimat.substr(4,(a-4));

    std::cout << a << std::endl;
    std::cout << kal << std::endl;
    std::strcpy(value, kal.c_str());

    return *value;
}