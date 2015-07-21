#include <winsock2.h>
#include <stdio.h>
#include <math.h>

//-lwsock32

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

double *getlist(char *string, int index) {
    int listlen = getlistlen(string, index);
    double *output = (double*) malloc (listlen*sizeof(double));
    int outer = 0;
    int inner = 0;
    int layer = 0;
    int i;
    int decimals = 1;
    int decimal = 0;
    double value = 0.0;
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
                value /= decimals;
                output[inner] = value;
                value = 0;
                inner = 0;
            }
        }
        if (letter == ',' && layer == 2) {
            value /= decimals;
            output[inner] = value;
            value = 0;
            inner ++;
        }
        if (layer == 2 && outer == (index+1)) {
            if (letter != ',' && letter != '[' && letter != ']' && letter != '.') {
                value *= 10.0;
                value += (letter - '0');
                if (decimal) {
                    decimals ++;
                }
            }
            else if (letter == '.') {
                decimal = 1;
            }
        }
    }
    return output;
}

int dist(double x, double y) {
    return sqrt(x*x+y*y);
}

int in_circle(double *point, double *circledata) {
    if(circledata[2] >= dist(point[0]-circledata[0],point[1]-circledata[1])) {
        return 1;
    }
    else {
        return 0;
    }
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
    message = "\"circles\"\n";
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
    double outside = 0;
    double *circle1 = (double*) malloc (3*sizeof(double));
    circle1 = getlist(server_reply,0);
    double *circle2 = (double*) malloc (3*sizeof(double));
    circle2 = getlist(server_reply,1);
    double *circle3 = (double*) malloc (3*sizeof(double));
    circle3 = getlist(server_reply,2);
    double *circle4 = (double*) malloc (3*sizeof(double));
    circle4 = getlist(server_reply,3);
    double *circle5 = (double*) malloc (3*sizeof(double));
    circle5 = getlist(server_reply,4);
    double *circle6 = (double*) malloc (3*sizeof(double));
    circle6 = getlist(server_reply,5);
    double coordx;
    double coordy;
    for (coordx = 0; coordx < 1000; coordx++) {
        for (coordy = 0; coordy < 1000; coordy++) {
            double *coords = (double*) malloc (2*sizeof(double));
            double x = coordx;
            x /= 1000;
            double y = coordy;
            y /= 1000;
            coords[0] = x;
            coords[1] = y;
            if (in_circle(coords,circle1)) {continue;}
            if (in_circle(coords,circle2)) {continue;}
            if (in_circle(coords,circle3)) {continue;}
            if (in_circle(coords,circle4)) {continue;}
            if (in_circle(coords,circle5)) {continue;}
            if (in_circle(coords,circle6)) {continue;}
            outside ++;
        }
    }
    printf("OUTSIDE: %d\n",outside);
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