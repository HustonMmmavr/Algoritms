// ConsoleApplication7.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <string.h>
//#define TEST
#define MIN_LEN 16
#define MEMORY_STEP 2
#define MEM_ERR -1
#define MEM_SCS 1

struct Man
{
	int w;
	int s;
};

class Exception
{
	char message[1024];
public:
	Exception() {}
	Exception(const char *message, const char* file, const char* func, int line);
	~Exception() {}
	const char* GetMessage() const;
};

Exception::Exception(const char *message, const char* file, const char* func, int line)
{
	sprintf(this->message, "In file %s func %s line %d message %s", file, func, line, message);
}

const char* Exception::GetMessage() const
{
	return message;
}

#define ThrowException(msg) throw Exception(msg, __FILE__, __FUNCTION__, __LINE__); 

template <typename T>
class LightArray
{
	T* ptr;
	int allocatedSize;
	int count;
public:
	LightArray();
	~LightArray();
	void PushBack(const T& data);
	void Resize();
	int Count() const;
	T* GetPointer();
	void Sort(int comparator(const void *, const void *), int b, int e);
	const T& operator[] (int i) const { return ptr[i]; }
};

template <typename T>
LightArray<T>::LightArray()
{
	allocatedSize = MIN_LEN;
	ptr = new T[allocatedSize];//(T*)malloc(allocatedSize * sizeof(T));

	if (!ptr)
		ThrowException("cant allocate");
	count = 0;
}

template <typename T>
LightArray<T>::~LightArray()
{
	delete[] ptr;
}

template <typename T>
void LightArray<T>::PushBack(const T& data)
{
	if (count == allocatedSize)
		Resize();
	ptr[count++] = data;
}

template <typename T>
void LightArray<T>::Resize()
{
	int newSize = allocatedSize * MEMORY_STEP;
	T *oldPtr = ptr;
	T *newPtr = new T[newSize];//(T*)realloc(oldPtr, newSize * sizeof(T));
	if (!newPtr) ThrowException("Cant reallcate");
	memcpy(newPtr, oldPtr, allocatedSize * sizeof(T));
	delete[] oldPtr;
	ptr = newPtr;
	allocatedSize *= MEMORY_STEP;
}

template <typename T>
int LightArray<T>::Count() const
{
	return count;
}

template <typename T>
T *LightArray<T>::GetPointer()
{
	return ptr;
}


int comparator(const void * arg1, const void *arg2)
{
	return ((Man*)arg1)->w < ((Man*)arg2)->w;
}

template <typename T>
void LightArray<T>::Sort(int comparator(const void*, const void*), int b, int e )
{
	int l = b, r = e;
	T piv = ptr[(l + r) / 2];
	while (l <= r)
	{
		while (comparator(&ptr[l], &piv))
			l++;
		while (comparator(&piv, &ptr[r]))
			r--;
		if (l <= r)
		{
			std::swap(ptr[l], ptr[r]);
			l++, r--;
		}
	}
	if (b < r)
		Sort(comparator, b, r);
	if (e > l)
		Sort(comparator, l, e);
}
/*
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
*/

void GetMans(LightArray<Man> &mans)// **arr)
{
	int w, s;
	while (scanf("%d %d", &w, &s) == 2)
		mans.PushBack({ w,s });
}

int GetHeight(LightArray<Man> &mans)//Man* arr, int n)
{
	mans.Sort(comparator, 0, mans.Count() - 1);
	int cnt = 1;
	int totalW = mans[0].w;
#ifdef TEST
	printf("%d %d\n", mans[0].w, mans[0].s);
#endif
	for (int i = 1; i < mans.Count(); i++)
	{
		if (totalW <= mans[i].s)
		{
			totalW += mans[i].w;
			cnt++;
		}
#ifdef TEST
		printf("%d %d\n", mans[i].w, mans[i].s);
#endif
	}

	return cnt;
}

int main()
{
	try 
	{
		Man *arr;
		LightArray<Man> mans;
		GetMans(mans);//&arr);
		int res = GetHeight(mans);
		printf("%d", res);
	}
	catch (Exception &e)
	{
		printf("%s", e.GetMessage());
	}
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
/*
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


/*
int comp(const void *arg1, const void *arg2)
{
Man m1 = *(Man*)arg1;
Man m2 = *(Man*)arg2;
return m1.w - m2.w;
}

*
if (arr[j].w > arr[j+1].w)//comp((void*)&arr[j], (void*)&arr[j+1]))
{
Man temp = arr[j];
arr[j] = arr[j+1];
arr[j+1] = temp;
}
}
}*/