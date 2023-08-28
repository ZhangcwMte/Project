#pragma once
#include<iostream>
using namespace std;

class ComputerRoom
{
public:
	int m_Comid;//机房id号

	int m_MaxNum;//机房最大容量

	int m_Person = 0;//机房预约学生人数
};