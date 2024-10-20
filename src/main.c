// Nessecary Includes.
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

// Buffer 
#define Buffer 1500
#define NOLL 0
// A Pragma comment. Loading Library 
#pragma comment(lib, "ws2_32.lib")

void readFile(char* input, char* nameFile);
void getPath(char* input, char* output);

int main()
{
	// Variables and Declarations
	WSADATA wsa;
	struct sockaddr_in server;
	SOCKET s;
	SOCKET clientS;
	int iResult;

	//const char message[Buffer] = "This message being displayed to you in your Browser.\nIs sent from a C++ Winsock HTTP Server.\n Now Closing Connection.\r";
	char buffer[Buffer];
	char cbuffer[Buffer];

	snprintf(buffer, sizeof(buffer), "HTTP/1.1 200 OK\r\n");
	snprintf(buffer + strlen(buffer), sizeof(buffer), "Content-Type: text/html\r\n");

	char message[Buffer] = "<html><head><title>C++</title></head></html>";
	char* messages = message;
	readFile(messages, "page.html");
	snprintf(buffer + strlen(buffer), sizeof(buffer), "Content-Length: %zu\r\n", strlen(messages));
	snprintf(buffer + strlen(buffer), sizeof(buffer), "\r\n");
	snprintf(buffer + strlen(buffer), sizeof(buffer), messages);
	//printf(buffer);

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

	printf("Socket Created");

	server.sin_family = AF_INET; // Using AF_INET Address Family.
	server.sin_port = htons(80); // Defining PORT
	InetPton(AF_INET, _T("0.0.0.0"), &server.sin_addr); // Defining The Network Address to Run the Server on

	iResult = bind(s, (struct sockaddr*)&server, sizeof(server)); // binding the Host Address and Port Number
	if (iResult == SOCKET_ERROR) // If Bind gives Error
	{
		printf("Bind Error");
		getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}

	// Tell the User we Started Listening.
	printf("Listening on : localhost");
	iResult = listen(s, AF_INET); // Then Actually Start Listening for incoming Connections.

	/* 
	The Program will start to listen for incoming connections and will do so until 
	Someone Connects.
	*/

	while(true){
		clientS = accept(s, NOLL, NOLL); // Accept a Connection on a new Socket made for the Client.
		if (clientS == SOCKET_ERROR) { // if Accepting Connection is a Error
			printf("Accept Failed");
			getchar(); // Pause Console 
			exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
		}
		else {
			// notify user that someone connected

			recv(clientS, reply, sizeof(reply), NOLL); // Just in case if the Client sends something, We Receive it.
			getPath(reply, cbuffer);
			if(strcmp(cbuffer, "/data") == 0){
				char data[] = "<h1>ini adalah routing /data</h1>";
				send(clientS, data, strlen(data), NOLL);
			}else{
				ssize_t valread =  read(clientS, cbuffer, strlen(cbuffer));
				send(clientS, buffer, strlen(buffer), NOLL); // Send Client a Message
			}
		}
		printf("client: %s \n", cbuffer);
		closesocket(clientS); // close the Client Socket now that our Work is Complete.
	}

	WSACleanup(); // Clean Windows Socket API.

	getchar();
}

void readFile(char* input, char* nameFile) {
    FILE *file = fopen(nameFile, "r");  // Membuka file dalam mode baca
    
	if (file == NULL) {
        printf("Gagal membuka file.\n");
    }

    // Menghitung ukuran file
    fseek(file, 0, SEEK_END);  // Pindah ke akhir file
    long fileSize = ftell(file);  // Mendapatkan ukuran file
    rewind(file);  // Kembali ke awal file

    // Alokasi memori untuk buffer
    char *buffer = (char*)malloc((fileSize + 1) * sizeof(char));
    if (buffer == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        fclose(file);
    }

    // Membaca file ke buffer
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0';  // Menambahkan null-terminator

	memcpy(input, buffer, bytesRead);

    //printf("Isi file:\n%s\n", buffer);  // Menampilkan isi file

    // Membersihkan memori dan menutup file
    free(buffer);
    fclose(file);
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
	char buff[second];
	strncpy(buff, input + first, second);
	snprintf(buff + strlen(buff), sizeof(buff), "\0");
	strcpy(output, buff);
}