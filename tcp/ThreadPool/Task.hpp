#pragma once
#include<iostream>
#include<functional>
#include<string>

typedef std::function<void(int, const std::string&, const uint16_t&, const std::string&)> func_t;

class Task
{
public:
    Task()
    {}

    Task(int sock, std::string ip, uint16_t port, func_t func)
        :m_sock(sock)
        ,m_ip(ip)
        ,m_port(port)
        ,m_func(func)
    {}

    void operator()(const std::string& name)
    {
        m_func(m_sock, m_ip, m_port, name);
    }

    ~Task()
    {}
private:
    int m_sock;
    std::string m_ip;
    uint16_t m_port;
    func_t m_func;
};