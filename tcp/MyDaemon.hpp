#pragma once
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/stat.h>
#include<fcntl.h>

void MyDaemon()
{
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    if(fork() > 0)exit(0);

    setsid();

    int devnull = open("/dev/null", O_RDONLY | O_WRONLY);
    if(devnull > 0)
    {
        dup2(0, devnull);
        dup2(1, devnull);
        dup2(2, devnull);

        close(devnull);
    }
}