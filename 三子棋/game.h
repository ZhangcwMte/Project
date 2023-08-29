#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#define ROW 3
#define COL 3

void initBoard(char arr[ROW][COL], int row, int col);

void displayBoard(char arr[ROW][COL], int row, int col);

void playerMove(char arr[ROW][COL], int row, int col);

void computerMove(char arr[ROW][COL], int row, int col);

char isWin(char arr[ROW][COL], int row, int col);