#pragma once
#include<iostream>
#include<vector>
#include<deque>
#include<map>
#include<algorithm>
#include<numeric>
#include<functional>
#include<fstream>
#include"speaker.h"
using namespace std;

class SpeechManager
{
public:
	SpeechManager();

	void showMenu();

	void exitSystem();

	void initSpeech();//初始化容器属性

	void createSpeaker();

	void startSpeech();//开始比赛

	void speechDraw();//抽签

	void speechContest();//比赛流程

	void showScore();//显示晋级结果

	void saveRecord();//保存记录

	void loadRecord();//读取记录

	void showRecord();

	void clearRecord();

	bool fileIsEmpty;//判断文件是否为空

	map<int, vector<string>>m_Record;//存放往届记录

	~SpeechManager();

	vector<int>v1;//存放首轮选手编号

	vector<int>v2;//存放晋级选手编号

	vector<int>V;//存放胜出选手编号

	map<int, Speaker>m_Speaker;//存放选手编号和对应的选手

	int m_Index;//比赛轮数
};