#include"student.h"

Student::Student()
{

}

Student::Student(int id, string name, string pwd)
{
	this->m_Id = id;
	this->m_Name = name;
	this->m_Pwd = pwd;
	this->initcomputerRoom();
}

void Student::operMenu()
{
	cout << "欢迎" << this->m_Name << "同学登录！" << endl;
	cout << "\t\t-------------------------------\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          1.申请预约         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          2.查看我的预约     |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          3.查看所有预约     |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          4.取消预约         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t|          0.注销登录         |\n";
	cout << "\t\t|                             |\n";
	cout << "\t\t-------------------------------\n";
	cout << "请输入功能选择：" << endl;
 }

void Student::applyOrder()
{
	cout << "机房开放时间为周一至周五" << endl;
	cout << "请输入申请预约时间：" << endl;
	cout << "1.周一" << endl;
	cout << "2.周二" << endl;
	cout << "3.周三" << endl;
	cout << "4.周四" << endl;
	cout << "5.周五" << endl;

	int date;
	int interval;
	int room;

	while (true)
	{
		date = inputInt();
		if (date >= 1 && date <= 5)
		{
			break;
		}
		cout << "输入有误，请重新输入：" << endl;
	}

	cout << "请输入申请预约的时间段：" << endl;
	cout << "1.上午" << endl;
	cout << "2.下午" << endl;

	while (true)
	{
		interval = inputInt();
		if (interval == 1 || interval == 2)
		{
			break;
		}
		cout << "输入有误，请重新输入：" << endl;
	}

	cout << "请选择机房：" << endl;
	for (int i = 0; i < vCom.size(); i++)
	{
		cout << vCom[i].m_Comid << "号机房容量为：" << vCom[i].m_MaxNum - vCom[i].m_Person<< endl;
	}

	while (true)
	{
		room = inputInt();
		if (room >= 1 && room <= 3)
		{
			break;
		}
		cout << "输入有误，请重新输入：" << endl;
	}
	cout << "预约成功！请等待教师审核！" << endl;
	vCom[room - 1].m_Person++;

	ofstream ofs;
	ofs.open(ORDER_FILE, ios::app);

	ofs << "date:" << date << " ";
	ofs << "interval:" << interval << " ";
	ofs << "stuId:" << this->m_Id << " ";
	ofs << "stuName:" << this->m_Name << " ";
	ofs << "roomId:" << room << " ";
	ofs << "status:" << 1 << endl;

	ofs.close();
}

void Student::showMyOrder()
{
	OrderFile of;
	if (of.m_Size == 0)
	{
		cout << "无预约记录！" << endl;
		return;
	}
	for (int i = 0; i < of.m_Size; i++)
	{
		if (this->m_Id == atoi(of.m_OrderDate[i]["stuId"].c_str()))
		{
			cout << "预约日期：周" << of.m_OrderDate[i]["date"] << " ";
			cout << "时间段：" << (of.m_OrderDate[i]["interval"] == "1" ? "上午" : "下午") << " ";
			cout << "机房号：" << of.m_OrderDate[i]["roomId"] << " ";
			string status = "状态:";
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
}

void Student::showAllOrder()
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

void Student::cancelOrder()
{
	OrderFile of;
	if (of.m_Size == 0)
	{
		cout << "无预约记录！" << endl;
		return;
	}
	cout << "您可以取消审核中以及成功的预约：" << endl;
	vector<int>v;
	int index = 1;
	for (int i = 0; i < of.m_Size; i++)
	{
		if (this->m_Id == atoi(of.m_OrderDate[i]["stuId"].c_str()))
		{
				if (atoi(of.m_OrderDate[i]["status"].c_str()) == 1 || atoi(of.m_OrderDate[i]["status"].c_str()) == 2)
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
	}
	cout << "请输入取消的记录，0代表返回" << endl;
	while (true)
	{
		int select = inputInt();
		if (select >= 0 && select <= v.size())
		{
			if (select == 0)
			{
				break;
			}
			of.m_OrderDate[v[select - 1]]["status"] = "0";

			of.updateOrder();

			vCom[atoi(of.m_OrderDate[v[select - 1]]["roomId"].c_str()) - 1].m_Person--;

			cout << "预约已取消!" << endl;
			return;
		}
		cout << "输入有误，请重新输入：" << endl;
	}
}

void Student::initcomputerRoom()
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