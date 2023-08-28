#include<iostream>
#include<ctime>
#include"speechManager.h"
using namespace std;

int main()
{
	srand((unsigned int)time(NULL));

	SpeechManager sm;

	int choice = 0;

	while (true)
	{
		sm.showMenu();

		cout << "请输入功能编号：" << endl;
		if (!(cin >> choice))
		{
			cout << "请输入合法字符：" << endl;
			cin.clear();
			cin.ignore(1024 , '\n');
		}

		switch (choice)
		{
		case 1:
			sm.startSpeech();
			break;
		case 2:
			sm.showRecord();
			break;
		case 3:
			sm.clearRecord();
			break;
		case 0:
			sm.exitSystem();
			break;
		default:
			cout << "请输入正确的编号!" << endl;
			break;
		}
		system("pause");
		system("cls");
	}

	system("pause");

	return 0;
}