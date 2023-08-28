#include"speechManager.h"

SpeechManager::SpeechManager()
{
	this->initSpeech(); 

	this->createSpeaker();

	this->loadRecord();
}

void SpeechManager::showMenu()
{
	cout << "********************************************" << endl;
	cout << "***********   欢迎参加演讲比赛   ***********" << endl;
	cout << "***********    1.开始演讲比赛    ***********" << endl;
	cout << "***********    2.查看往届记录    ***********" << endl;
	cout << "***********    3.清空比赛记录    ***********" << endl;
	cout << "***********    0.退出比赛程序    ***********" << endl;
	cout << "********************************************" << endl;
}

void SpeechManager::exitSystem()
{
	cout << "欢迎下次使用！" << endl;
	system("pause");
	exit(0);
}

void SpeechManager::initSpeech()
{
	this->v1.clear();
	this->v2.clear();
	this->V.clear();
	this->m_Speaker.clear();
	this->m_Index = 1;
	this->m_Record.clear();
}

void SpeechManager::createSpeaker()
{
	string nameSeed = "ABCDEFGHIJKL";

	for (int i = 0; i < nameSeed.size(); i++)
	{
		string name = "选手";
		name += nameSeed[i];

		Speaker sp;
		sp.m_Name = name;

		for (int j = 0; j < 2; j++)
		{
			sp.m_Score[j] = 0;
		}

		this->v1.push_back(i + 10001);

		this->m_Speaker.insert(make_pair(i + 10001, sp));
	}
}

void SpeechManager::startSpeech()
{
	//第一轮开始比赛

	//抽签
	this->speechDraw();
	//比赛
	this->speechContest();
	//显示晋级结果
	this->showScore();
	//第二轮开始比赛
	this->m_Index++;
	//抽签
	this->speechDraw();
	//比赛
	this->speechContest();
	//显示最终结果
	this->showScore();
	//保存分数到文件中
	this->saveRecord();
	//重置
	this->m_Index = 1;
	this->initSpeech();
	this->createSpeaker();
	this->loadRecord();
	cout << "本届比赛结束！" << endl;
}

void SpeechManager::speechDraw()
{
	cout << "第" << this->m_Index << "轮比赛选手正在抽签" << endl;
	cout << "------------------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;
	if (this->m_Index == 1)
	{
		random_shuffle(v1.begin(), v1.end());
		for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	else
	{
		random_shuffle(v2.begin(), v2.end());
		for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	cout << "------------------------------" << endl;
	system("pause");
}

void SpeechManager::speechContest()
{
	cout << "-----------第" << this->m_Index << "轮比赛正式开始：-----------" << endl;

	multimap<double, int , greater<double>>groupScore;

	int num = 0;

	vector<int>v_Src;
	if (this->m_Index == 1)
	{
		v_Src = v1;
	}
	else
	{
		v_Src = v2;
	}

	for (vector<int>::iterator it = v_Src.begin(); it != v_Src.end(); it++)
	{
		num++;
		deque<double>d;
		for (int i = 0; i < 10; i++)
		{
			double score = (rand() % 401 + 600) / 10.f;
			d.push_back(score);
		}

		sort(d.begin(), d.end(), greater<double>());
		d.pop_front();
		d.pop_back();

		double sum = accumulate(d.begin(), d.end(), 0.0f);
		double avg = sum / double(d.size());

		this->m_Speaker[*it].m_Score[this->m_Index - 1] = avg;

		groupScore.insert(make_pair(avg, *it));

		if (num % 6 == 0)
		{
			cout << "第" << num / 6 << "小组比赛名次：" << endl;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); it != groupScore.end(); it++)
			{
				cout << "选手编号：" << it->second << "\t" << "姓名：" << this->m_Speaker[it->second].m_Name << "\t" << "成绩：" <<this->m_Speaker[it->second].m_Score[this->m_Index - 1] << endl;
			}

			int count = 0;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); count < 3; it++, count++)
			{
				if (this->m_Index == 1)
				{
					v2.push_back(it->second);
				}
				else
				{
					V.push_back(it->second);
				}
			}
			groupScore.clear();
			cout << endl;
		}
		
	}
	cout << "-----------第" << this->m_Index << "轮比赛结束-----------" << endl;
	system("pause");
}

void SpeechManager::showScore()
{
	cout << "第" << this->m_Index << "轮晋级选手信息如下：" << endl;
	vector<int>v;
	if (this->m_Index == 1)
	{
		v = v2;
	}
	else
	{
		v = V;
	}
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << "选手编号：" << *it << "姓名：" << this->m_Speaker[*it].m_Name << "得分：" << this->m_Speaker[*it].m_Score[this->m_Index - 1] << endl;
	}
	cout << endl;

	system("pause");
	system("cls");
	this->showMenu();
}

void SpeechManager::saveRecord()
{
	ofstream ofs;
	ofs.open("speech.csv", ios::out | ios::app);//用追加的方式写文件

	for (vector<int>::iterator it = V.begin(); it != V.end(); it++)

	{
		ofs << *it << "," << this->m_Speaker[*it].m_Score[1] << ",";
	}
	ofs << endl;
	ofs.close();
	cout << "记录保存成功！" << endl;
	this->fileIsEmpty = false;
}

void SpeechManager::loadRecord()
{
	ifstream ifs("speech.csv", ios::in);
	if (!ifs.is_open())
	{
		this->fileIsEmpty = true;
		cout << "文件为空" << endl;
		ifs.close();
		return;
	}
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		cout << "数据为空" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	this->fileIsEmpty = false;
	ifs.putback(ch);//将上面读取的单个字符放回
	string data;
	int index = 0;
	while (ifs >> data)
	{
		vector<string>v;
		int pos = -1;//查到","位置的变量
		int start = 0;
		

		while (true)
		{
			pos = data.find(",", start);
			if (pos == -1)
			{
				break;
			}
			string temp = data.substr(start, pos - start);
			v.push_back(temp);
			start = pos + 1;
		}
		this->m_Record.insert(make_pair(index, v));
		v.clear();
		index++;

	}
	ifs.close();
}

void SpeechManager::showRecord()
{
	if (this->fileIsEmpty == true)
	{
		cout << "文件不存在或记录为空!" << endl;
	}
	else
	{
		for (int i = 0; i < this->m_Record.size(); i++)
		{
			cout << "第" << i + 1 << "届"
				<< "冠军编号：" << this->m_Record[i][0] << "得分：" << this->m_Record[i][1] << "\t"
				<< "亚军编号：" << this->m_Record[i][2] << "得分：" << this->m_Record[i][3] << "\t"
				<< "季军编号：" << this->m_Record[i][4] << "得分：" << this->m_Record[i][5] << endl;
		}
	}
}

void SpeechManager::clearRecord()
{
	cout << "确认清空全部数据？" << endl;
	cout << "1.确认" << endl;
	cout << "2.取消" << endl;

	int choice;

	if (!(cin >> choice))
	{
		cout << "请输入合法字符：" << endl;
		cin.clear();
		cin.ignore(1024, '\n');
	}

	if (choice == 1)
	{
		ofstream ofs("speech.csv", ios::trunc);
		ofs.close();

		this->initSpeech();

		this->createSpeaker();

		this->loadRecord();

		cout << "清空成功！" << endl;

		this->fileIsEmpty = true;
	}
	else
	{
		return;
	}
}

SpeechManager::~SpeechManager()
{

}