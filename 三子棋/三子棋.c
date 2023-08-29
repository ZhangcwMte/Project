#define _CRT_SECURE_NO_WARNINGS
#include"game.h"

void menu()
{
	printf("**********************\n");
	printf("******* 1.play *******\n");
	printf("******* 0.exit *******\n");
	printf("**********************\n");
	printf("请输入您的选择：\n");
}

void game()
{

	//棋盘数据
	char board[ROW][COL] = { 0 };
	//棋盘初始化
	initBoard(board, ROW, COL);
	//打印棋盘
	displayBoard(board, ROW, COL);
	while (1)
	{
		//玩家操作
		playerMove(board, ROW, COL);
		displayBoard(board, ROW, COL);
		char ret = isWin(board, ROW, COL);
		//电脑操作
		computerMove(board, ROW, COL);
		displayBoard(board, ROW, COL);
		ret = isWin(board, ROW, COL);
		if (ret != 'c')
		{
			if (ret == 'o')
			{
				printf("玩家胜利！\n");
			}
			else if (ret == 'x')
			{
				printf("电脑胜利！\n");
			}
			else
			{
				printf("平局！\n");
			}
			break;
		}
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	int input = 0;
	do
	{
		system("cls");
		menu();
		scanf("%d", &input);
		switch(input)
		{
		case 1:
			game();
			break;
		case 0:
			break;
		default:
			printf("输入有误！");
		}
		system("pause");
	} while (input);

	return 0;
}