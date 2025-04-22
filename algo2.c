#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void ins_sort(int v[], int n) {
    for (int i = 1; i < n; i++) {
        int x = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > x) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = x;
    }
}

void quick_sort_aux(int v[], int left, int right) {
    if (left < right) {
        int x = left + rand() % (right - left + 1); 
        int pivot = v[x];
        swap(&v[left], &v[x]); 
        int i = left + 1, j = right;

        while (i <= j) {
            while (i <= right && v[i] < pivot) i++;
            while (v[j] > pivot) j--;
            if (i <= j) {
                swap(&v[i], &v[j]);
                i++;
                j--;
            }
        }
        swap(&v[left], &v[j]); 
        quick_sort_aux(v, left, j - 1);  
        quick_sort_aux(v, j + 1, right); 
    }
}

void quick_sort(int v[], int n) {
    quick_sort_aux(v, 0, n - 1);
}

void init_seed() {
    srand(time(NULL)); 


void random_init(int v[], int n) {
    int m = 2 * n + 1;
    for (int i = 0; i < n; i++) {
        v[i] = (rand() % m) - n; 
    }
}


void ascending_init(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = i; 
    }
}

void descending_init(int v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = n - i - 1; 
    }
}

int main() {
    init_seed(); 

    int v[] = {3, -3, 0, 17, -5, 2, 11, 13, 6, 1, 7, 14, 1, -2, 5, -14, -2};
    int n = sizeof(v) / sizeof(v[0]);


    printf("Original array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");


    ins_sort(v, n);
    printf("\nSorted array (Insertion Sort):\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    random_init(v, n);
    printf("\nRandomly reinitialized array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    quick_sort(v, n);
    printf("\nSorted array (Quicksort):\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    return 0;
}
