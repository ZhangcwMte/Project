#pragma once
#include<iostream>
#include<string>
#include"worker.h"
using namespace std;

class Manager : public Worker
{
public:

	Manager(int id, string name, int did);

	void showInfo();

	string getDeptName();
};
