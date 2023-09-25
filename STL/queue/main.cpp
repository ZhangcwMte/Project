#include<iostream>
#include<vector>
#include<functional>
using namespace std;

#include"priority_queue.h"

int main()
{
    Z::priority_queue<int> pq;

    pq.push(4);
    pq.push(9);
    pq.push(6);
    pq.push(3);
    pq.push(1);
    pq.push(7);
    pq.push(5);

    for(int i = 0; i < 7; i++)
    {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;

    return 0;
}
