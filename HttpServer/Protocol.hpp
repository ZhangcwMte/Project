#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <unistd.h>

#include"Util.hpp"

const std::string SEP = ": ";

struct HttpRequest
{
    std::string _request_line;
    std::vector<std::string> _request_header;
    std::string _blank;
    std::string _request_body;

    std::string _method;
    std::string _uri;
    std::string _version;

    std::unordered_map<std::string, std::string> _header_kv;
    size_t _content_length = 0;
};

class HttpResponse
{
    std::string _response_line;
    std::vector<std::string> _response_header;
    std::string _blank;
    std::string _response_body;
};

//读取请求，分析请求，构建响应，IO
class EndPoint
{
private:
    void recvHttpRequestLine()
    {
        auto& line = _http_request._request_line;
        Util::readLine(_sock, line);
        line.resize(line.size() - 1);
    }

    void recvHttpRequestHeader()
    {
        std::string line;
        while(1)
        {
            line.clear();
            Util::readLine(_sock, line);
            if(line == "\n")
            {
                _http_request._blank = line;
                break;
            }
            line.resize(line.size() - 1);
            _http_request._request_header.push_back(line);
        }
    }

    bool isNeedRecvHttpRequestBody()
    {
        auto& method = _http_request._method;
        if(method == "POST")
        {
            auto& header_kv = _http_request._header_kv;
            auto pos = header_kv.find("Content-Length");
            if(pos != header_kv.end())
            {
                _http_request._content_length = atoi(pos->second.c_str());
                return true;
            }
        }
        return false;
    }

    void recvHttpRequestBody()
    {
        if(isNeedRecvHttpRequestBody())
        {
            char ch;
            for(int i = 0; i < _http_request._content_length; i++)
            {
                ssize_t s = recv(_sock, &ch, 1, 0);
                if(s > 0)
                    _http_request._request_body += ch;
                else
                    break;
            }
        }
    }

    void parseHttpRequestLine()
    {
        auto& line = _http_request._request_line;
        std::stringstream ss(line);
        ss >> _http_request._method >> _http_request._uri >> _http_request._version;
    }

    void parseHttpRequestHeader()
    {
        std::string key;
        std::string value;
        for(auto& iter: _http_request._request_header)
        {
            if(Util::cutString(iter, key, value, SEP))
                _http_request._header_kv.insert({key, value});
        }
    }
public:
    EndPoint(int sock)
        :_sock(sock)
    {}

    void recvHttpRequest()
    {
        recvHttpRequestLine();
        recvHttpRequestHeader();
        parseHttpRequestLine();
        parseHttpRequestHeader();
        recvHttpRequestBody();
    }

    // void parseHttpRequest()
    // {
    //     //parseHttpRequestBody();
    // }

    void buildHttpResponse()
    {}

    void sendHttpResponse()
    {}

    ~EndPoint()
    {
        close(_sock);
    }
private:
    int _sock;
    HttpRequest _http_request;
    HttpResponse _http_reaponse;
};

class Entrance
{
public:
    static void* handlerRequest(void* _sock)
    {
        int sock = *(int*)_sock;
        delete (int*)_sock;

        //std::cout << "get a new link ..." << std::endl;

        EndPoint* ep = new EndPoint(sock);
        ep->recvHttpRequest();
        //ep->parseHttpRequest();
        ep->buildHttpResponse();
        ep->sendHttpResponse();
        delete ep;

        return nullptr;
    }
};