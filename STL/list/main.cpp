#include"list.h"
using namespace Z;

int main()
{
    list<int> lt;

    for(int i = 0; i < 10; i++)
    {
        lt.push_back(i);
    }

    for(auto i : lt)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}

