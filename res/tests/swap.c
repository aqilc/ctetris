#include <stdio.h>
// gcc swap.c -std=c17 -o swap.exe

void swap(int *a, int *b)
{
    int temp = a;
    a = b;
    b = temp;
}

int main()
{
    int i, j;
    scanf("%d%d", &i, &j);


    printf("Before swap: %d %d\n", i, j);
    swap(&i, &j);
    printf("After swap: %d %d", i, j);
}

