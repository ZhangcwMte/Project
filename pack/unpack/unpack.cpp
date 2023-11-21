#include<iostream>    
#include<fstream>    
#include<string>    
#include"bundle.h"    

int main(int argc,char* argv[])
{    
    if(argc < 3){
        std::cout << "Invalid command" << std::endl;
        return -1;    
    }    
    std::string filepath=argv[1];    
    std::string unpackname=argv[2];
    
    std::ifstream ifs;    
    ifs.open(filepath,std::ios::binary);    
    
    ifs.seekg(0,std::ios::end);    
    size_t size=ifs.tellg();    
    ifs.seekg(0,std::ios::beg);                                                                                                                                                               
    std::string body;    
    body.resize(size);     
    ifs.read(&body[0],size);    
                               
    std::string unpacked=bundle::unpack(body);    

    std::ofstream ofs;                            
    ofs.open(unpackname,std::ios::binary);                         
    ofs.write(&unpacked[0],unpacked.size());  

    std::cout << "The file was extracted successfully" << std::endl;

    ofs.close();                                  
    ifs.close();                                
}    
