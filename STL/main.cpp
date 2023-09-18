#include"string.h"
using namespace Z;

int main()
{
    string s = "Hello World!";
    s.insert(0 , "No");
    s.erase(0 , 2);
    int n = s.find ('H');
    
    cout << s << std::endl;
    cout << s[6] << std::endl;
    std::cout << n << std::endl;

    return 0;
}
