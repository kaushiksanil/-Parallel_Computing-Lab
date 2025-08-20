#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Sequential Fibonacci
long long fib_seq(int n) {
    if (n <= 1) return n;
    return fib_seq(n - 1) + fib_seq(n - 2);
}

// Parallel Fibonacci
long long fib_par(int n) {
    if (n <= 1) return n;
    long long x, y;

    if (n < 30) {
        // below a threshold, don’t parallelize (to avoid overhead)
        return fib_par(n - 1) + fib_par(n - 2);
    }

    #pragma omp task shared(x)
    x = fib_par(n - 1);

    #pragma omp task shared(y)
    y = fib_par(n - 2);

    #pragma omp taskwait
    return x + y;
}

int main() {
    int n;
    printf("Enter n (Fibonacci term to compute): ");
    scanf("%d", &n);

    // Sequential
    clock_t start_seq = clock();
    long long res_seq = fib_seq(n);
    clock_t end_seq = clock();
    double time_seq = ((double)(end_seq - start_seq)) / CLOCKS_PER_SEC;

    // Parallel
    double start_par = omp_get_wtime();
    long long res_par;
    #pragma omp parallel
    {
        #pragma omp single
        res_par = fib_par(n);
    }
    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    printf("Sequential Fibonacci(%d): %lld, Time: %f sec\n", n, res_seq, time_seq);
    printf("Parallel   Fibonacci(%d): %lld, Time: %f sec\n", n, res_par, time_par);

    return 0;
}
