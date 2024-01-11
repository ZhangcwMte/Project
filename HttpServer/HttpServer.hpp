#pragma once

#include <signal.h>

#include"TcpServer.hpp"
#include"Task.hpp"
#include"ThreadPool.hpp"

const uint16_t PORT = 8081;

class HttpServer
{
public:
    HttpServer(uint16_t port = PORT)
        :_port(port), _stop(false)
    {}

    void initServer()
    {
        // 忽略SIGPIPE， 否则服务器可能崩溃
        signal(SIGPIPE, SIG_IGN);
        //_tcp_server = TcpServer::getInstance(_port);
    }

    void loop()
    {
        while(!_stop)
        {
            logMessage(NORMAL, "Loop begin");
            std::string client_ip;
            uint16_t client_port;
            int sock = Sock::Accept(TcpServer::getInstance(_port)->getSock(), &client_ip, &client_port);
            Task task(sock);
            ThreadPool::getInstance()->pushTask(task);
        }
        
    }

    ~HttpServer()
    {}
private:
    uint16_t _port;
    bool _stop;
};