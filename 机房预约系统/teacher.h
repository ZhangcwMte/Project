#pragma once
#include<iostream>
using namespace std;
#include"Identity.h"
#include"orderFile.h"
#include"computerRoom.h"

class Teacher :public Identity
{
public:
	Teacher();//默认构造

	Teacher(int empId, string name, string pwd);//有参构造

	virtual void operMenu();//菜单

	void showAllOrder();//查看所有预约

	void validOrder();//审核预约

	void initcomputerRoom();

	int m_EmpId;//职工号

	vector<ComputerRoom>vCom;//机房容器
};
