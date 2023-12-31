#include <memory>

#include"HttpServer.hpp"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        return 4;
    }

    std::shared_ptr<HttpServer> http_server(new HttpServer(atoi(argv[1])));
    http_server->initServer();
    http_server->loop();

    return 0;
}