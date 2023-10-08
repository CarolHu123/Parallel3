#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <MAX_NUMBER> <NUM_THREADS>\n", argv[0]);
        return 1;
    }
    
    long MAX_NUMBER = atol(argv[1]);
    long NUM_THREADS = atol(argv[2]);
    
    printf("%ld %ld\n", MAX_NUMBER, NUM_THREADS); 
    
    long n = sqrt(MAX_NUMBER);
    // Allocate memory for the primes array and initialize it
    long* primes = (long*)malloc((MAX_NUMBER + 1) * sizeof(long));
    if (primes == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    for (long i = 0; i <= MAX_NUMBER; i++) {
        primes[i] = 0; // Initialize all numbers as potentially prime
    }

    // Set the number of OpenMP threads
    omp_set_num_threads(NUM_THREADS);
    omp_set_nested(1);//设置支持嵌套并行


    // Parallelize the Sieve of Eratosthenes algorithm
    double start_time = omp_get_wtime();
    #pragma omp parallel for num_threads(NUM_THREADS) 
    for (long i = 2; i <= n; i++) {
        if (primes[i] == 0) { // Check if i is marked as prime
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (long j = i * i; j <= MAX_NUMBER; j += i) {
                primes[j] = 1; // Mark non-prime numbers
                printf("  j=%ld I'm thread %d\n", j, omp_get_thread_num() );
            }
        }
        printf("i=%ld I'm thread %d\n", i, omp_get_thread_num() );
    }
    
    double run_time = omp_get_wtime() - start_time;
    printf("the running time is %.5f\n",run_time);


    // Output prime numbers
/*    printf("Prime numbers up to %ld:\n", MAX_NUMBER);
    for (int i = 100000; i <= 101456; i++) {
        if (primes[i] == 0) {
            printf("%d ", i);
        }
    }
    printf("\n");
*/
    // Calculate and print execution time
    // printf("Execution time: %.4f seconds\n", execution_time);

    // Free allocated memory
    free(primes);

    return 0;
}
