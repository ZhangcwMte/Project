#include"workerManager.h"

WorkerManager::WorkerManager()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	//文件不存在
	if (!ifs.is_open())
	{
		//cout << "文件不存在" << endl;
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;
		this->m_FileIsEmpty = true;
		ifs.close();
		return;
	}
	//文件为空
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		//cout << "文件为空" << endl;
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;
		this->m_FileIsEmpty = true;
		ifs.close();
		return;
	}
	//文件存在，且有数据
	int num = this->get_EmpNum();
	//cout << "职工人数为：" << num << endl;
	this->m_EmpNum = num;

	//开辟空间
	this->m_EmpArray = new Worker * [this->m_EmpNum];
	//将文件中的数据存到数组中
	this->init_Emp();
	ifs.close();
}

void WorkerManager::Show_Menu()
{
	cout << "***************************************" << endl;
	cout << "******* 欢迎使用职工管理系统! *******" << endl;
	cout << "***************************************" << endl;
	cout << "*********** 0.退出管理程序 ***********" << endl;
	cout << "*********** 1.增加职工信息 ***********" << endl;
	cout << "*********** 2.显示职工信息 ***********" << endl;
	cout << "*********** 3.删除离职职工 ***********" << endl;
	cout << "*********** 4.修改职工信息 ***********" << endl;
	cout << "*********** 5.查找职工信息 ***********" << endl;
	cout << "*********** 6.按照编号排序 ***********" << endl;
	cout << "*********** 7.清空所有文档 ***********" << endl;
	cout << "***************************************" << endl;
}

void WorkerManager::exitSystem()
{
	cout << "欢迎下次使用！" << endl;
	system("pause");
	exit(0);
}

void WorkerManager::addEmp()
{
	cout << "请输入添加员工数量：" << endl;
	int addNum = 0;
	while (true)
	{
		if (!(cin >> addNum))
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

	if (addNum > 0)
	{
		int newSize = this->m_EmpNum + addNum;
		Worker** newSpace = new Worker * [newSize];
		if (this->m_EmpArray != NULL)
		{
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				newSpace[i] = this->m_EmpArray[i];
			}
		}
		for (int i = 0; i < addNum; i++)
		{
			int id;//职工编号
			string name;//职工姓名
			int dselect;//部门选择
			cout << "请输入第" << i + 1 << "个新职工编号：" << endl;
			while (true)
			{
				if (!(cin >> id))
				{
					cout << "请输入合法字符：" << endl;
					cin.clear();
					cin.ignore(1024, '\n');
				}
				else if (this->isExist(id) != -1)
				{
					cout << "该编号的职工已存在,请重新输入：" << endl;
					continue;
				}
				else
				{
					break;
				}
			}	
			cout << "请输入第" << i + 1 << "个新职工姓名：" << endl;
			cin >> name;
			cout << "请选择该职工的岗位：" << endl;
			cout << "1.普通职工" << endl;
			cout << "2.经理" << endl;
			cout << "3.老板" << endl;
			while (true)
			{
				if (!(cin >> dselect))
				{
					cout << "请输入合法字符：" << endl;
					cin.clear();
					cin.ignore(1024, '\n');
				}
				else if (dselect < 1 || dselect > 3)
				{
					cout << "请输入正确的序号：" << endl;
					continue;
				}
				else
				{
					break;
				}
			}

			Worker* worker = NULL;
			switch (dselect)
			{
			case 1:
				worker = new Employee(id , name , dselect);
				break;
			case 2:
				worker = new Manager(id, name, dselect);
				break;
			case 3:
				worker = new Boss(id, name, dselect);
				break;
			}
			newSpace[this->m_EmpNum + i] = worker;
		}
		delete[] this->m_EmpArray;
		this->m_EmpArray = newSpace;
		this->m_EmpNum = newSize;
		this->m_FileIsEmpty = false;

		this->save();
		cout << "成功添加" << addNum << "名职工" << endl;
	}
	else
	{
		cout << "输入数据有误" << endl;
	}
}

void WorkerManager::save()
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out);
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		ofs << this->m_EmpArray[i]->m_Id << "\t" <<
			this->m_EmpArray[i]->m_Name << "\t" <<
			this->m_EmpArray[i]->m_DeptId << endl;
	}
	ofs.close();
}

int WorkerManager::get_EmpNum()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	int id;
	string name;
	int dId;
	int num = 0;
	while (ifs >> id && ifs >> name >> dId)
	{
		num++;
	}
	return num;
}

void WorkerManager::init_Emp()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	int id;
	string name;
	int dId;
	int index = 0;
	while (ifs >> id && ifs >> name && ifs >> dId)
	{
		Worker* worker = NULL;
		if (dId == 1)
		{
			worker = new Employee(id, name, dId);
		}
		else if (dId == 2)
		{
			worker = new Manager(id, name, dId);
		}
		else
		{
			worker = new Boss(id, name, dId);
		}
		this->m_EmpArray[index] = worker;
		index++;
	}
}

void WorkerManager::showEmp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else
	{
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			this->m_EmpArray[i]->showInfo();
		}
	}
}

void WorkerManager::delEmp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else
	{
		cout << "请输入要删除的职工编号：" << endl;
		int id = 0;
		while (true)
		{
			if (!(cin >> id))
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
		int index = this->isExist(id);
		if (index != -1)
		{
			for (int i = index; i < this->m_EmpNum - 1; i++)
			{
				this->m_EmpArray[i] = this->m_EmpArray[i + 1];
			}
			this->m_EmpNum--;
			this->save();
			cout << "删除成功" << endl;
			if (this->m_EmpNum == 0)
			{
				this->m_FileIsEmpty = true;
			}
		}
		else
		{
			cout << "该职工不存在" << endl;
		}
	}
}

int WorkerManager::isExist(int id)
{
	int index = -1;
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		if (this->m_EmpArray[i]->m_Id == id)
		{
			index = i;
			break;
		}
	}
	return index;
}

bool WorkerManager::isExist(string name)
{
	bool flag = false;
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		if (this->m_EmpArray[i]->m_Name == name)
		{
			this->m_EmpArray[i]->showInfo();
			flag = true;
		}
	}
	return flag;
}

void WorkerManager::modEmp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else
	{
		cout << "请输入要修改的职工编号：" << endl;
		int id = 0;
		while (true)
		{
			if (!(cin >> id))
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
		int index = this->isExist(id);
		int newid = 0;
		string name;
		int dId = 0;
		if (index != -1)
		{
			delete this->m_EmpArray[index];
			cout << "请修改id为：" << index << "的新职工id：" << endl;
			while (true)
			{
				if (!(cin >> newid))
				{
					cout << "请输入合法字符：" << endl;
					cin.clear();
					cin.ignore(1024, '\n');
				}
				else if (this->isExist(newid) != -1)
				{
					cout << "该编号的职工已存在,请重新输入：" << endl;
					continue;
				}
				else
				{
					break;
				}
			}
			cout << "请输入职工姓名：" << endl;
			cin >> name;
			cout << "请选择该职工的岗位：" << endl;
			cout << "1.普通职工" << endl;
			cout << "2.经理" << endl;
			cout << "3.老板" << endl;
			while (true)
			{
				if (!(cin >> dId))
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
			switch (dId)
			{
			case 1:
				this->m_EmpArray[index] = new Employee(newid, name, dId);
				break;
			case 2:
				this->m_EmpArray[index] = new Manager(newid, name, dId);
				break;
			case 3:
				this->m_EmpArray[index] = new Boss(newid, name, dId);
				break;
			}
			this->save();
			cout << "修改成功" << endl;
		}
		else
		{
			cout << "该职工不存在" << endl;
		}
	}
}

void WorkerManager::findEmp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else
	{
		int select = 0;
		cout << "请选择查找方式：" << endl;
		cout << "1.按编号查找" << endl;
		cout << "2.按姓名查找" << endl;
		while (true)
		{
			if (!(cin >> select))
			{
				cout << "请输入合法字符：" << endl;
				cin.clear();
				cin.ignore(1024, '\n');
			}
			else if (select != 1 && select != 2)
			{
				cout << "请输入正确的序号：" << endl;
				continue;
			}
			else
			{
				break;
			}
		}
		
		if (select == 1)
		{
			int id = 0;
			cout << "请输入查找的职工编号：" << endl;
			while (true)
			{
				if (!(cin >> id))
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
			int index = this->isExist(id);
			if (index != -1)
			{
				this->m_EmpArray[index]->showInfo();
			}
			else
			{
				cout << "该编号的员工不存在" << endl;
			}
		}
		else
		{
			string name;
			cout << "请输入查找的职工姓名：" << endl;
			cin >> name;

			if(!this->isExist(name))
			{
				cout << "该姓名的员工不存在" << endl;
			}
		}
	}
}

void WorkerManager::sortEmp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else
	{
		cout << "请选择排序方式：" << endl;
		cout << "1.升序排列" << endl;
		cout << "2.降序排列" << endl;
		int select = 0;
		while (true)
		{
			if (!(cin >> select))
			{
				cout << "请输入合法字符：" << endl;
				cin.clear();
				cin.ignore(1024, '\n');
			}
			else if (select != 1 && select != 2)
			{
				cout << "请输入正确的序号：" << endl;
				continue;
			}
			else
			{
				break;
			}
		}
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			int minormax = i;
			for (int j = i + 1; j < this->m_EmpNum; j++)
			{
				if (select == 1)
				{
					if (this->m_EmpArray[minormax]->m_Id > this->m_EmpArray[j]->m_Id)
					{
						minormax = j;
					}
				}
				else
				{
					if (this->m_EmpArray[minormax]->m_Id < this->m_EmpArray[j]->m_Id)
					{
						minormax = j;
					}
				}
			}
			if (minormax != i)
			{
				Worker* temp = this->m_EmpArray[i];
				this->m_EmpArray[i] = this->m_EmpArray[minormax];
				this->m_EmpArray[minormax] = temp;
			}
		}
		cout << "排序成功" << endl;
		this->save();
	}
}

void WorkerManager::cleanFile()
{
	cout << "确定要清空文档？数据将无法恢复" << endl;
	cout << "1.是" << endl;
	cout << "2.否" << endl;
	int select = 0;
	while (true)
	{
		if (!(cin >> select))
		{
			cout << "请输入合法字符：" << endl;
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else if (select != 1 && select != 2)
		{
			cout << "请输入正确的序号：" << endl;
			continue;
		}
		else
		{
			break;
		}
	}
	if (select == 1)
	{
		ofstream ofs;
		ofs.open(FILENAME, ios::trunc);
		ofs.close();
		if (this->m_EmpArray != NULL)
		{
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				if (this->m_EmpArray[i] != NULL)
				{
					delete this->m_EmpArray[i];
					this->m_EmpArray[i] = NULL;
				}
			}
			delete[] this->m_EmpArray;
			this->m_EmpArray = NULL;
			this->m_EmpNum = 0;
			this->m_FileIsEmpty = true;
		}
		cout << "数据已清空" << endl;
	}
	else
	{
		return;
	}
}

WorkerManager::~WorkerManager()
{
	if (this->m_EmpArray != NULL)
	{
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			if (this->m_EmpArray[i] != NULL)
			{
				delete this->m_EmpArray[i];
				this->m_EmpArray[i] = NULL;
			}
		}
		delete[] this->m_EmpArray;
		this->m_EmpArray = NULL;
	}
}
