#include <stdio.h>
#include <stdlib.h>
#define MEM_ERR -1
#define MEM_SCS 1
//#define TEST


typedef struct Rect
{
	int w, h, x0;
} Rect, T, *pT;

typedef struct stack
{
	struct stack * prev;
	pT data;
} stack;

int Push(stack **stackHead, pT data)
{
	stack *stackElement = (stack*)malloc(sizeof(stack));
	if (!stackElement) return MEM_ERR;
	stackElement->prev = *stackHead;
	stackElement->data = data;
	*stackHead = stackElement;
	return MEM_SCS;
}

pT Pop(stack **stackHead)
{
	stack *stackElement = *stackHead;
	pT data = stackElement->data;
	*stackHead = (*stackHead)->prev;
	free(stackElement);
	return data;
}

void FreeStack(stack *head)
{
	while (head)
	{
		stack *temp = head;
		head = head->prev;
		free(temp);
	}
}

int GetMaxS()
{
	FILE *f;
#ifdef TEST
	f = fopen("C:\\users\\bigbrother\\desktop\\t.txt", "r");
#else
	f = stdin;
#endif
	int n;
	stack *rectangles = NULL;
	int left = -1;
	int right = 0;
	int w, h;
	int s, maxS = -1;
	fscanf(f, "%d", &n);

	for (int i = 0; i < n; i++)
	{
		// Get new h and w
		fscanf(f, "%d %d", &w, &h);
		if (!rectangles || rectangles->data->h < h)
		{
			// Push data to stack
			pT rect = (pT)malloc(sizeof(T));
			if (!rect)
			{
				FreeStack(rectangles);
				return MEM_ERR;
			}
			rect->h = h;
			rect->w = w;
			rect->x0 = right;
			if (Push(&rectangles, rect) == MEM_ERR)
			{
				FreeStack(rectangles);
				return MEM_ERR;
			}
		}
		else
		{
			// We need to pop all rectangles with h >= input h and check S
			int prevX0 = 0, prevW = 0, prevH;
			while (rectangles->data->h >= h)
			{
				pT rect = Pop(&rectangles);
				prevW += rect->w;
				prevX0 = rect->x0;
				prevH = rect->h;
				int width = right - prevX0;
				s = width * prevH;
				if (s > maxS) maxS = s;
				free(rect);
				if (!rectangles) break;
			}
			pT rect = (pT)malloc(sizeof(T));
			if (!rect)
			{
				FreeStack(rectangles);
				return MEM_ERR;
			}
			rect->h = h;
			rect->w = w + prevW;
			rect->x0 = prevX0;
			if (Push(&rectangles, rect) == MEM_ERR)
			{
				FreeStack(rectangles);
				return MEM_ERR;
			}
		}
		right += w; // right end of interval (or total width)
	}

	// if stack not empty we need to pop all recangles in stack
	if (rectangles)
	{
		while (rectangles != NULL)
		{
			pT rect = Pop(&rectangles);
			int h = rect->h;
			int width = right - rect->x0;
			s = width * rect->h;
			if (s > maxS) maxS = s;
			free(rect);
		}
	}
	return maxS;
}

int main()
{
	int res = GetMaxS();
	if (res == MEM_ERR)
	{
		return 0;
	}

	printf("%d", res);
#ifdef TEST
	getchar();
	getchar();
#endif
}