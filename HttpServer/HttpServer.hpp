#pragma once

#include"TcpServer.hpp"
#include"Protocol.hpp"

const uint16_t PORT = 8081;

class HttpServer
{
public:
    HttpServer(uint16_t port = PORT)
        :_port(port)
    {}

    void initServer()
    {
        _tcp_server = TcpServer::getInstance(_port);
    }

    void loop()
    {
        int listen_sock = -_tcp_server->getSock();
        while(!_stop)
        {
            std::string client_ip;
            uint16_t client_port;
            int sock = Sock::Accept(listen_sock, &client_ip, &client_port);
            int* _sock = new int(sock);
            pthread_t tid;
            pthread_create(&tid, nullptr, Entrance::handlerRequest, _sock);
        }
        
    }

    ~HttpServer()
    {}
private:
    uint16_t _port;
    TcpServer* _tcp_server;
    bool _stop;
};