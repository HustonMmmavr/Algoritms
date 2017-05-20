// ConsoleApplication3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <math.h>
#define MEM_ERR -1
#define SUCCESS 1
typedef long T;
#define TEST
//typedef char byte;
int GetPrimes(T **arr, size_t *size, size_t n)
{
	T x2 = 0, y2 = 0; // squares
	T sqrtN = (T)sqrt(n);
	bool *primeArray = new bool[n + 1];
	if (!primeArray) return MEM_ERR;
	memset(primeArray, 0, (n + 1) * sizeof(bool) );
	primeArray[2] = 1;
	primeArray[3] = 1;

	for (int i = 1; i <= sqrtN; i++)
	{
		x2 += 2 * i - 1;
		y2 = 0;
		for (int j = 1; j <= sqrtN; j++)
		{
			y2 += 2 * j - 1;
			T squareSum = 4 * x2 + y2; // 4x^2 + y^2
			// (num % 60) % 4 == 1
			if (squareSum <= n && (squareSum % 12 == 1 || squareSum % 12 == 5))
				primeArray[squareSum] = !primeArray[squareSum];

			squareSum -= x2; // 3x^2 + y^2;
			// (num % 60) % 6 == 1
			if (squareSum <= n && (squareSum % 12 == 7))
				primeArray[squareSum] = !primeArray[squareSum];
			
			squareSum -= 2 * y2; // 3x^2 - y^2
			// (num % 60) % 12 == 11
			if (i > j && (squareSum <= n) && (squareSum % 12 == 11))
				primeArray[squareSum] = !primeArray[squareSum];
		}
	}


	// delete wrong primes which is power of prime num 
	for (int i = 5; i <= sqrtN; i++)
	{
		if (primeArray[i])
		{
			int square = i * i;
			for (int j = square; j <= n; j += square)
				primeArray[j] = false;
		}
	}
	int resCnt = 0;
	for (int i = 1; i <= n; i++)
		if (primeArray[i]) resCnt++;

	T *resArr = new T[resCnt];
	if (!resArr) return MEM_ERR;
	int j = 0;
	for (int i = 1; i <= n; i++)
		if (primeArray[i]) resArr[j++] = i;

	*arr = resArr;
	*size = resCnt;
	delete[] primeArray;
	return SUCCESS;
}

int main()
{
	FILE *f;
#ifdef TEST
	f = fopen("C:\\users\\bigbrother\\desktop\\tes.txt", "w+");
#else
	f = stdin;
#endif
	int n;
	scanf("%d", &n);
	T *arr;
	size_t size;
	if (GetPrimes(&arr, &size, n) != SUCCESS)
	{
		printf("Error");
		return 0;
	}
	for (int i = 0; i < size; i++)
		fprintf(f, "%d\n", arr[i]);
	char c = 0;
	delete[] arr;
    return 0;
}

