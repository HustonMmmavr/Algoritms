#include <stdio.h>
#include <stdlib.h>
typedef int T;

int UpperBound(T* arr, int n)
{
	int step = 1;
	int doubleStep = step << 1;
	for (; doubleStep < n;)
	{
		if (arr[step] > arr[doubleStep] && doubleStep < n)
			return doubleStep;
		step = doubleStep;
		doubleStep = step << 1;
	}
	return step;
}

int LowerBound(T* arr, int n)
{
	int step = 1;
	int doubleStep = step << 1;
	while (arr[step] < arr[doubleStep] && doubleStep < n)
	{
		step = doubleStep;
		doubleStep = step << 1;
	}
	return step;
}

int BinarySearch(T* arr, int start, int finish)
{
	int left = start, right = finish;

	while (left < right - 1)
	{
		int middle = left + (right - left) / 2;
		if (arr[middle] > arr[middle + 1])
			if (arr[middle - 1] < arr[middle])
				left = middle;
			else
				right = middle;
		else
			if (arr[middle - 1] < arr[middle])
				left = middle;
			else
				right = middle;
	}
	return left;

}


int main()
{
	//int A[] = { 1,2,3,4,5,6,7,6,5,2 };
	//int arr[] = { 0, 20, 30,  29, 28, 27, 26, 25 }; // , 80, 70, 20, 1
	int n = 10;
	scanf("%d", &n);
	T* A = new T[n];
	if (!A)
	{
		return 0;
	}

	for (int i = 0; i < n; i++)
		scanf("%d", &A[i]);
	int st = LowerBound(A, n);
	int fin = UpperBound(A, n);
	//printf("%d %d", st, fin);

	int c = BinarySearch(A, 0, n);
	delete[] A;
	printf("%d", c);
    return 0;
}

