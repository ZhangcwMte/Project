#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " port\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    sockaddr_in server;
    bzero(&server, 0);
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (connect(sock, (struct sockaddr *)&server, sizeof server) < 0)
    {
        std::cerr << "connect error" << std::endl;
        exit(3);
    }

    std::string message;
    while (1)
    {
        getline(std::cin, message);
        ssize_t s = send(sock, message.c_str(), message.size(), 0);
        if(s > 0)
        {
            char buffer[1024];
            ssize_t s = recv(sock, buffer, sizeof buffer, 0);
            if(s > 0)
            {
                buffer[s] = 0;
                std::cout << "server 回显# " << buffer << std::endl;
            }
        }
    }
    close(sock);

    return 0;
}