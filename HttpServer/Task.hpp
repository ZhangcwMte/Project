#pragma once

#include <iostream>

#include"Protocol.hpp"

class Task
{
public:
    Task()
    {}
    Task(int sock)
        :_sock(sock)
    {}

    void processOn()
    {
        _handler(_sock);
    }

    ~Task()
    {}
private:
    int _sock;
    CallBack _handler;
};