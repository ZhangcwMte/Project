#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory>

#include"log.hpp"
#include"Thread.hpp"

uint16_t server_port = 0;
std::string server_ip;

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " serverIp serverPort\n"
              << std::endl;
}

static void* udpSend(void* args)
{
    int sock = *(int*)((ThreadData*)args)->m_args;    
    std::string message;
    if(sock < 0)
    {
        logMessage(FATAL, "%d:%s", errno, strerror(errno));
        exit(2);
    }
    struct sockaddr_in server;
    bzero(&server, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_port);  

     while(1)
    {
        std::cout << "what you say" << std::endl;
        getline(std::cin, message);
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof server);
    }
}

static void* udpRecv(void* args)
{
    int sock = *(int*)((ThreadData*)args)->m_args;
    char buffer[128];
     while(1)
    {
        struct sockaddr_in temp;
        bzero(&temp, 0);
        socklen_t len = sizeof temp;
        ssize_t s = recvfrom(sock, buffer, sizeof buffer - 1, 0, (struct sockaddr*)&temp, &len);
        if(s > 0)
        {
            buffer[s] = 0;
            std::cout << buffer << std::endl;
        }
    }
}

int main(int argc , char* argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    std::string message;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        logMessage(FATAL, "%d:%s", errno, strerror(errno));
        exit(2);
    }
    server_port = atoi(argv[2]);
    server_ip = argv[1];

    std::unique_ptr<Thread> sender(new Thread(1, udpSend, (void *)&sock));
    std::unique_ptr<Thread> recver(new Thread(2, udpRecv, (void *)&sock));

    sender->start();
    recver->start();

    sender->join();
    recver->join();

    close(sock);

    return 0;
}