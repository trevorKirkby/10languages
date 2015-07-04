#include <winsock2.h>
#include <stdio.h>
 
int main(int argc , char *argv[])
{
    WSADATA wsa;
     
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.");
 
    return 0;
}