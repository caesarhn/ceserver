#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <vector>
#include "cesocket.h"
#include "operation.h"

#define TRUE 1
#define FALSE 0
#define PORT 80

//int addrlen = sizeof(address);
//const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nContent-Length: 3000\n\n<!DOCTYPE html>\n<html len='en'>\n<head>\n<link rel='icon' href='localhost/ajg.png'>\n<title>asu</title>\n</head>\n<body>\n<h1>Sammy's Sample HTML</h1>\n<p>This code is an example of how HTML is written.</p><p>It uses HTML tags to structure the text.</p><p>It uses HTML to add a <a href='digitalocean.com/community'>link</a>.</p><p>And it also uses HTML to add an image:</p><img src='https://html.sammy-codes.com/images/small-profile.jpeg'/>\n</body>\n</html>";
cesocket::cesocket(){
    multi = 0;
    if(createCesocket() && bindCesocket() == true){
        std::cout << "Success create and bind socket"<< std::endl;
    }
}

cesocket::cesocket(std::string value){
    multi = 1;
    initCesocket();
    if(createMasterCesocket() && bindMasterCesocket() == true){
        std::cout << "Success create and bind master socket"<< std::endl;
    }
}

bool cesocket::createCesocket(){
	if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0){
        perror("In socket");
        exit(EXIT_FAILURE);
        return false;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    return true;
}

bool cesocket::createMasterCesocket(){
    if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0){
        perror("setcocpot");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    return true;
}

bool cesocket::bindCesocket(){
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0){
        perror("In listen");
        exit(EXIT_FAILURE);
        return false;
    }
    else{
    	return true;
    }
}
bool cesocket::bindMasterCesocket(){
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(master_socket, 3) < 0){
        perror("In listen");
        exit(EXIT_FAILURE);
        return false;
    }
    else{
    	return true;
    }
}

void cesocket::initCesocket(){
    for(int_iteration=0; int_iteration < max_clients; int_iteration++){
        client_socket[int_iteration] = 0;
    }
}

bool cesocket::runCesocket(){
    if(multi == 0){
        while(1){
            printf("\n+++++++ Waiting for new connection ++++++++\n\n");
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
                perror("In accept");
                exit(EXIT_FAILURE);
            }
        
            char buffer[30000] = {0};
            valread = read( new_socket , buffer, 30000);
            printf("%s\n",buffer );
            std::cout <<"nilai read : " << valread << std::endl;
            std::cout <<"size buffer : " << sizeof(buffer)/sizeof(char) << std::endl;
            //process(buffer);
            write(new_socket , hello , strlen(hello));
            printf("------------------Hello message sent-------------------");
            close(new_socket);
        }
    }
    else if(multi == 1){
        while(1){
            FD_ZERO(&readfds);
            FD_SET(master_socket, &readfds);
            max_sd = master_socket;
            //std::cout << "yang pertama" << std::endl;

            for(int_iteration=0; int_iteration < max_clients; int_iteration++){
                sd = client_socket[int_iteration];
                if(sd>0){
                    FD_SET(sd, &readfds);
                }
                if(sd>max_sd){
                    max_sd = sd;
                }
            }
            activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
            if((activity < 0)&&(errno=EINTR)){
                printf("select error");
            }
            if(FD_ISSET(master_socket, &readfds)){
                if((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                //printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));    
                if(send(new_socket, hello, strlen(hello), 0) != strlen(hello)){
                    perror("send");
                }
                for(int_iteration=0; int_iteration<max_clients; int_iteration++){
                    if(client_socket[int_iteration] == 0){
                        client_socket[int_iteration] = new_socket;
                        printf("adding to list of sockets as %d\n", int_iteration);
                        break;
                    }
                }
            }
            for(int_iteration=0; int_iteration<max_clients; int_iteration++){
                //std::cout << "loop paling bawah" << std::endl;
                sd = client_socket[int_iteration];
                if(FD_ISSET(sd, &readfds)){
                    if((valread = read(sd, buffer, 1024))==0){
                        getpeername(sd, (struct sockaddr*)&address,(socklen_t*)&addrlen);
                        close(sd);
                        client_socket[int_iteration]=0;
                    }
                    else{
                        std::cout << "buffer[valread] : "<< buffer[valread] << std::endl;
                        buffer[valread] = '\0';
                        printf("%s\n",buffer );
                        std::cout << "buffer count : " << strlen(buffer) << std::endl;
                        std::cout << "buffer[valread] : "<< buffer[valread] << std::endl;
                        //process2(buffer);
                        //send(sd, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }
    return true;
}


