#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<map>
using namespace std;
#include"globalFile.h"

class OrderFile
{
public:
	OrderFile();

	void updateOrder();//更新预约记录

	int m_Size;//记录预约条数

	map<int, map<string, string>>m_OrderDate;//记录所有预约信息的容器 key记录条数 value具体记录键值对信息
};