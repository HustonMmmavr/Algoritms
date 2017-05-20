#include <stdio.h>
#include <stdlib.h>

typedef int t;
void reverse(int n, t *arr)
{
    int half_n = n / 2;
    for (int i  = 0; i < half_n; i++)
    {
        t temp = arr[i];
        arr[i] = arr[n - i - 1];
        arr[n - i - 1] = temp;
    }
}

int main()
{
    int n;
    scanf("%d", &n);
    t *arr = (t*)malloc(n*sizeof(t));
    if(!arr)
    {
        printf("Error allocate");
        return 0;
    }
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    reverse(n, arr);
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    free(arr);
    return 0;

}
