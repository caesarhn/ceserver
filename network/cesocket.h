#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/time.h>

#define TRUE 1
#define FALSE 0

class cesocket{
public:
    cesocket();
    cesocket(std::string value);
    int server_fd, new_socket; 
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nContent-Length: 1000\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>asu</title>\n</head>\n<body>\n<h1>Sammy's Sample HTML</h1>\n<p>This code is an example of how HTML is written.</p><p>It uses HTML tags to structure the text.</p><p>It uses HTML to add a <a href='digitalocean.com/community'>link</a>.</p><p>And it also uses HTML to add an image:</p><img src='https://html.sammy-codes.com/images/small-profile.jpeg'/>\n</body>\n</html>";

    bool runCesocket();

private:
    int multi = 0;
    int int_iteration;
    int opt = TRUE;
    int master_socket, client_socket[30], max_clients = 30, activity;
    int sd;
    int max_sd;
    char buffer[1025];
    fd_set readfds;

    void initCesocket();
    bool createCesocket();
    bool createMasterCesocket();
    bool bindCesocket();
    bool bindMasterCesocket();

};
