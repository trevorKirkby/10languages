#include <winsock2.h>
#include <stdio.h>
#include <math.h>

int getitem(char *string, int index1, int index2) {
    int outer = 0;
    int inner = 0;
    int layer = 0;
    int i;
    int value = 0;
    for (i = 0; i < strlen(string); i++) {
        char letter = string[i];
        if (letter == '[') {
            if (layer == 1) {
                outer ++;
            }
            layer ++;
        }
        if (letter == ']') {
            layer --;
            if (layer == 2) {
                inner = 0;
            }
        }
        if (letter == ',' && layer == 2) {
            inner ++;
        }
        if (layer == 2 && outer == (index1+1) && inner == (index2)) {
            if (letter != ',' && letter != '[' && letter != ']') {
                value *= 10;
                value += (letter - '0');
            }
        }
    }
    return value;
}

int getlistlen(char *string, int index) {
    int outer = 0;
    int inner = 0;
    int layer = 0;
    int i;
    for (i = 0; i < strlen(string); i++) {
        char letter = string[i];
        if (letter == '[') {
            if (layer == 1) {
                outer ++;
            }
            layer ++;
        }
        if (letter == ']') {
            //printf("%d %d\n",layer,outer);
            if (layer == 2) {
                if (outer == (index+1)) {
                    return (inner+1);
                }
                inner = 0;
            }
            layer --;
        }
        if (letter == ',' && layer == 2) {
            inner ++;
            //printf("%d\n",inner);
        }
    }
    return 0;
}

int *getlist(char *string, int index) {
    int listlen = getlistlen(string, index);
    int *output = (int*) malloc (listlen*sizeof(int));
    int i;
    for(i = 0; i < listlen; i++) {
        output[(i)] = getitem(string, index, (i+1));
    }
    return output;
}

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[4000];
    int recv_size;
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 1234 );
    //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
    puts("Connected");
    //Send some data
    message = "\"histogram\"\n";
    if( send(s , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");
    //Receive a reply from the server
    if((recv_size = recv(s , server_reply , 4000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
    puts("Reply received\n");
    //Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);
    //Processing histogram from server_reply to define message as the solution.
    int answerlen = getitem(server_reply,0,0);
    //int answer[answerlen];
    int *answer = (int*) malloc (answerlen*sizeof(int));
    int increment = getitem(server_reply,0,1);
    int listlen = getlistlen(server_reply,1);
    printf("ANSWER LEN: %d, INCREMENT: %d, LISTLEN: %d\n",answerlen,increment,listlen);
    int *data = getlist(server_reply,1);
    int i;
    printf("\n");
    for (i = 0; i < answerlen; i++) {
        answer[i] = 0;
    }
    for (i = 0; i < getlistlen(server_reply,1); i++) {
        int index = data[i]/increment;
        answer[index] += 1;
    }
    int total = 0;
    for (i = 0; i < answerlen; i++) {
        total += answer[i];
    }
    printf("TOTAL: %d\n",total);
    printf("\n");
    message = "[";
    if( send(s , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    for (i = 0; i < answerlen; i++) {
        char message[10];
        if (i != 0)
        {
            sprintf(message,",%d",answer[i]);
        }
        else
        {
            sprintf(message,"%d",answer[i]);
        }
        if( send(s , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
    }
    //Send some data
    message = "]\n";
    if( send(s , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");
    //Receive a reply from the server
    if((recv_size = recv(s , server_reply , 4000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
    puts("Reply received\n");
    //Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);
    closesocket(s);
	WSACleanup();
    return 0;
}