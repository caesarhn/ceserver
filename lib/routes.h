global char route[Buffer];

void root(char * path){
    if(strcmp(route, path) == 0){
        char data[] = "{\"data\": [{\"nama\": \"Caesar\", \"membership\": \"Premium\"}, {\"nama\": \"Alvonso\", \"membership\": \"Silver\"}]}";
        char responses[strlen(responseJson) + strlen(data) + 501];
        snprintf(responses, sizeof(responses), responseJson);

        addContent(data, responses, sizeof(responses));
        printf("\n%s \n",responses);
        send(socketBuffer, responses, strlen(responses), NOLL);
    }
}