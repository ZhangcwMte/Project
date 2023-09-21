#define _CRT_SECURE_NO_WARNINGS 
#include"MergeSortFile.h"

int cmpfunc(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

void _mergeFile(const char* file1, const char* file2, const char* files)
{
	FILE* fout1 = fopen(file1, "r");
	if (fout1 == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}
	FILE* fout2 = fopen(file2, "r");
	if (fout2 == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}
	FILE* fin = fopen(files, "w");
	if (fin == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	int num1, num2;
	int ret1 = fscanf(fout1, "%d\n", &num1);
	int ret2 = fscanf(fout2, "%d\n", &num2);
	while (ret1 != EOF && ret2 != EOF)
	{
		if (num1 <= num2)
		{
			fprintf(fin, "%d\n", num1);
			ret1 = fscanf(fout1, "%d\n", &num1);
		}
		else
		{
			fprintf(fin, "%d\n", num2);
			ret2 = fscanf(fout2, "%d\n", &num2);
		}
	}
	while (ret1 != EOF)
	{
		fprintf(fin, "%d\n", num1);
		ret1 = fscanf(fout1, "%d\n", &num1);
	}
	while (ret2 != EOF)
	{
		fprintf(fin, "%d\n", num2);
		ret2 = fscanf(fout2, "%d\n", &num2);
	}
	fclose(fout1);
	fclose(fout1);
	fclose(fin);
}

void mergeSortFile(const char* file)
{
	FILE* fout = fopen(file, "r");
	if (fout == NULL)
	{
		printf("文件打开失败\n");
		exit(0);
	}

	int range = 10;
	int num = 0;
	int array[10] = { 0 };
	int i = 0;
	char subfile[20];
	int filenum = 1;

	while (fscanf(fout, "%d\n", &num) != EOF)
	{
		if (i < range - 1)
		{
			array[i++] = num;
		}
		else
		{
			array[i] = num;
			qsort(array, range, sizeof(int), cmpfunc);
			sprintf(subfile, "%d", filenum++);
			FILE* fin = fopen(subfile, "w");
			if (fin == NULL)
			{
				printf("文件打开失败\n");
				exit(0);
			}
			for (int j = 0; j < range; j++)
			{
				fprintf(fin, "%d\n", array[j]);
			}
			fclose(fin);
			i = 0;
		}
	}

	char files[100] = "12";
	char file1[100] = "1";
	char file2[100] = "2";
	for (int k = 2; k <= range; k++)
	{
		_mergeFile(file1, file2, files);

		strcpy(file1 , files);
		sprintf(file2, "%d", k + 1);
		sprintf(files, "%s%d", files , k + 1);
	}

	fclose(fout);
}