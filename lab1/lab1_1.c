#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int* genRandArray(int size, int maxValue);

void print(int* arr);

int* genRandArray(int size, int maxValue)
{
    int* arr = (int*)malloc(sizeof(int) * (size + 1));
    arr[0] = size;
    for (int i = 1; i <= size; i++) {
        arr[i] = rand() % maxValue+1;
    }
    return arr;
}

void print(int* arr)
{
    printf("%d: ", arr[0]);
    for (int i = 1; i <= arr[0]; i++) {
        printf("%d ", arr[i]);
    }
}

int main()
{
    srand(time(NULL));
    int size = rand() % 10;
    int maxValue = 100;
    int* arr = genRandArray(size, maxValue);
    print(arr);

    return 0;
}