#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include"Sock.hpp"
#include"Log.hpp"

class TcpServer
{
private:
    TcpServer(uint16_t port)
        :_port(port), _listen_sock(-1)
    {}
    TcpServer(const TcpServer &s){}
public:
    static TcpServer* getInstance(uint16_t port)
    {
        static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        if(nullptr == svr)
        {
            pthread_mutex_lock(&lock);
            if(nullptr == svr)
            {
                svr = new TcpServer(port);
                svr->initServer();
            }
            pthread_mutex_unlock(&lock);

        }
        return svr;
    }

    void initServer()
    {
        _listen_sock = Sock::Socket();
        Sock::Bind(_listen_sock, _port);
        Sock::Listen(_listen_sock);
        logMessage(NORMAL, "tcp_server init ... success");
    }

    int getSock()
    {
        return _listen_sock;
    }

    ~TcpServer()
    {
        if(_listen_sock >= 0)close(_listen_sock);
    }
private:
    uint16_t _port;
    int _listen_sock;
    static TcpServer* svr;
};

TcpServer* TcpServer::svr = nullptr;