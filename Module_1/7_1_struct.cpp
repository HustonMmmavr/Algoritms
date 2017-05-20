// ConsoleApplication7.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <utility>
//#define TEST
#define MIN_LEN 16
#define MEMORY_STEP 2
#define MEM_ERR -1
#define MEM_SCS 1

int ResizeArray(void **arrayStrings, int *oldSize, int typeSize)
{
	int newSize = *oldSize * MEMORY_STEP;
	void *oldPtr = *arrayStrings;
	void *newPtr = (void*)realloc(oldPtr, newSize * typeSize);
	if (!newPtr) return MEM_ERR;
	*oldSize = newSize;
	*arrayStrings = newPtr;
	return MEM_SCS;
}

typedef struct Man_
{
	int w;
	int s;
} Man;


int comp(const void *arg1, const void *arg2)
{
	Man m1 = *(Man*)arg1;
	Man m2 = *(Man*)arg2;
	return m1.w - m2.w;
}

void QuickSort(Man* arr, int b, int e)
{
	int l = b, r = e;
	Man piv = arr[(l + r) / 2]; 
	while (l <= r)
	{
		while (arr[l].w < piv.w)
			l++;
		while (arr[r].w > piv.w)
			r--;
		if (l <= r)
		{
			std::swap(arr[l], arr[r]);
			l++, r--;
		}
	}
	if (b < r)
		QuickSort(arr, b, r);
	if (e > l)
		QuickSort(arr, l, e);
}


int GetMans(Man **arr)
{
	Man *ar = (Man*)malloc( MIN_LEN * sizeof(Man));
	int allocatedSize = MIN_LEN;
	if (!ar) return MEM_ERR;
	int cnt = 0;
	int w, s;
	while (scanf("%d %d", &w, &s) == 2)
	{
		if (cnt == allocatedSize)
		{
			int err = ResizeArray((void**)&ar, &allocatedSize, sizeof(Man));
			if (err == MEM_ERR)
			{
				free(ar);
				return err;
			}
		}
		ar[cnt++] = { w,s };
	}
	*arr = ar;
	return cnt;
}

int GetHeight(Man* arr, int n)
{
	int f;
	QuickSort(arr, 0, n - 1);
	//qsort((void*)arr, n, sizeof(Man), (int(*) (const void *, const void*)) comp);
	int cnt = 1;
	int totalW = arr[0].w;
#ifdef TEST
	printf("%d %d\n", arr[0].w, arr[0].s);
#endif
	for (int i = 1; i < n; i++)
	{
		if (totalW < arr[i].s)
		{
			totalW += arr[i].w;
			cnt++;
		}
#ifdef TEST
		printf("%d %d\n", arr[i].w, arr[i].s);
#endif
	}

	return cnt;
}

int main()
{
	Man *arr;
	int n = GetMans(&arr);
	if (n == MEM_ERR)
	{
		printf("error");
		return 0;
	}
	int res = GetHeight(arr, n);
	free(arr);
	printf("%d", res);
#ifdef TEST
	getchar();
	getchar();
	getchar();
	getchar();
#endif
	return 0;
}









/*
void BubbleSort(Man *arr, int len)
{
//char arr1
for (int i = 0; i < len; i++)
for (int j = 0; j < len - 1; j++)
{
if (arr[j].w > arr[j+1].w)//comp((void*)&arr[j], (void*)&arr[j+1]))
{
Man temp = arr[j];
arr[j] = arr[j+1];
arr[j+1] = temp;
}
}
}*/