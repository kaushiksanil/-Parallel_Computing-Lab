#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 10000000

// Merge function
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L); free(R);
}

// Sequential Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parallelMergeSort(int arr[], int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (depth <= 4) {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, l, m, depth + 1);
                #pragma omp section
                parallelMergeSort(arr, m + 1, r, depth + 1);
            }
        } else {
            parallelMergeSort(arr, l, m, depth + 1);
            parallelMergeSort(arr, m + 1, r, depth + 1);
        }

        merge(arr, l, m, r);
    }
}

// Utility to copy array
void copyArray(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

int main() {
    int *original = malloc(SIZE * sizeof(int));
    int *seqArr = malloc(SIZE * sizeof(int));
    int *parArr = malloc(SIZE * sizeof(int));

    // Generate random array
    for (int i = 0; i < SIZE; i++) original[i] = rand() % 100000;

    // Sequential Sort
    copyArray(original, seqArr, SIZE);
    clock_t start_seq = clock();
    mergeSort(seqArr, 0, SIZE - 1);
    clock_t end_seq = clock();
    double time_seq = ((double)(end_seq - start_seq)) / CLOCKS_PER_SEC;

    // Parallel Sort
    copyArray(original, parArr, SIZE);
    double start_par = omp_get_wtime();
    parallelMergeSort(parArr, 0, SIZE - 1, 0);
    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    // Results
    printf("Sequential Merge Sort Time: %f seconds\n", time_seq);
    printf("Parallel Merge Sort Time:   %f seconds\n", time_par);

    free(original);
    free(seqArr);
    free(parArr);
    return 0;
}

