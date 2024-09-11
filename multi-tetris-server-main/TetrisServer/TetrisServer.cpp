#include <iostream>
#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32")

#define PORT 33333
#define MAX_PACKET_SIZE 4096

typedef struct {
    int id;
    float value;
    char message[256];
} DataPacket;

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN sockaddr = {};
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sock, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
    listen(sock, SOMAXCONN);

    SOCKADDR_IN clientAddr = {};
    int clientSize = sizeof(clientAddr);
    SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &clientSize);

   // while (true) {
        char buf[MAX_PACKET_SIZE] = {};
        int recvBytes = recv(client, buf, MAX_PACKET_SIZE, 0);

       // if (recvBytes <= 0) {
            // 연결이 종료되었거나 오류 발생
       //     break;
        //}

        // 역직렬화: 수신한 데이터를 구조체로 복원
        DataPacket receivedPacket;
        memcpy(&receivedPacket, buf, sizeof(DataPacket));

        // 수신한 데이터 출력
        printf("Received ID: %d\n", receivedPacket.id);
        printf("Received Value: %.2f\n", receivedPacket.value);
        printf("Received Message: %s\n", receivedPacket.message);

        // 수신한 데이터를 그대로 다시 클라이언트로 전송
        send(client, buf, recvBytes, 0);
   // }

    closesocket(client);
    closesocket(sock);
    WSACleanup();
    return 0;
}
