#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
using namespace std;
#include"globalFile.h"

class Identity
{
public:

	virtual void operMenu() = 0;

	string m_Name;

	string m_Pwd;
};

int inputInt();