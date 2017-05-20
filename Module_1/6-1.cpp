// ConsoleApplication6.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
//#define TEST
typedef long T;
T** InitMatrix(T n)
{
	int cnt = n + 1;
	T** matrix = new T*[cnt];
	if (!matrix) return NULL;
	for (T i = 0; i < cnt; i++)
	{
		matrix[i] = new T[cnt];
		if (!matrix[i])
		{
			for (int j = 0; j < i; j++)
				delete[] matrix[j];
			delete matrix;
		}
		for (int k = 0; k < cnt; k++)
			matrix[i][k] = 0;
		matrix[i][i] = 1;
		matrix[i][1] = 1;
	}
	return matrix;
}


void PrintMatrix(T** arr, T n)
{
	for (T i = 0; i < n; i++)
	{
		for (T j = 0; j < n; j++)
			printf("%d ", arr[i][j]);
		printf("\n");
	}
}

T GetCountPyramids(T** arr, T n)
{
	for (T i = 3; i <= n; i++)
	{
		for (T j = 2; j < i; j++)
		{
			T sum = 0;
			T idx = i - j;
			for (int k = 1; k <= j; k++)
			{
				if (k > idx) break;
				sum += arr[idx][k];
			}
			arr[i][j] = sum;
		}
	}

	T res = 0;
	for (T i = 1; i < n + 1; i++)
		res += arr[n][i];
#ifdef TEST
	printf("\n");
	printf("\n");
	PrintMatrix(arr, n);
#endif
	return res;
}


void FreeMatrix(T **arr, int n)
{
	int cnt = n + 1;
	for (int i = 0; i < cnt; i++)
		delete[] arr[i];
	delete[] arr;
}
//#include <utility>

int main()
{
	//std::pair<int, double> g;
	T n;
	scanf("%ld", &n);
	//n += 1;
	T** matrix = InitMatrix(n);
	if (!matrix)
	{
		printf("error");
	}
#ifdef TEST
	//PrintMatrix(matrix, n);
#endif

	printf("%ld", GetCountPyramids(matrix, n));
	FreeMatrix(matrix, n);
#ifdef TEST
	getchar();
	getchar();
#endif
    return 0;
}

