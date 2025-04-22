#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int heap[], int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[left] < heap[smallest])
        smallest = left;

    if (right < n && heap[right] < heap[smallest])
        smallest = right;

    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        heapify(heap, n, smallest);
    }
}

void createHeap(int heap[], int array[], int k) {
    for (int i = 0; i < k; i++) {
        heap[i] = array[i];
    }
    for (int i = k / 2 - 1; i >= 0; i--) {
        heapify(heap, k, i);
    }
}

int getSmallest(int heap[]) {
    return heap[0];
}

void removeSmallest(int heap[], int *k) {
    swap(&heap[0], &heap[*k - 1]);
    (*k)--;
    heapify(heap, *k, 0);
}

void insertHeap(int heap[], int *k, int value) {
    heap[*k] = value;
    (*k)++;
    for (int i = (*k / 2) - 1; i >= 0; i--) {
        heapify(heap, *k, i);
    }
}

int kthLargest(int array[], int n, int k) {
    int heap[k];
    int heapSize = k;
    createHeap(heap, array, k);

    for (int i = k; i < n; i++) {
        if (array[i] > getSmallest(heap)) {
            removeSmallest(heap, &heapSize);
            insertHeap(heap, &heapSize, array[i]);
        }
    }
    return getSmallest(heap);
}

void generateRandomArray(int array[], int n) {
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 100;
    }
}

void extractAndValidate(int array[], int n) {
    printf("Original array: [");
    for (int i = 0; i < n; i++) {
        printf("%d%s", array[i], i == n - 1 ? "]\n" : ", ");
    }

    int *tempArray = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        tempArray[i] = array[i];
    }

    printf("Extracted elements: [");
    for (int i = 0; i < n; i++) {
        int largest = kthLargest(tempArray, n - i, 1);
        printf("%d%s", largest, i == n - 1 ? "]\n" : ", ");
        tempArray[n - i - 1] = tempArray[0]; 
    }

    free(tempArray);
}

void validateCorrectness(int numTests, int arraySize) {
    for (int test = 0; test < numTests; test++) {
        int *array = (int *)malloc(arraySize * sizeof(int));
        generateRandomArray(array, arraySize);
        extractAndValidate(array, arraySize);
        free(array);
        printf("\n");
    }
}

double get_time_usec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec * 1000000.0) + t.tv_usec;
}

double calculate_upper_bound(int n) {
    return n;
}

double calculate_tight_bound(int n) {
    return n * log2(n);
}

double calculate_lower_bound(int n) {
    return n * sqrt(n);
}

double calculate_constant_for_tight(double total_time, int n) {
    return total_time / calculate_tight_bound(n);
}

void measure_complexity(int max_size, int iterations) {
    printf("Results for k=9:\n");
    printf("n\tt(n)\tUpper\tTight\tLower\n");
    double constant_k9 = 0;

    for (int n = 100; n <= max_size; n *= 2) {
        int *array = (int *)malloc(n * sizeof(int));
        double execution_time_k9 = 0;

        if (n < 500) {
            double start = get_time_usec();
            for (int iter = 0; iter < iterations; iter++) {
                generateRandomArray(array, n);
                kthLargest(array, n, 9);
            }
            double end = get_time_usec();
            double full_time = end - start;

            start = get_time_usec();
            for (int iter = 0; iter < iterations; iter++) {
                generateRandomArray(array, n);
            }
            end = get_time_usec();
            double init_time = end - start;

            execution_time_k9 = (full_time - init_time) / iterations;
        } else {
            double start = get_time_usec();
            generateRandomArray(array, n);
            kthLargest(array, n, 9);
            double end = get_time_usec();
            execution_time_k9 = end - start;
        }

        double upper = calculate_upper_bound(n);
        double tight = calculate_tight_bound(n);
        double lower = calculate_lower_bound(n);

        if (n == max_size) {
            constant_k9 = calculate_constant_for_tight(execution_time_k9, n);
        }

        printf("%d\t%.3f\t%.6f\t%.6f\t%.6f\n",
               n, execution_time_k9,
               execution_time_k9 / upper,
               execution_time_k9 / tight,
               execution_time_k9 / lower);

        free(array);
    }

    printf("\nConstant for Tight Bound (k=9): %.6f\n", constant_k9);

    printf("\nResults for k=n/2:\n");
    printf("n\tt(n)\tUpper\tTight\tLower\n");
    double constant_kn2 = 0;

    for (int n = 100; n <= max_size; n *= 2) {
        int *array = (int *)malloc(n * sizeof(int));
        double execution_time_kn2 = 0;

        if (n < 500) {
            double start = get_time_usec();
            for (int iter = 0; iter < iterations; iter++) {
                generateRandomArray(array, n);
                kthLargest(array, n, n / 2);
            }
            double end = get_time_usec();
            double full_time = end - start;

            start = get_time_usec();
            for (int iter = 0; iter < iterations; iter++) {
                generateRandomArray(array, n);
            }
            end = get_time_usec();
            double init_time = end - start;

            execution_time_kn2 = (full_time - init_time) / iterations;
        } else {
            double start = get_time_usec();
            generateRandomArray(array, n);
            kthLargest(array, n, n / 2);
            double end = get_time_usec();
            execution_time_kn2 = end - start;
        }

        double upper = calculate_upper_bound(n);
        double tight = calculate_tight_bound(n);
        double lower = calculate_lower_bound(n);

        if (n == max_size) {
            constant_kn2 = calculate_constant_for_tight(execution_time_kn2, n);
        }

        printf("%d\t%.3f\t%.6f\t%.6f\t%.6f\n",
               n, execution_time_kn2,
               execution_time_kn2 / upper,
               execution_time_kn2 / tight,
               execution_time_kn2 / lower);

        free(array);
    }

    printf("\nConstant for Tight Bound (k=n/2): %.6f\n", constant_kn2);
}

int main() {
    srand(time(NULL));
    measure_complexity(6400, 2000);


    printf("\nValidation Tests:\n");
    validateCorrectness(5, 10);

    return 0;
}
