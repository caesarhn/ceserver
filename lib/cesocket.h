#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
//#include "json.h"

// Buffer 
#define Buffer 1500
#define NOLL 0
#define MAX_CLIENTS 100
// A Pragma comment. Loading Library 
#pragma comment(lib, "ws2_32.lib")

struct routes {
	char route[500];
	void (*function)();
	struct routes* next;
};

WSADATA wsa;
struct sockaddr_in server, client;
SOCKET s, socketBuffer;
SOCKET clientS, client_socket[MAX_CLIENTS];
SOCKET maxSock;
fd_set readfds;
int activity, clientLen;
int breakSignal = 0;
u_long mode = 1;
struct routes* Routing;

int iResult;
char buffer[Buffer];
char route[Buffer];
char responseJson[Buffer];
char responseHtml[Buffer];
char reply[Buffer] = { 0 };

void initCesocket();
void startListen();
void stopListen();
void addDefaultHeader();
void readFile(char* input, char* nameFile);
void getPath(char* input, char* output);
void addContent(char* content, char* buffer, size_t size);
void addRoute(char* route, void(*handleRoute)());
void handleRootPath();
void responseJsonFunc(char* data);
void checkRoute();

void initCesocket(){
    char reply[Buffer] = { 0 };

	// Simple. Start WSA(Windows Sockets API). If the return answer is not 0. It means error so therefore, 
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("WinsocK Failed"); // Print out this on the Screen.
		getchar(); // Pause Console
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}

	// Notifiy user that, WSA (Windows Socket API) has started.
	printf("Windows Socket API Started");
	// Create a Network Socket
	s = socket(AF_INET, SOCK_STREAM, NOLL);
	// If the Socket is Invalid or a Socket Error Occurs
	if (s == SOCKET_ERROR) 
	{
		printf("Socket Error");
		getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}
	else if (s == INVALID_SOCKET) {
		printf("socket Error");
		getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}

	if(ioctlsocket(s, FIONBIO, &mode) != 0){
		printf("error");
	}
	printf("Socket Created");

	server.sin_family = AF_INET; // Using AF_INET Address Family.
	server.sin_port = htons(80); // Defining PORT
	server.sin_addr.s_addr = INADDR_ANY;
	//InetPton(AF_INET, _T("0.0.0.0"), &server.sin_addr); // Defining The Network Address to Run the Server on

	iResult = bind(s, (struct sockaddr*)&server, sizeof(server)); // binding the Host Address and Port Number
	if (iResult == SOCKET_ERROR) // If Bind gives Error
	{
		printf("Bind Error");
		getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}

	// Tell the User we Started Listening.
	printf("Listening on : localhost\n");
	iResult = listen(s, SOMAXCONN); // Then Actually Start Listening for incoming Connections.
	clientLen = sizeof(struct sockaddr_in);
	maxSock = s;

	for(int i = 0; i < MAX_CLIENTS; i++){
		client_socket[i] = 0;
	}

	//Add list of route
	Routing = (struct routes*)malloc(sizeof(struct routes));
	Routing->next = NULL;
	printf("define routings area ...");
	strcpy(Routing->route, "/");
	Routing->function = handleRootPath;

	struct routes* n = (struct routes*)malloc(sizeof(struct routes));
	n->next = NULL;
	strcpy(n->route, "/ss");
	n->function = handleRootPath;
	Routing->next = n;
}

void startListen(){
	printf("\nLOOP\n");
	FD_ZERO(&readfds);
	FD_SET(s, &readfds);

	//enter client socket to descriptor set
	for(int i=0; i < MAX_CLIENTS; i++){
		SOCKET socket = client_socket[i];
		if(socket != 0){
			FD_SET(socket, &readfds);
		}
		if(socket > maxSock){
			maxSock = socket;
		}
	}

	//check activity on socket
	activity = select(maxSock + 1, &readfds, NULL, NULL, NULL);
	if (activity < 0 && WSAGetLastError() != WSAEINTR) {
		printf("error: select error, return: %zu\n", activity);
		breakSignal = 1;
	}

	if(FD_ISSET(s, &readfds)){
		clientS = accept(s, (struct sockaddr*)&client, &clientLen); // Accept a Connection on a new Socket made for the Client.
		if (clientS != INVALID_SOCKET) {
			FD_ZERO(&readfds);
			for(int i=0; i < MAX_CLIENTS; i++){
				if(client_socket[i] == 0){
					client_socket[i] = clientS;
					break;
				}
			}
		}
	}

	for(int i=0; i <= MAX_CLIENTS; i++){
		socketBuffer = client_socket[i];
		if(FD_ISSET(socketBuffer, &readfds)){
			//FD_SET(i, &readfds);
			int recv_size = recv(socketBuffer, reply, sizeof(reply), NOLL);
			getPath(reply, route);
			if (recv_size == SOCKET_ERROR || recv_size == 0) {
				closesocket(socketBuffer);
				FD_CLR(socketBuffer, &readfds);
				client_socket[i] = 0;
			}
			else {
				//notify user that someone connected
				struct routes* r = Routing;
				while(true){
					if(strcmp(route, r->route) == 0){
						r->function();
						FD_CLR(socketBuffer, &readfds);
						break;
					}else{
						if(r->next != NULL){
							r = r->next;
						}
						else{
							send(socketBuffer, buffer, strlen(buffer), NOLL);
							FD_CLR(socketBuffer, &readfds);
							break;
						}
					}
				}
			}
			snprintf(reply, sizeof(reply), "");
			snprintf(route, sizeof(route), "");
		}
	}
}

void stopListen(){
    closesocket(clientS);
	FD_CLR(clientS, &readfds);
}

void addDefaultHeader(){
    snprintf(buffer, sizeof(buffer), "HTTP/1.1 200 OK\r\n");
	snprintf(buffer + strlen(buffer), sizeof(buffer), "Content-Type: text/html\r\n");

	char message[Buffer] = "<html><head><title>C++</title></head></html>";
	char* messages = message;
	snprintf(buffer + strlen(buffer), sizeof(buffer), "Content-Length: %zu\r\n", strlen(messages));
	snprintf(buffer + strlen(buffer), sizeof(buffer), "\r\n");
	snprintf(buffer + strlen(buffer), sizeof(buffer), messages);

    snprintf(responseJson, sizeof(responseJson), "HTTP/1.1 200 OK\r\n");
	snprintf(responseJson + strlen(responseJson), sizeof(responseJson), "Content-Type: application/json\r\n");
}

void addContent(char* content, char* response, size_t size){
    snprintf(response + strlen(response), size, "Content-Length: %zu\r\n", strlen(content));
	snprintf(response + strlen(response), size, "\r\n");
	snprintf(response + strlen(response), size, content);
}

void getPath(char* input, char* output){
	int first = 0;
	int second = 0;
	for(int i=0; i<strlen(input); i++){
		if(input[i] == ' ' && first == 0){
			first = i + 1;
		}
		else if(input[i] == ' ' && first > 0){
			second = (i - first);
			break;
		}
	}
	strncpy(output, input + first, second);
}

void addRoute(char* route, void(*handleRoute)()){
	struct routes* newRoute = (struct routes*)malloc(sizeof(struct routes));
	newRoute->function = handleRoute;
	newRoute->next = NULL;
	strcpy(newRoute->route, route);
	struct routes* last = Routing;
	while (last->next != NULL){
		last = last->next;
	}
	last->next = newRoute;
}

void handleRootPath(){
    char data[] = "{\"data\": [{\"nama\": \"Caesar\", \"membership\": \"Premium\"}, {\"nama\": \"Alvonso\", \"membership\": \"Silver\"}]}";
	char responses[strlen(responseJson) + strlen(data) + 501];
	snprintf(responses, sizeof(responses), responseJson);
	addContent(data, responses, sizeof(responses));
	send(socketBuffer, responses, strlen(responses), NOLL);
}

void responseJsonFunc(char* data){
	char responses[strlen(responseJson) + strlen(data) + 501];
	snprintf(responses, sizeof(responses), responseJson);
	addContent(data, responses, sizeof(responses));
	printf("\n data: \n %s", responses);
	send(socketBuffer, responses, strlen(responses), NOLL);
}

void checkRoute(){
	struct routes* last = Routing;
	while(true){
		printf("data route: %s\n", last->route);
		if(last->next == NULL){
			break;
		}
		last = last->next;
	}
}