#include<iostream>
#include<string>
using namespace std;
#define MAX 1000

struct person
{
	string m_Name;
	int m_Sex;
	int m_Age;
	string m_Phone;
	string m_Addr;
};

struct addressbook
{
	person personArray[MAX];
	int m_Size;
};

void showMenu()
{
	cout << "****** 通讯录管理系统 ******" << endl;
	cout << "****************************" << endl;
	cout << "*****   1.添加联系人   *****" << endl;
	cout << "*****   2.显示联系人   *****" << endl;
	cout << "*****   3.删除联系人   *****" << endl;
	cout << "*****   4.查找联系人   *****" << endl;
	cout << "*****   5.修改联系人   *****" << endl;
	cout << "*****   6.清空联系人   *****" << endl;
	cout << "*****   0.退出通讯录   *****" << endl;
	cout << "****************************" << endl;
}

void addPerson(addressbook* abs) {
	if (abs->m_Size == MAX)
	{
		cout << "通讯录已满，无法继续添加联系人" << endl;
		return;
	}
	else
	{
		cout << "请输入姓名：" << endl;
		string name;
		cin >> name;
		abs->personArray[abs->m_Size].m_Name = name;

		cout << "请输入性别：" << endl;
		cout << "1 -- 男" << endl;
		cout << "2 -- 女" << endl;
		int sex;
		
		while (true)
		{
			cin >> sex;
			if (sex == 1 || sex == 2)
			{
				abs->personArray[abs->m_Size].m_Sex = sex;
				break;
			}

				cout << "您输入的编号有误，请重新输入：" << endl;
				cin.clear();//修复输入流
				cin.ignore();//取走引发错误的流中的字符
		}
	
				
		cout << "请输入年龄：" << endl;
		int age;	
		while (true)
		{
			cin >> age;
			if (sex < 150 || sex > 0)
			{
				abs->personArray[abs->m_Size].m_Age = age;
				break;
			}
				cout << "请输入正确的年龄：" << endl;
				cin.clear();
				cin.ignore();
		}
		
		cout << "请输入联系电话：" << endl;
		string phone;
		cin >> phone;
		abs->personArray[abs->m_Size].m_Phone = phone;

		cout << "请输入家庭地址：" << endl;
		string address;
		cin >> address;
		abs->personArray[abs->m_Size].m_Addr = address;

		abs->m_Size++;
		cout << "添加成功" << endl;
		system("pause");
		system("cls");
		return;
	}
}

void showPerson(addressbook* abs)
{
	if (abs -> m_Size == 0)
	{
		cout << "通讯录为空" << endl;
	}
	else
	{
		for (int i = 0; i < abs->m_Size; i++)
		{
			cout << "姓名：" << abs->personArray[i].m_Name << "\t" << "性别：" << (abs->personArray[i].m_Sex == 1 ? "男" : "女") << "\t" << "年龄：" 
				<< abs->personArray[i].m_Age << "\t" << "联系电话：" << abs->personArray[i].m_Phone << "\t" << "家庭住址：" << abs->personArray[i].m_Addr << endl;
		}
	}
	system("pause");
	system("cls");
}

int isExist(addressbook* abs , string name)
{
	for (int i = 0; i < abs->m_Size; i++)
	{
		if (abs->personArray[i].m_Name == name)
		{
			return i;
		}
	}
	return -1;
}

void deletePerson(addressbook * abs)
{
	cout << "请输入您要删除的联系人：" << endl;
	string name;
	cin >> name;
	int ret = isExist(abs, name);
	if (ret != -1)
	{
		for (int i = ret; i < abs->m_Size; i++)
		{
			abs->personArray[i] = abs->personArray[i + 1];
		}
		abs->m_Size--;
		cout << "删除成功" << endl;
	}
	else
	{
		cout << "该联系人不存在" << endl;
	}
	system("pause");
	system("cls");
}

void findPerson(addressbook* abs)
{
	cout << "请输入您要查找的联系人" << endl;
	string name;
	cin >> name;
	int ret = isExist(abs, name);
	if (ret != -1)
	{
		cout << "姓名：" << abs->personArray[ret].m_Name << "\t" << "性别：" << (abs->personArray[ret].m_Sex == 1 ? "男" : "女") << "\t" << "年龄："
			<< abs->personArray[ret].m_Age << "\t" << "联系电话：" << abs->personArray[ret].m_Phone << "\t" << "家庭住址：" << abs->personArray[ret].m_Addr << endl;
	}
	else
	{
		cout << "该联系人不存在" << endl;
	}
	system("pause");
	system("cls");
}

void modifyPerson(addressbook * abs)
{
	cout << "请输入您要修改的联系人" << endl;
	string name;
	cin >> name;
	int ret = isExist(abs , name);
	if (ret != -1)
	{
		cout << "请输入姓名：" << endl;
		string n_name;
		cin >> n_name;
		abs->personArray[ret].m_Name = n_name;

		cout << "请输入性别：" << endl;
		cout << "1 -- 男" << endl;
		cout << "2 -- 女" << endl;
		int sex;

		while (true)
		{
			cin >> sex;
			if (sex == 1 || sex == 2)
			{
				abs->personArray[ret].m_Sex = sex;
				break;
			}

			cout << "您输入的编号有误，请重新输入：" << endl;
			cin.clear();//修复输入流
			cin.ignore();//取走引发错误的流中的字符
		}


		cout << "请输入年龄：" << endl;
		int age;
		while (true)
		{
			cin >> age;
			if (sex < 150 || sex > 0)
			{
				abs->personArray[ret].m_Age = age;
				break;
			}
			cout << "请输入正确的年龄：" << endl;
			cin.clear();
			cin.ignore();
		}

		cout << "请输入联系电话：" << endl;
		string phone;
		cin >> phone;
		abs->personArray[ret].m_Phone = phone;

		cout << "请输入家庭地址：" << endl;
		string address;
		cin >> address;
		abs->personArray[ret].m_Addr = address;

		cout << "修改成功" << endl;
	}
	else
	{
		cout << "该联系人不存在" << endl;
	}
	system("pause");
	system("cls");
}

void cleanPerson(addressbook* abs)
{
	cout << "您确定要清空通讯录吗？清空后数据将无法恢复" << endl;
	cout << "1 -- 是" << endl;
	cout << "2 -- 否" << endl;
	int sur = 0;
	while (true)
	{
		cin >> sur;
		if (sur == 1 || sur == 2)
		{
			switch (sur)
			{
			case 1:
				abs->m_Size = 0;
				cout << "通讯录已清空" << endl;
				break;
			case 2:
				break;
			}
			break;
		}

		cout << "您输入的编号有误，请重新输入：" << endl;
		cin.clear();
		cin.ignore();
	}
	
	system("pause");
	system("cls");

}

int main()
{
	string name;
	int select = 0;
	addressbook abs;
	abs.m_Size = 0;

	while (true)
	{
		showMenu();
		cin >> select;

		switch (select)
		{
		case 1:
			addPerson(&abs);
			break;
		case 2:
			showPerson(&abs);
			break;
		case 3:
			deletePerson(&abs);
			break;
		case 4:
			findPerson(&abs);
			break;
		case 5:
			modifyPerson(&abs);
			break;
		case 6:
			cleanPerson(&abs);
			break;
		case 0:
			cout << "欢迎下次使用" << endl;
			system("pause");
			return 0;
			break;
		default:
			cout << "请选择已有功能" << endl;
			system("pause");
			system("cls");
		}

	}


	system("pause");

	return 0;
}