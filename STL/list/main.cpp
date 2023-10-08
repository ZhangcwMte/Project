#include<iostream>

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

    list<int>::const_reverse_iterator it = lt.begin();
    std::cout << *it << std::endl;

    return 0;
}

