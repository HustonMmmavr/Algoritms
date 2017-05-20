// ConsoleApplication3.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MEMORY_ERROR -1
#define MEMORY_SUCCESS 1
//#define TEST
typedef int T;

struct point
{
	int x;
	int y;
};

struct point Search(T* A, T* B, int n)
{
	int i0 = 0, j0 = 0;
	int i, j;
	int maxPosA = 0, maxPosB = 0;
	T max = INT_MIN;
	T newPosA = 0;
	T newPosB = 0;
	T maxA = A[0];
	T maxB = B[0];
	// i <= j
	//while (i <= j)
	for (int i = 0; i < n; i++)
	{

		if (maxA < A[i])
		{
			maxA = A[i];
			newPosA = i;
		}

		if (maxB < B[i] || newPosA > j0)
		{
			maxB = B[i];
			newPosB = i;
		}

		if (newPosA < j0)
		{
			if (A[i0] + B[j0] < A[newPosA] + B[j0])
				i0 = newPosA;
		}
		if (newPosB > i0)
		{
			if (A[i0] + B[j0] < A[i0] + B[newPosB])
				j0 = newPosB;
		}

		if(newPosA <= newPosB)
			if (A[newPosA] + B[newPosB] > A[i0] + B[j0])
			{
				i0 = newPosA;
				j0 = newPosB;
			}
	}
	point res = { i0, j0 };
	return res;
}

int ReadArray(T** array, int n)
{
	T* arr = new T[n];
	if (!arr) return MEMORY_ERROR;
	for (int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	*array = arr;
	return MEMORY_SUCCESS;
}


int main()
{
	int n;
	scanf("%d", &n);
	int *A, *B;
	if (ReadArray(&A, n) == MEMORY_ERROR)
		return 0;
	if (ReadArray(&B, n) == MEMORY_ERROR)
	{
		delete[] A;
		return 0;
	}

	struct point res = Search(A, B, n);
	printf("%d %d", res.x, res.y);
#ifdef TEST
	getchar();
	getchar();
#endif
    return 0;
}

