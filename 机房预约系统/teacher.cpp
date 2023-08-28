#include"teacher.h"

Teacher::Teacher()
{
	
}

Teacher::Teacher(int empId, string name, string pwd)
{
	this->m_EmpId = empId;
	this->m_Name = name;
	this->m_Pwd = pwd;
	this->initcomputerRoom();
}

void Teacher::operMenu()
{
	cout << "欢迎" << this->m_Name << "同学登录！" << endl;
	cout << "\t\t-------------------------------\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          1.查看所有预约     |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          2.审核预约         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          0.注销登录         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t-------------------------------\n";
	cout << "请输入功能选择：" << endl;
}

void Teacher::showAllOrder()
{
	OrderFile of;
	if (of.m_Size == 0)
	{
		cout << "无预约记录！" << endl;
		return;
	}
	for (int i = 0; i < of.m_Size; i++)
	{
		cout << "预约日期：周" << of.m_OrderDate[i]["date"] << "\t";
		cout << "时间段：" << (of.m_OrderDate[i]["interval"] == "1" ? "上午" : "下午") << "\t";
		cout << "学号：" << of.m_OrderDate[i]["stuId"] << "\t";
		cout << "姓名：" << of.m_OrderDate[i]["stuName"] << "\t";
		cout << "机房号：" << of.m_OrderDate[i]["roomId"] << "\t";
		string status = "状态：";
		switch (atoi(of.m_OrderDate[i]["status"].c_str()))
		{
		case 1:
			status += "审核中";
			break;
		case 2:
			status += "预约成功";
			break;
		case -1:
			status += "预约失败";
			break;
		case 0:
			status += "预约取消";
			break;
		}
		cout << status << endl;
	}
}

void Teacher::validOrder()
{
	OrderFile of;
	if (of.m_Size == 0)
	{
		cout << "无预约记录！" << endl;
		return;
	}
	cout << "可审核的预约记录：" << endl;
	vector<int>v;
	int index = 1;
	for (int i = 0; i < of.m_Size; i++)
	{
			if (atoi(of.m_OrderDate[i]["status"].c_str()) == 1)
			{
				v.push_back(i);
				cout << index++ << "丶";
				cout << "预约日期：周" << of.m_OrderDate[i]["date"] << " ";
				cout << "时间段：" << (of.m_OrderDate[i]["interval"] == "1" ? "上午" : "下午") << " ";
				cout << "机房号：" << of.m_OrderDate[i]["roomId"] << " ";
				string status = "状态：";
				switch (atoi(of.m_OrderDate[i]["status"].c_str()))
				{
				case 1:
					status += "审核中";
					break;
				case 2:
					status += "预约成功";
					break;
				case -1:
					status += "预约失败";
					break;
				case 0:
					status += "预约取消";
					break;
				}
				cout << status << endl;
		}
	}
	cout << "请选择审核的记录，0代表返回" << endl;
	while (true)
	{
		int select = inputInt();

		if (select >= 0 && select <= v.size())
		{
			if (select == 0)
			{
				break;
			}
			else
			{
				cout << "请选择审核结果：" << endl;
				cout << "1.通过" << endl;
				cout << "2.不通过" << endl;
				int ret = inputInt();
				if (ret == 1 || ret == 2)
				{
					if (ret == 1)
					{
						of.m_OrderDate[v[select - 1]]["status"] = "2";
					}
					else if (ret == 2)
					{
						of.m_OrderDate[v[select - 1]]["status"] = "-1";
						vCom[atoi(of.m_OrderDate[v[select - 1]]["roomId"].c_str()) - 1].m_Person--;
					}
				}
			}


			of.updateOrder();
			cout << "审核完成！" << endl;

			return;
		}
		cout << "输入有误，请重新输入：" << endl;
	}
}

void Teacher::initcomputerRoom()
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

