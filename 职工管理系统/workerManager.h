#pragma once//防止头文件重复包含
#include<iostream>//包含输入输出流头文件
#include<fstream>
#include"worker.h"
#include"employee.h"
#include"manager.h"
#include"boss.h"
#define FILENAME "empFile.txt"
using namespace std;//使用标准的命名空间

class WorkerManager
{
public:

	WorkerManager();

	void Show_Menu();//菜单

	void exitSystem();//退出系统

	int m_EmpNum;//职工人数

	Worker** m_EmpArray;//二级指针

	void addEmp();//添加职工

	void save();//存储到文件中

	bool m_FileIsEmpty;//文件是否为空

	int get_EmpNum();//统计文件中的人数

	void init_Emp();//初始化职工

	void showEmp();//显示职工

	void delEmp();//删除职工

	int isExist(int id);//判断id是否存在

	bool isExist(string name);//通过姓名查找并打印

	void modEmp();//修改职工

	void findEmp();//查找职工

	void sortEmp();//排序

	void cleanFile();//清空数据

	~WorkerManager();
};
