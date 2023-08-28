#include<iostream>
#include<fstream>
using namespace std;
#include"Identity.h"
#include"globalFile.h"
#include"student.h"
#include"teacher.h"
#include"manager.h"

int inputInt()
{
	int num;
	while (true)
	{
		if (!(cin >> num))
		{
			cout << "请输入合法字符：" << endl;
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else
		{
			break;
		}
	}
	return num;
}

void managerMenu(Identity* &manager)
{
	while (true)
	{
		manager->operMenu();

		Manager* man = (Manager*)manager;

		int select = inputInt();

		switch (select)
		{
		case 1://添加账号
			man->addPerson();
			break;
		case 2://查看账号
			man->showPerson();
			break;
		case 3://查看机房信息
			man->showComputer();
			break;
		case 4://清空预约
			man->cleanFile();
			break;
		case 0:
			delete manager;
			cout << "注销成功！" << endl;
			return;
		default:
			cout << "请输入正确的功能编号！" << endl;
			break;
		}
		system("pause");
		system("cls");
	}
}

void studentMenu(Identity*& student)
{
	while (true)
	{
		student->operMenu();

		Student* stu = (Student*)student;

		int select = inputInt();

		switch (select)
		{
		case 1://申请预约
			stu->applyOrder();
			break;
		case 2://查看我的预约
			stu->showMyOrder();
			break;
		case 3://查看所有预约
			stu->showAllOrder();
			break;
		case 4://取消预约
			stu->cancelOrder();
			break;
		case 0:
			delete student;
			cout << "注销成功！" << endl;
			return;
		default:
			cout << "请输入正确的功能编号！" << endl;
			break;
		}
		system("pause");
		system("cls");
	}
}

void teacherMenu(Identity*& teacher)
{
	while (true)
	{
		teacher->operMenu();

		Teacher* tea = (Teacher*)teacher;

		int select = inputInt();

		switch (select)
		{
		case 1://查看所有预约
			tea->showAllOrder();
			break;
		case 2://审核预约
			tea->validOrder();
			break;
		case 0://注销登录
			delete teacher;
			cout << "注销成功！" << endl;
			return;
		default:
			cout << "请输入正确的功能编号！" << endl;
			break;
		}
		system("pause");
		system("cls");
	}
}

void login(string fileName, int type)
{
	Identity* person = NULL;//父类指针

	ifstream ifs;
	ifs.open(fileName, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件不存在！" << endl;
		ifs.close();
		return;
	}

	//接受用户信息
	int id = 0;
	string name;
	string pwd;

	if (type == 1)
	{
		cout << "请输入学号：" << endl;
		id = inputInt();
	}
	else if(type == 2)
	{
		cout << "请输入职工号：" << endl;
		id = inputInt();
	}
	cout << "请输入用户名：" << endl;
	cin >> name;

	cout << "请输入密码：" << endl;
	cin >> pwd;

	if (type == 1)
	{
		//学生身份验证
		int fId;
		string fName;
		string fPwd;
		while (ifs >> fId && ifs >> fName && ifs >> fPwd)
		{
			if (id == fId && name == fName && pwd == fPwd)
			{
				cout << "学生账号验证登录成功！" << endl;
				system("pause");
				system("cls");
				person = new Student(fId, fName, fPwd);
				//进入学生子菜单
				studentMenu(person);
				return;
			}
		}
	}
	else if (type == 2)
	{
		//教师身份验证
		int fId;
		string fName;
		string fPwd;
		while (ifs >> fId && ifs >> fName && ifs >> fPwd)
		{
			if (id == fId && name == fName && pwd == fPwd)
			{
				cout << "教师账号验证登录成功！" << endl;
				system("pause");
				system("cls");
				person = new Teacher(fId, fName, fPwd);
				//进入教师子菜单
				teacherMenu(person);
				return;
			}
		}
	}
	else if (type == 3)
	{
		//管理员身份验证
		string fName;
		string fPwd;
		while (ifs >> fName && ifs >> fPwd)
		{
			if (name == fName && pwd == fPwd)
			{
				cout << "管理员账号验证登录成功！" << endl;
				system("pause");
				system("cls");
				person = new Manager(fName, fPwd);
				//进入管理员子菜单
				managerMenu(person);
				return;
			}
		}
	}
	cout << "验证登录失败！" << endl;
}

int main()
{
	int select;
	while (true)
	{
		cout << "========================= 机房预约系统 =========================" << endl;
		cout << "\t\t--------------------------------\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|           1.学生端           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|           2.教师端           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|           3.管理员           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|           0.退出             |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t--------------------------------\n";
		cout << "请输入功能选择：" << endl;

		select = inputInt();

		switch (select)
		{
		case 1://学生端
			login(STUDENT_FILE, select);
			break;
		case 2://教师端
			login(TEACHER_FILE, select);
			break;
		case 3://管理员
			login(ADMIN_FILE, select);
			break;
		case 0:
			cout << "欢迎下次使用！" << endl;
			system("pause"); 
			return 0;
			break;
		default:
			cout << "请输入正确的编号！" << endl;
			break;
		}
		system("pause");
		system("cls");
	}
	
	

	system("pause");

	return 0;
}