#include<iostream>
#include<string>
#include<fstream>
#include"../bundle.h"

int main(int argc , char* argv[])
{
    if(argc < 3)
    {
        std::cout << "Invalid command" << std::endl;
        return -1;
    }

    std::string filepath = argv[1];
    std::string packedname = argv[2];

    std::ifstream ifs;
    ifs.open(filepath , std::ios::binary);

    ifs.seekg(0 , std::ios::end);
    size_t size = ifs.tellg();
    ifs.seekg(0 , std::ios::beg);

    std::string body;
    body.resize(size);
    ifs.read(&body[0] , size);

    std::string packed = bundle::pack(bundle::LZIP , body);

    std::ofstream ofs;
    ofs.open(packedname , std::ios::binary);
    ofs.write(&packed[0] , packed.size());

    std::cout << "The file was extracted successfully" << std::endl;

    ifs.close();
    ofs.close();
}
