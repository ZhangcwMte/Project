#pragma once
#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<cstdlib>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<unordered_map>

#include"log.hpp"

class UdpServer
{
public:
    UdpServer(uint16_t port , std::string ip = "")
        :m_ip(ip),m_port(port),m_sock(-1)
    {}

    void initServer()
    {
        m_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(m_sock < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno));
            exit(2);
        }
        struct sockaddr_in local;
        bzero(&local, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(m_port);
        local.sin_addr.s_addr = m_ip.empty() ? INADDR_ANY : inet_addr(m_ip.c_str());
        if(bind(m_sock, (struct sockaddr*)&local, sizeof local) < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno));
            exit(3);
        }
        logMessage(NORMAL, "init udp server done ... %s", strerror(errno));
    }

    void startServer()
    {
        char buffer[128];
        while(1)
        {
            struct sockaddr_in peer;
            bzero(&peer, 0);
            socklen_t len = sizeof peer;
            char key[64];
            ssize_t s = recvfrom(m_sock, buffer, sizeof buffer - 1, 0, (struct sockaddr*)&peer, &len);
            if(s > 0)
            {
                buffer[s] = 0;
                uint16_t cli_port = ntohs(peer.sin_port);
                std::string cli_ip = inet_ntoa(peer.sin_addr);
                //std::cout << "[" << cli_ip << ":" << cli_port << "]#" << buffer << std::endl;
                snprintf(key, sizeof key, "%s-%u", cli_ip.c_str(), cli_port);
                logMessage(NORMAL, "key: %s", key);

                if(m_users.find(key) == m_users.end())
                {
                    logMessage(NORMAL, "add new user : %s", key);
                    m_users.insert({key, peer});
                }
            }
            
            for(auto &iter: m_users)
            {
                std::string send_message = key;
                send_message += "#";
                send_message += buffer;
                logMessage(NORMAL, "push message to %s", iter.first.c_str());
                sendto(m_sock, send_message.c_str(), send_message.size(), 0, (struct sockaddr*)&iter.second, sizeof iter.second);
            }
            
        }
    }

    ~UdpServer()
    {
        if(m_sock >= 0)
            close(m_sock);
    }
private:
    uint16_t m_port;
    std::string m_ip;
    int m_sock;
    std::unordered_map<std::string, sockaddr_in> m_users;
};