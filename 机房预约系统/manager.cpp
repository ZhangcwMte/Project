#include<algorithm>
#include"manager.h"

void printStudent(Student& s)
{
	cout << "学号：" << s.m_Id << "用户名：" << s.m_Name << "密码：" << s.m_Pwd << endl;
}

void printTeacher(Teacher& t)
{
	cout << "职工号：" << t.m_EmpId << "用户名：" << t.m_Name << "密码：" << t.m_Pwd << endl;
}

Manager::Manager()
{

}

Manager::Manager(string name, string pwd)
{
	this->m_Name = name;
	this->m_Pwd = pwd;
	this->initVector();
	this->initcomputerRoom();
}

void Manager::operMenu()
{
	cout << "欢迎管理员：" << this->m_Name << "登录！" << endl;
	cout << "\t\t-------------------------------\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          1.添加账号         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          2.查看账号         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          3.查看机房信息     |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          4.清空预约         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          0.注销登录         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t-------------------------------\n";
	cout << "请输入功能选择：" << endl;
}

void Manager::addPerson()
{
	cout << "请输入添加账号的类型：" << endl;
	cout << "1.添加学生账号" << endl;
	cout << "2.添加教师账号" << endl;

	string fileName;
	string tip;
	string errorTip;
	ofstream ofs;
	int select = inputInt();

	if (select == 1)
	{
		fileName = STUDENT_FILE;
		tip = "请输入学号：";
		errorTip = "该学号已存在，请重新添加！";
	}
	else
	{
		fileName = TEACHER_FILE;
		tip = "请输入职工号：";
		errorTip = "该职工号已存在，请重新添加！";
	}

	ofs.open(fileName, ios::out | ios::app);

	cout << tip << endl;
	int id = inputInt();
	bool ret = this->checkRepeat(id, select);
	if (ret)
	{
		cout << errorTip << endl;
		return;
	}

	cout << "请输入姓名：" << endl;
	string name;
	cin >> name;

	cout << "请输入密码：" << endl;
	string pwd;
	cin >> pwd;

	ofs << id << " " << name << " " << pwd << " " << endl;
	cout << "添加成功！" << endl;
	ofs.close();
	this->initVector();
}

void Manager::showPerson()
{
	cout << "请选择查看内容：" << endl;
	cout << "1.查看全部学生账号" << endl;
	cout << "2.查看全部教师账号" << endl;

	int select = inputInt();

	if (select == 1)
	{
		cout << "学生账号列表：" << endl;
		for_each(vStu.begin(), vStu.end(), printStudent);
	}
	else if (select == 2)
	{
		cout << "教师账号列表：" << endl;
		for_each(vTea.begin(), vTea.end(), printTeacher);
	}
 }

void Manager::showComputer()
{
	cout << "机房信息如下：" << endl;
	for (vector<ComputerRoom>::iterator it = vCom.begin(); it != vCom.end(); it++)
	{
		cout << "机房编号：" << it->m_Comid << "机房最大容量：" << it->m_MaxNum << endl;
	}
}

void Manager::cleanFile()
{
	ofstream ofs;
	ofs.open(ORDER_FILE, ios::trunc);
	ofs.close();
	cout << "清空成功！" << endl;
}

void Manager::initVector()
{
	vStu.clear();
	vTea.clear();
	ifstream ifs;
	ifs.open(STUDENT_FILE, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件读取失败！" << endl;
		return;
	}

	Student s;
	while (ifs >> s.m_Id && ifs >> s.m_Name && ifs >> s.m_Pwd)
	{
		vStu.push_back(s);
	}
	ifs.close();

	ifs.open(TEACHER_FILE , ios::in);
	if (!ifs.is_open())
	{
		cout << "文件读取失败！" << endl;
		return;
	}

	Teacher t;
	while (ifs >> t.m_EmpId && ifs >> t.m_Name && ifs >> t.m_Pwd)
	{
		vTea.push_back(t);
	}
	ifs.close();
}

bool Manager::checkRepeat(int id, int type)
{
	if (type == 1)
	{
		for (vector<Student>::iterator it = vStu.begin(); it != vStu.end(); it++)
		{
			if (id == it->m_Id)
			{
				return true;
			}
		}
	}
	else
	{
		for (vector<Teacher>::iterator it = vTea.begin(); it != vTea.end(); it++)
		{
			if (id == it->m_EmpId)
			{
				return true;
			}
		}
	}
	return false;
}

void Manager::initcomputerRoom()
{
	ifstream ifs;
	ifs.open(COMPUTER_FILE, ios::in);
	ComputerRoom cr;
	while (ifs >> cr.m_Comid && ifs >> cr.m_MaxNum && ifs >> cr.m_Person)
	{
		this->vCom.push_back(cr);
	}
	ifs.close();
}