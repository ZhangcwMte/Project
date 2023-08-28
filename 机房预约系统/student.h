#pragma once
#include<iostream>
using namespace std;
#include"Identity.h"
#include"computerRoom.h"
#include"orderFile.h"

class Student :public Identity
{
public:
	Student();//默认构造

	Student(int id, string name, string pwd);//有参构造

	virtual void operMenu();//菜单

	void applyOrder();//申请预约

	void showMyOrder();//查看自身预约

	void showAllOrder();//查看所有预约

	void cancelOrder();//取消预约

	void initcomputerRoom();

	int m_Id;//学生学号

	vector<ComputerRoom>vCom;//机房容器
};