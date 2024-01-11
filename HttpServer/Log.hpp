#pragma once

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <string>

// 日志是有日志级别的
#define DEBUG   0
#define NORMAL  1
#define WARNING 2
#define ERROR   3
#define FATAL   4

const char *gLevelMap[] = {
    "DEBUG",
    "NORMAL",
    "WARNING",
    "ERROR",
    "FATAL"
};

#define LOGFILE "./HttpServer.log"

void logMessage(int level, const char *format, ...)
{
    // va_list ap;
    // va_start(ap, format);
    // while()
    // int x = va_arg(ap, int);
    // va_end(ap); //ap=nullptr
    char stdBuffer[1024]; //标准部分
    time_t timestamp = time(nullptr);
    // struct tm *localtime = localtime(&timestamp);
    snprintf(stdBuffer, sizeof stdBuffer, "[%s] [%ld] ", gLevelMap[level], timestamp);

    char logBuffer[1024]; //自定义部分
    va_list args;
    va_start(args, format);
    // vprintf(format, args);
    vsnprintf(logBuffer, sizeof logBuffer, format, args);
    va_end(args);

    FILE *fp = fopen(LOGFILE, "a");
    printf("%s%s\n", stdBuffer, logBuffer);
    fprintf(fp, "%s%s\n", stdBuffer, logBuffer);
    fclose(fp);
}