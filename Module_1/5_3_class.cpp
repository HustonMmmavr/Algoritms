// ConsoleApplication10.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
//#define TEST

// Exception 
class Exception
{
	char message[1024];
public:
	Exception() {}
	Exception(const char *message, const char* file, const char* func, int line);
	const char* GetMesssage() const;
};

// Exception Realisation
Exception::Exception(const char *message, const char* file, const char* func, int line)
{
	sprintf(this->message, "In file %s func %s line %d message %s", file, func, line, message);
}

const char* Exception::GetMesssage() const
{
	return message;
}

#define ThrowException(msg)	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);

template <typename T>
struct Node
{
	Node *next;
	T data;
};

// class Stack
template <typename T>
class Stack
{
	Node<T> *head;
	int elementsCount;
	T elementData;
public:
	Stack() : head(NULL), elementsCount(0) {}
	Stack(const Stack *prev);
	~Stack();
	void Push(const T & data);
	T Pop();
	bool IsEmpty() const;
	T Top() const;
	int Count() const;
};


template <typename T>
Stack<T>::~Stack()
{
	while (head)
	{
		Node<T> *t = head;
		head = head->next;
		delete t;
	}
}

template <typename T>
Stack<T>::Stack(const Stack<T> *previous)
{
}

template <typename T>
void Stack<T>::Push(const T& data)
{
	Node<T> *node = new Node<T>();
	if (!node)
		ThrowException("Cant allocat nod");
	node->next = head;
	node->data = data;
	head = node;
	elementsCount++;
}

template <typename T>
T Stack<T>::Pop()
{
	if (IsEmpty())
		ThrowException("No elements");
	Node<T> *node = head;
	head = head->next;
	T res = node->data;
	delete node;
	elementsCount--;
	return res;
}

template <typename T>
T Stack<T>::Top() const
{
	return head->data;
}

template <typename T>
bool Stack<T>::IsEmpty() const
{
	return elementsCount == 0;
}

template <typename T>
int Stack<T>::Count() const
{
	return elementsCount;
}

/////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;

struct Rect
{
	int w, h, x0;
};

vector<Rect> ReadRectangles()
{
	int count;
	scanf("%d", &count);
	int right = 0;
	vector<Rect> rectangles;
	for (int i = 0; i < count; i++)
	{
		int w, h;
		scanf("%d %d", &w, &h);
		Rect rect = { w, h, right };
		right += w;
		rectangles.push_back(rect);
	}
	return rectangles;
}

int GetMaxS(const vector<Rect> &rectangles)
{
	Stack<Rect> rectanglesStack;
	int count = rectangles.size();
	int prevW = 0;
	int right = 0;
	int maxS = 0;

	for (int i = 0; i < count;)
	{
		Rect rect = rectangles[i];
		if (rectanglesStack.IsEmpty() || rectanglesStack.Top().h < rect.h)
		{
			rect.x0 = right - prevW;
			right += rect.w;
			rect.w += prevW;
			rectanglesStack.Push(rect);
			i++;
			prevW = 0;
		}
		else
		{
			Rect popRect = rectanglesStack.Pop();
			prevW += popRect.w;
			int width = right - popRect.x0;
			int s = width * popRect.h;
			if (s > maxS) maxS = s;
		}
	}

	while (!rectanglesStack.IsEmpty())
	{
		Rect popRect = rectanglesStack.Pop();
		int width = right - popRect.x0;
		int s = width * popRect.h;
		if (s > maxS) maxS = s;
	}

	return maxS;
}
int main()
{
	try
	{
		vector<Rect> rectangles = ReadRectangles();
		int res = GetMaxS(rectangles);
		printf("%d", res);
#ifdef TEST
		getchar();
		getchar();
#endif
	}
	catch (Exception &e)
	{
		printf("%s", e.GetMesssage());
	}
}
