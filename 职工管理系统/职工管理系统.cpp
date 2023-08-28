#include"workerManager.h"
#include"worker.h"
#include"employee.h"
#include"manager.h"
#include"boss.h"

int main()
{

	WorkerManager wm;
	int choice = 0;
	while (true)
	{
		wm.Show_Menu();
		cout << "请选择功能编号：" << endl;
		while (true)
		{
			if (!(cin >> choice))
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
		switch (choice)
		{
		case 0:
			wm.exitSystem();
			break;
		case 1:
			wm.addEmp();
			break;
		case 2:
			wm.showEmp();
			break;
		case 3:
			wm.delEmp();
			break;
		case 4:
			wm.modEmp();
			break;
		case 5:
			wm.findEmp();
			break;
		case 6:
			wm.sortEmp();
			break;
		case 7:
			wm.cleanFile();
			break;
		default:
			cout << "请输入正确的编号" << endl;
			break;
		}
		system("pause");
		system("cls");
	}

	system("pause");

	return 0;
}