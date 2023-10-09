#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x*1000)
#endif

int** genRandMatrix(int size, int maxValue);
void printMatrix(int** matrix,int size);

int** genRandMatrix(int size, int maxValue)
{
    int** matrix = (int**)malloc(sizeof(int*) * (size));
    int colsize;
    for (int i = 0; i < size; i++) {
        colsize = rand() % 10;
        matrix[i] = (int*)malloc(sizeof(int) * (colsize+1));
        matrix[i][0] = colsize;
        sleep(1);
        srand(time(NULL));
    }
    for (int i = 0; i < size; i++) {
        for (int j = 1; j <= matrix[i][0]; j++) {
            matrix[i][j] = rand() % maxValue + 1;
        }
    }
    return matrix;
}

void printMatrix(int** matrix,int size)
{
    printf("%d\n",size);
    for (int n = 0;n<size;n++){
        printf("%d: ", matrix[n][0]);
        for (int i = 1; i <= matrix[n][0]; i++) {
            printf("%d ", matrix[n][i]);
        }
        printf("\n");        
    }

}


int main()
{
	srand(time(NULL));
    int size = rand() % 10;
	int maxValue = 100;
	int** matrix = genRandMatrix(size, maxValue);
	printMatrix(matrix, size);
	return 0;
}