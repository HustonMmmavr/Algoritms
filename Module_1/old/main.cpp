#include <stdio.h>
#include <stdlib.h>
#define MEM_ERR -1
#define MEM_SCS 1
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

int GetMaxS()
{
    int n;
    stack *rectangles = NULL;
    int left = -1;
    int right;
    int w, h;
    int s, maxS = -1;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
    {
        scanf("%d %d", &w, &h);
        if (!rectangles)
        {
            pT rect = (pT)malloc(sizeof(T));
            if (!rect) return MEM_ERR;
            rect->h = h;
            rect->w = w;
            rect->x0 = 0;
            if (Push(&rectangles ,rect) == MEM_ERR) return MEM_ERR;
        }
        else
        {
            if (rectangles->data->h <= h)
            {
                pT rect = (pT)malloc(sizeof(T));
                if (!rect) return MEM_ERR;
                rect->h = h;
                rect->w = w;
                left = rectangles->data->x0;
                rect->x0 = rectangles->data->x0 + rectangles->data->w;
                if (Push(&rectangles ,rect) == MEM_ERR) return MEM_ERR;
            }
            else
            {
                left = left == -1 ? 0 : left;
                right = rectangles->data->x0 + rectangles->data->w;
                printf("%d\n", right);
                while(rectangles)
                {
                    pT rect = Pop(&rectangles);
                    int width = right - rect->x0;
                    s = width * rect->h;
                    if (s > maxS) maxS = s;
                    delete rect;
                }
            }
        }

        if (rectangles)
        {
            right = rectangles->data->x0 + rectangles->data->w;
            printf("%d\n", right);
            while(rectangles)
            {
                pT rect = Pop(&rectangles);
                int width = right - rect->x0;
                s = width * rect->h;
                if (s > maxS) maxS = s;
                delete rect;
            }
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
}
