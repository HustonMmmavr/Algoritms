#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
//#define TEST
// write to dipl and rem

class Exception
{
	char message[1024];
public:
	Exception()
	{

	}
	Exception(const char *message, const char* file, const char* func, int line)
	{
		sprintf(this->message, "In file %s func %s line %d message %s", file, func, line, message);
	}

	const char* GetMesssage()
	{
		return message;
	}
};

inline void ThrowException(const char *msg)
{
	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);
}

typedef int t;
struct point
{
	int x, y;
};
/*
int SearchMax(t* arr, int n)
{
	t max = INT_MIN;
	int idx = 0;
	for (int i = 0; i < n; i++)
		if (arr[i] > max)
		{
			max = arr[i];
			idx = i;
		}
	return idx;
}

struct point SearchIdxs(t* A, t* B, int n)
{
	int j0 = SearchMax(B, n);
	int i0 = SearchMax(A, j0);
	struct point res = { i0, j0 };
	return res;
}
*/

int func1(t a, t b)
{
	return a < b;
}

int func2(t a, t b)
{
	return a <= b;
}


t MaxPos(t* arr, int n, int f(t, t))
{
	t max = INT_MIN;
	int maxPos;
	for (int i = 0; i < n; i++)
	{
		if (f(max, arr[i]))//max <= arr[i])
		{
			max = arr[i];
			maxPos = i;
		}
	}

	return maxPos;
}

struct point Search(t* A, t* B, int n)
{
	int i0 = 0, j0 = 1;
	int maxPosA = MaxPos(A, n, func1);
	int maxPosB = MaxPos(B, n, func2);
	int max = INT_MIN;
	int maxA = A[maxPosA];
	int maxB = B[maxPosB];
	for (int i = maxPosA; i < n; i++)
	{
		if (max < maxA + B[i])
		{
			max = maxA + B[i];
			i0 = maxPosA;
			j0 = i;
		}
	}

	struct point res1 = { i0, j0 };
	t oldMax = max;

	for (int i = 0; i <= maxPosB; i++)
	{
		if (max <= maxB + A[i])
		{
			max = maxB + A[i];
			i0 = i;
			j0 = maxPosB;
		}
	}

	struct point res2 = { i0, j0 };
	if (oldMax == max)
	{
		return res1.y < res2.y ? res1 : res2;
	}
}

int Search(t* A, t*B, int n)
{
	int i = 0, j = n -
	for (int i = 0; i < n; i++)
}


void ReadArray(int n, t*arr)
{
	for (int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
}

int main()
{
	int n;
	scanf("%d", &n);
	t *A, *B;
	A = (t*)malloc(n * sizeof(t));
	B = (t*)malloc(n * sizeof(t));
	if (!A || !B)
	{
		printf("Error allocate");
		return 0;
	}

	ReadArray(n, A);
	ReadArray(n, B);

	struct point res = Search(A, B, n);
	printf("%d %d", res.x, res.y);
	return 0;
}