#pragma once
#include<iostream>
#include<string>
using namespace std;

class Worker
{
public:

	virtual void showInfo() = 0;

	virtual string getDeptName() = 0;

	int m_Id;

	string m_Name;

	int m_DeptId;
};