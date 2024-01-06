#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <algorithm>

#include"Util.hpp"

const std::string SEP = ": ";
const std::string WEB_ROOT = "wwwroot";
const std::string HOME_PAGE = "index.html";
const std::string PAGE_400 = "400.html";
const std::string PAGE_404 = "404.html";
const std::string PAGE_500 = "500.html";
const std::string HTTP_VERSION = "HTTP/1.0";
const std::string LINE_END = "\r\n";

const int OK = 200;
const int BAD_REQUEST = 400;
const int NOT_FOUND = 404;
const int SERVER_ERROR = 500;

static std::string Code2Desc(int code)
{
    std::string desc;
    switch(code)
    {
        case 200:
            desc = "OK";
            break;
        case 404:
            desc = "Not Found";
            break;
        default:
            break;
    }
    return desc;
}

static std::string Suffix2Desc(const std::string& suffix)
{
    static std::unordered_map<std::string, std::string> suffix2desc = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".jpg", "application/x-jpg"},
        {".xml", "application/xml"}
    };

    auto iter = suffix2desc.find(suffix);
    if(iter != suffix2desc.end())
    {
        return iter->second;
    }
    return "text/html";
}

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

    std::string _path;
    std::string suffix;
    std::string _query_string;
    size_t _size;
    bool _cgi;
};

struct HttpResponse
{
    std::string _status_line;
    std::vector<std::string> _response_header;
    std::string _blank = LINE_END;
    std::string _response_body;

    int _status_code = OK;
    int _fd = -1;
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
        auto& method = _http_request._method;
        std::transform(method.begin(), method.end(), method.begin(), ::toupper);
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

    int processCgi()
    {
        int code = OK;
        auto& method = _http_request._method;
        auto& query_string = _http_request._query_string;
        auto& body_text = _http_request._request_body;
        auto& bin = _http_request._path;
        int content_length = _http_request._content_length;
        auto& response_body = _http_reaponse._response_body;

        std::string query_string_env;
        std::string method_env;
        std::string content_length_env;

        int input[2];
        int output[2];

        if(pipe(input) < 0)
        {
            logMessage(ERROR, "pipe input error!");
            code = SERVER_ERROR;
            return code;
        }
        if(pipe(output) < 0)
        {
            logMessage(ERROR, "pipe output error!");
            code = SERVER_ERROR;
            return code;
        }

        pid_t pid = fork();
        if(pid == 0)
        {
            close(input[0]);
            close(output[1]);

            method_env = "METHOD=";
            method_env += method;
            putenv((char*)method_env.c_str());

            if(method == "GET")
            {
                query_string_env = "QUERY_STRING=";
                query_string_env += query_string;
                putenv((char*)query_string_env.c_str());
            }
            else if(method == "POST")
            {
                content_length_env = "CONTENT_LENGTH=";
                content_length_env += std::to_string(content_length);
                putenv((char*)content_length_env.c_str());
            }
            else
            {
                //nothing
            }

            dup2(input[1], 1);
            dup2(output[0], 0);

            execl(bin.c_str(), bin.c_str(), nullptr);
            exit(1);
        }
        else if(pid < 0)
        {
            logMessage(ERROR, "fork error!");
            return SERVER_ERROR;
        }
        else
        {
            close(input[1]);
            close(output[0]);

            if(method == "POST")
            {
                const char* start = body_text.c_str();
                size_t total = 0;
                size_t size = 0;
                while(total < content_length && (size = write(output[1], start + total, body_text.size() - total)) > 0)
                {
                        total += size;
                }
            }

            char ch = 0;
            while(read(input[0], &ch, 1))
            {
                response_body.push_back(ch);
            }

            int status = 0;
            int ret = waitpid(pid, &status, 0);
            if(ret == pid)
            {
                if(WIFEXITED(status))
                {
                    if(WEXITSTATUS(status) == 0)
                        code = OK;
                    else
                        code = BAD_REQUEST;
                }
                else
                    code = SERVER_ERROR;
            }

            close(input[0]);
            close(output[1]);
        }
        return code;
    }

    int processNonCgi()
    {
        _http_reaponse._fd = open(_http_request._path.c_str(), O_RDONLY);
        if(_http_reaponse._fd >= 0)
        {
        //状态行
        // _http_reaponse._status_line = HTTP_VERSION;
        // _http_reaponse._status_line += " ";
        // _http_reaponse._status_line += std::to_string(_http_reaponse._status_code);
        // _http_reaponse._status_line += " ";
        // _http_reaponse._status_line += Code2Desc(_http_reaponse._status_code);
        // _http_reaponse._status_line += LINE_END;
        // _http_reaponse._size = _http_reaponse._size;
        // //响应报头
        // std::string header_line = "Content-Length: ";
        // header_line += std::to_string(_http_reaponse._size);
        // header_line += LINE_END; 
        // _http_reaponse._response_header.push_back(header_line);

        // header_line = "Content-Type: ";
        // header_line += Suffix2Desc(_http_request.suffix);
        // header_line += LINE_END;
        // _http_reaponse._response_header.push_back(header_line);
        //内容


        return OK;
        }

        return NOT_FOUND;
    }

    void buildOkResponse()
    {
        std::string header_line = "Content-Length: ";
        if(_http_request._cgi)
            header_line += std::to_string(_http_reaponse._response_body.size());
        else
            header_line += std::to_string(_http_request._size);
        header_line += LINE_END; 
        _http_reaponse._response_header.push_back(header_line);

        header_line = "Content-Type: ";
        header_line += Suffix2Desc(_http_request.suffix);
        header_line += LINE_END;
        _http_reaponse._response_header.push_back(header_line);
    }

    void handlerError(const std::string& page)
    {
        _http_request._cgi = false;
        _http_reaponse._fd = open(page.c_str(), O_RDONLY);
        if(_http_reaponse._fd > 0)
        {
            struct stat st;
            stat(page.c_str(), &st);
            _http_request._size = st.st_size;
            std::string line = "Content-Type: text/html";
            line += LINE_END;
            _http_reaponse._response_header.push_back(line);

            line = "Content-Length ";
            line += std::to_string(st.st_size);
            line += LINE_END;
            _http_reaponse._response_header.push_back(line);
        }
    }

    void buildHttpResponseHelper()
    {
        auto& code = _http_reaponse._status_code;
        //构建状态行
        auto& status_line = _http_reaponse._status_line;
        status_line = HTTP_VERSION;
        status_line += " ";
        status_line += std::to_string(code);
        status_line += " ";
        status_line += Code2Desc(code);
        status_line += LINE_END;

        std::string root_path = WEB_ROOT;
        root_path += "/";
        //构建响应正文、报头
        switch(code)
        {
            case OK:
                buildOkResponse();
                break;
            case BAD_REQUEST:
                root_path += PAGE_400;
                handlerError(root_path);
                break;
            case NOT_FOUND:
                root_path += PAGE_404;
                handlerError(root_path);
                break;
            case SERVER_ERROR:
                root_path += PAGE_500;
                handlerError(root_path);
                break;
            default:
                break;
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
    {
        std::string path;
        struct stat st;
        size_t found = 0;
        size_t size = 0;
        auto& code = _http_reaponse._status_code;
        if(_http_request._method != "GET" && _http_request._method != "POST")
        {
            logMessage(WARNING, "method is not right");
            code = BAD_REQUEST;
            goto END;
        }
        if(_http_request._method == "GET")
        {
            ssize_t pos = _http_request._uri.find('?');
            if(pos != std::string::npos)
            {
                Util::cutString(_http_request._uri, _http_request._path, _http_request._query_string, "?");
                _http_request._cgi = true;
            }
            else
            {
                _http_request._path = _http_request._uri;
            }
        }
        else if(_http_request._method == "POST")
        {
            _http_request._cgi = true;
            _http_request._path = _http_request._uri;
        }
        else{
            //nothing
        }

        path = _http_request._path;
        _http_request._path = WEB_ROOT;
        _http_request._path += path;
        if(_http_request._path[_http_request._path.size() - 1] == '/')
        {
            _http_request._path += HOME_PAGE;
        }
        if(stat(_http_request._path.c_str(), &st) == 0)
        {
            if(S_ISDIR(st.st_mode))
            {
                _http_request._path += '/';
                _http_request._path += HOME_PAGE;
                stat(_http_request._path.c_str(), &st);
            }
            if((st.st_mode&S_IXUSR) || (st.st_mode&S_IXGRP) || (st.st_mode&S_IXOTH))
            {
                //可执行
                _http_request._cgi = true;
            }
            _http_request._size = st.st_size;
        }
        else
        {
            std::string info = _http_request._path;
            info += " Not Found";
            logMessage(WARNING, info.c_str());
            code = NOT_FOUND;
            goto END;
        }    

        found = _http_request._path.rfind(".");
        if(found == std::string::npos)
        {
            _http_request.suffix = ".html";
        }
        else
        {
            _http_request.suffix = _http_request._path.substr(found);
        }

        if(_http_request._cgi)
        {
            code = processCgi();
        }
        else
        {
            code = processNonCgi();
        }
END:
        buildHttpResponseHelper();
    }

    void sendHttpResponse()
    {
        send(_sock, _http_reaponse._status_line.c_str(), _http_reaponse._status_line.size(), 0);
        for(auto& iter: _http_reaponse._response_header)
        {
            send(_sock, iter.c_str(), iter.size(), 0);
        }
        send(_sock, _http_reaponse._blank.c_str(), _http_reaponse._blank.size(), 0);
        if(_http_request._cgi)
        {   auto& response_body = _http_reaponse._response_body;
            const char* start = response_body.c_str();
            size_t total = 0;
            size_t size = 0;
            while(total < response_body.size() && (size = send(_sock, start + total, response_body.size() - total, 0)) > 0)
            {
                total += size;
            }
        }
        else
        {
        sendfile(_sock, _http_reaponse._fd, nullptr, _http_request._size);
        close(_http_reaponse._fd);
        }
    }

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