#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

typedef long T;
//template <typename T>
class Stack
{
	T* ptr;
	int stackBufferSize;
	int stackSize;
public:
	Stack()
	{
		ptr = NULL;
		stackBufferSize = 2;
		stackSize = 0;
		ptr = new T[stackBufferSize];
		if (!ptr)
			ThrowException("Error memory");
	}

	Stack(int size)
	{
		ptr = NULL;
		this->stackBufferSize = size;
		stackSize = 0;
		ptr = new T[stackBufferSize];
		if (!ptr)
			ThrowException("Error memory");
	}

	~Stack()
	{
		delete[] ptr;
	}

	void Realloc()
	{
		int pow2 = log2(stackSize) + 1;
		int newSize = 1 << pow2;//2 << pow2; // 1 << 
		T* newPtr = new T[newSize];
		if (!newPtr) ThrowException("Cant realloca");
		memcpy(newPtr, ptr, stackSize * sizeof(T));
		stackBufferSize = newSize;
		delete[] ptr;
		ptr = newPtr;
	}

	void Push(T stackElement)
	{
		if (stackSize == stackBufferSize)
			Realloc();

		ptr[stackSize++] = stackElement;
	}

	T Pop()
	{
		//if (stackSize == 0)
		//    return -1;
		//stackSize--;
		return ptr[--stackSize];
	}

	int Count()
	{
		return stackSize;
	}

	bool IsEmpty()
	{
		return stackSize == 0;
	}
};


//template <typename T>
class Queue
{
	Stack *left, *right;
	int countElements;
public:
	Queue()
	{
		left = new Stack();
		if (!left) ThrowException("Error new Stack");
		right = new Stack();
		if (!right) ThrowException("Error new Stack");
	}

	void Push(T queueElement)
	{
		left->Push(queueElement);
	}

	T Pop()
	{
		if (right->IsEmpty())
		{
			if (left->IsEmpty())
				return -1;
			else
			{
				while (left->Count() > 0)
					right->Push(left->Pop());
				//return right->Pop();
			}
		}
		//else
		//{
		return right->Pop();
		//}

	}
};


bool Controller(Queue &q)
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
	try
	{
		Queue q;
		if (Controller(q))
			printf("YES");
		else
			printf("NO");

	}
	catch (Exception e)
	{
		printf("%s", e.GetMesssage());
	}
}
