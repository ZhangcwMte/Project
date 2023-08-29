#define _CRT_SECURE_NO_WARNINGS
#include"game.h"

void initBoard(char arr[ROW][COL], int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			arr[i][j] = ' ';
		}
	}
}

void displayBoard(char arr[ROW][COL], int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			printf(" %c ", arr[i][j]);
			if (j < col - 1)
				printf("|");
		}
		printf("\n");

		for (int k = 0; k < col; k++)
		{
			if (i < row - 1)
				printf("___");
			else
				printf("   ");
			if (k < col - 1)
				printf("|");
		}
		printf("\n");
	}
}

void playerMove(char arr[ROW][COL], int row, int col)
{
	printf("玩家回合：\n");
	while (1)
	{
		printf("请输入落子坐标：");
		int x = 0;
		int y = 0;
		scanf("%d%d", &x, &y);
		if (arr[x - 1][y - 1] == ' ')
		{
			arr[x - 1][y - 1] = 'o';
			break;
		}
		else
		{
			printf("无效的坐标！");
		}
	}
	
}

void computerMove(char arr[ROW][COL], int row, int col)
{
	printf("电脑回合：\n");
	while (1)
	{
		int x = rand() % 3;
		int y = rand() % 3;
		if (arr[x][y] != ' ')
			continue;
		arr[x][y] = 'x';
		break;
	}
}

static bool isContinue(char arr[ROW][COL], int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (arr[i][j] == ' ')
			{
				return 1;
			}
		}
	}
	return 0;
}

char isWin(char arr[ROW][COL], int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		if (arr[i][0] == arr[i][1] && arr[i][1] == arr[i][2] && arr[i][1] != ' ')
		{
			return arr[i][1];
		}
	}

	for (int j = 0; j < row; j++)
	{
		if (arr[0][j] == arr[1][j] && arr[1][j] == arr[2][j] && arr[1][j] != ' ')
		{
			return arr[1][j];
		}
	}

	if (arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2] && arr[1][1] != ' ')
	{
		return arr[1][1];
	}

	if (arr[0][2] == arr[1][1] && arr[1][1] == arr[2][0] && arr[1][1] != ' ')
	{
		return arr[1][1];
	}

	if (isContinue(arr , row , col) == 0)
	{
		return 's';
	}
	return 'c';
}
