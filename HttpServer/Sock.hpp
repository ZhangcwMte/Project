#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <unistd.h>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

#include "Log.hpp"

class Sock
{
private:
    const static int gbacklog = 20;

public:
    Sock() {}
    static int Socket()
    {
        int listensock = socket(AF_INET, SOCK_STREAM, 0);
        if (listensock < 0)
        {
            logMessage(FATAL, "create socket error, %d:%s", errno, strerror(errno));
            exit(2);
        }
        logMessage(NORMAL, "create socket success, listensock: %d", listensock);
        return listensock;
    }
    
    static void Bind(int sock, uint16_t port, std::string ip = "0.0.0.0")
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &local.sin_addr);
        if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind error, %d:%s", errno, strerror(errno));
            exit(3);
        }
    }

    static void Listen(int sock)
    {
        if (listen(sock, gbacklog) < 0)
        {
            logMessage(FATAL, "listen error, %d:%s", errno, strerror(errno));
            exit(4);
        }

        logMessage(NORMAL, "init server success");
    }

    static int Accept(int listensock, std::string *ip, uint16_t *port)
    {
        struct sockaddr_in src;
        socklen_t len = sizeof(src);
        int servicesock = accept(listensock, (struct sockaddr *)&src, &len);
        if (servicesock < 0)
        {
            logMessage(ERROR, "accept error, %d:%s", errno, strerror(errno));
            return -1;
        }
        if(port) *port = ntohs(src.sin_port);
        if(ip) *ip = inet_ntoa(src.sin_addr);

        logMessage(NORMAL, "Get a new link");
        return servicesock;
    }
    static bool Connect(int sock, const std::string &server_ip, const uint16_t &server_port)
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(server_port);
        server.sin_addr.s_addr = inet_addr(server_ip.c_str());

        if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) return true;
        else return false;
    }
    ~Sock() {}
};