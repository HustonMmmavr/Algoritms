// ConsoleApplication15.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
class Stack
{
	T* ptr;
	int stackBufferSize;
	int stackSize;
public:
	Stack();
	Stack(int size);
	~Stack() { delete[] ptr; }

	void Realloc();
	void Push(const T& stackElement);
	T Pop();
	int Count();
	bool IsEmpty();
};

template <typename T>
Stack<T>::Stack()
{
	ptr = NULL;
	stackBufferSize = 2;
	stackSize = 0;
	ptr = new T[stackBufferSize];
	if (!ptr)
		ThrowException("Error memory");
}

template <typename T>
Stack<T>::Stack(int size)
{
	ptr = NULL;
	this->stackBufferSize = size;
	stackSize = 0;
	ptr = new T[stackBufferSize];
	if (!ptr)
		ThrowException("Error memory");
}

template <typename T>
void Stack<T>::Realloc()
{
	int pow2 = log2(stackSize) + 1;
	int newSize = 1 << pow2;
	T* newPtr = new T[newSize];
	if (!newPtr) ThrowException("Cant realloca");
	memcpy(newPtr, ptr, stackSize * sizeof(T));
	stackBufferSize = newSize;
	delete[] ptr;
	ptr = newPtr;
}

template <typename T>
void Stack<T>::Push(const T& stackElement)
{
	if (stackSize == stackBufferSize)
		Realloc();
	ptr[stackSize++] = stackElement;
}

template <typename T>
T Stack<T>::Pop()
{
	return ptr[--stackSize];
}

template <typename T>
int Stack<T>::Count()
{
	return stackSize;
}

template <typename T>
bool Stack<T>::IsEmpty()
{
	return stackSize == 0;
}

// class defenition
template <typename T>
class Queue
{
public:
	Queue();
	~Queue();
	void Push(const T& data);
	T Pop();
	bool Empty();
	void CheckRecopy();
	void CheckNormal();
	void AdditionalOperations();
	//bool IsEmpty() const;

private:
	Stack<T> *left;
	Stack<T> *right;
	Stack<T> *leftAdditional;
	Stack<T> *rightCopy;
	Stack<T>* rightCopyAdditional;
	Stack<T> *s;
	long toCopy;
	bool copied;
	bool empty;
	long size;
	bool recopy;
};


///////////////////////////////////////////////////////////////////////////////////
// Realisation
template <typename T>
Queue<T>::Queue()
{
	toCopy = 0;
	copied = 0;
	empty = true;
	recopy = false;
	left = new Stack<T>();
	if (!left) ThrowException("error allocate left");
	right = new Stack<T>();
	if (!right) ThrowException("error allocate right");
	s = new Stack<T>();
	if (!s) ThrowException("error allocate s");
	leftAdditional = new Stack<T>();
	if (!leftAdditional) ThrowException("error allocate leftAdditional");
	rightCopy = new Stack<T>();
	if (!rightCopy)  ThrowException("error allocate left");
	rightCopyAdditional = new Stack<T>();
}

template <typename T>
Queue<T>::~Queue()
{
	delete left;
	delete right;
	delete s;
	delete leftAdditional;
	delete rightCopy;
	delete rightCopyAdditional;
}

template <typename T>
bool Queue<T>::Empty()
{
	return !recopy && right.Empty();
}

template <typename T> 
void Queue<T>::Push(const T& data)
{
	if (!recopy)
	{
		left->Push(data);
		if (rightCopyAdditional->Count() > 0)
			rightCopyAdditional->Pop();
		CheckRecopy();
	}
	else
	{
		leftAdditional->Push(data);
		CheckNormal();
	}
}


template <typename T>
T Queue<T>::Pop()
{
	if (!recopy)
	{
		if (right->IsEmpty()) return -1;
		T temp = right->Pop();
		rightCopy->Pop();
		if (rightCopyAdditional->Count() > 0)
			rightCopyAdditional->Pop();
		CheckRecopy();
		return temp;
	}
	else
	{
		if (rightCopy->IsEmpty()) return -1;
		T temp = rightCopy->Pop();
		if (toCopy > 0)
			toCopy--;
		else
		{
			right->Pop();
			rightCopyAdditional->Pop();
		}
		CheckNormal();
		return temp;
	}
}

template <typename T>
void Queue<T>::CheckRecopy()
{
	recopy = left->Count() > right->Count();
	if (recopy)
	{
		toCopy = right->Count();
		copied = false;
		CheckNormal();
	}
}

template <typename T>
void Queue<T>::CheckNormal()
{
	AdditionalOperations();
	recopy = !s->IsEmpty();// s->Count() != 0;
}

template <typename T>
void Queue<T>::AdditionalOperations()
{
	int operationsCount = 3;

	while (!copied && operationsCount > 0 && right->Count() > 0)
	{
		s->Push(right->Pop());
		operationsCount--;
	}

	while (operationsCount > 0 && left->Count() > 0)
	{
		copied = true;
		T temp = left->Pop();
		right->Push(temp);
		rightCopyAdditional->Push(temp);
		operationsCount--;
	}

	while (operationsCount > 0 && s->Count() > 0)
	{
		T temp = s->Pop();
		if (toCopy > 0)
		{
			right->Push(temp);
			rightCopyAdditional->Push(temp);
			toCopy--;
		}
		operationsCount--;
	}
	if (s->Count() == 0)
	{
		Stack<T> *temp = left;
		left = leftAdditional;
		leftAdditional = temp;

		temp = rightCopy;
		rightCopy = rightCopyAdditional;
		rightCopyAdditional = temp;
	}
}
bool Controller(Queue<int> &q)
{
	FILE *f;
#ifdef TEST
	f = fopen("C:\\test.txt", "r");
#else
	f = stdin;
#endif
	long countCommands;
	fscanf(f, "%ld", &countCommands);
	long a, b;

	bool flag = true;

	for (long i = 0; i < countCommands && flag; i++)
	{
		fscanf(f, "%ld %ld", &a, &b);
		//printf("%ld %ld\n", a, b);
		switch (a)
		{
		case 2:
			flag = flag && (q.Pop() == b);
			break;
		case 3:
			q.Push(b);
			break;
		}
	}

	return flag;
}

int main()
{
	Queue<int> q;
	if (Controller(q))
		printf("YES");
	else
		printf("NO");

#ifdef TEST
	for (int i = 0; i < 100; i++)
		q.Push(i);
	for (int i = 0; i < 50; i++)
		printf("%d\n", q.Pop());

	for (int i = 200; i < 10000; i++)
		q.Push(i);

	for (int i = 0; i < 10050; i++)
		printf("%d\n", q.Pop());
	getchar();
	getchar();
#endif
    return 0;
}

