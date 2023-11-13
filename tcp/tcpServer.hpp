#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<memory>
#include "log.hpp"
#include"ThreadPool/ThreadPool.hpp"
#include"ThreadPool/Task.hpp"

static void service(int sock, const std::string& client_ip, const uint16_t& client_port, const std::string& thread_name)
{
    char buffer[1024];
    while(1)
    {
        ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
        if(s > 0)
        {
            buffer[s] = 0;
            std::cout << client_ip << ":" << client_port << "# " << buffer << std::endl;
        }
        else if(s == 0)
        {
            logMessage(NORMAL, "%s:%d shutdown, me too!", client_ip.c_str(), client_port);
            break;
        }
        else
        {
            logMessage(ERROR, "read socket error, %d:%s", errno, strerror(errno));
            break;
        }
        write(sock, buffer, strlen(buffer));
    }
    close(sock);
}

class TcpServer
{
    const static int gbacklog = 20;
public:
    TcpServer(uint16_t port, std::string ip = "")
        :m_port(port), m_ip(ip), m_listen_sock(-1), m_thread_pool_ptr(ThreadPool<Task>::getThreadPool())
    {}

    void initServer()
    {
        m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(m_listen_sock < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno));
            exit(2);
        }
        logMessage(NORMAL, "create socket success, m_listen_sock: %d", m_listen_sock);
        sockaddr_in local;
        bzero(&local, 0);
        local.sin_addr.s_addr = m_ip.empty() ? INADDR_ANY : inet_addr(m_ip.c_str());
        local.sin_family = AF_INET;
        local.sin_port = htons(m_port);
        if(bind(m_listen_sock, (struct sockaddr*)&local, sizeof local) < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno));
            exit(3);
        }

        if(listen(m_listen_sock, gbacklog) < 0)
        {
            logMessage(FATAL, "listen error, %d:%s", errno, strerror(errno));
            exit(4);
        }

        logMessage(NORMAL, "init server success");
    }

    void startServer()
    {
        // signal(SIGCHLD, SIG_IGN);
        m_thread_pool_ptr->run();
        while(1)
        {
            sockaddr_in src;
            socklen_t len = sizeof src;
            int service_sock = accept(m_listen_sock, (struct sockaddr*)&src, &len);
            if(service_sock < 0)
            {
                logMessage(ERROR, "accept error, %d:%s", errno, strerror(errno));
                continue;
            }

            uint16_t client_port = ntohs(src.sin_port);
            std::string client_ip = inet_ntoa(src.sin_addr);
            logMessage(NORMAL, "link success, servicesock: %d | %s : %d |\n", service_sock, client_ip.c_str(), client_port);

            //多进程
            // pid_t id = fork();
            // assert(id != -1);
            // if(id == 0)
            // {
            //     close(m_listen_sock);
            //     service(service_sock, client_ip, client_port);
            //     exit(0);
            // }
            // close(service_sock);

            //线程池
            Task t(service_sock, client_ip, client_port, service);
            m_thread_pool_ptr->pushTask(t);
        }
    }

    ~TcpServer()
    {}
private:
    std::string m_ip;
    uint16_t m_port;
    int m_listen_sock;
    std::unique_ptr<ThreadPool<Task>> m_thread_pool_ptr;
};