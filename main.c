#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_THREADS 4

int *arr;                            // Shared array
int  partial_sum[MAX_THREADS] = {0}; // Store partial results per thread
int  num_threads;                    // Number of active threads

int N; // Array size
int m; // Modular base

// Timer function
double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

// Serial work (non-parallelizable)
void serial_work(int N, int m) {
    arr[0] = 1;
    arr[1] = 1;
    for (int i = 2; i < N; i++) {
        arr[i] = (arr[i - 2] + arr[i - 1]) % m;
    }
}

// Parallel work function (executed by threads)
void *parallel_work(void *arg) {
    int thread_id = *(int *)arg;

    //// TODO: Calculate start and end indices for each thread

    ////

    int sum = 0;

    //// TODO: Compute partial sum of array elements assigned to this thread
    //// partial sum should be taken modulo 'm' in every step

    ////

    //// TODO: Store partial sum in the global array

    ////

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s : Needs 3 arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]); // Array size
    m = atoi(argv[2]); // Modular base

    if ((N % 4) != 0) {
        fprintf(stderr, "N must be divided by 4\n");
        exit(EXIT_FAILURE);
    }

    if ((N < 1000) || (N > 100000000)) {
        fprintf(stderr, "N must be 1,000 <= N <= 100,000,000\n");
        exit(EXIT_FAILURE);
    }

    if (m <= 1) {
        fprintf(stderr, "m must be a positive integer greater than 1\n");
        exit(EXIT_FAILURE);
    }

    arr = (int *)malloc(N * sizeof(int));

    // This `serial_work` call may look redundant, but it helps maintain the
    // stability of the cache. Please do not remove it unless you intent
    // to conduct further experimentation.
    serial_work(N, m);

    double T1, TN;

    // Test with different thread counts
    for (num_threads = 1; num_threads <= MAX_THREADS; num_threads *= 2) {
        pthread_t threads[num_threads];
        int       thread_ids[num_threads];

        // Start timing
        double start_time = get_time();

        // Call serial section
        serial_work(N, m);

        //// TODO: Create threads and execute parallel_work function (Hint: use pthread_create)

        ////

        //// TODO: Join threads after execution (Hint: use pthread_join)

        ////

        // Aggregate results
        int total_sum = 0;

        //// TODO: Sum up the partial results from all threads

        ////

        // This line is necessary to prevent overflow and ensure correctness
        total_sum = ((int) total_sum) % m;

        // End timing
        double end_time = get_time();
        TN              = end_time - start_time;

        if (num_threads == 1) {
            T1 = TN;
        }

        // Calculate speedup
        double speedup = T1 / TN;
        printf("Total sum: %d, Threads: %d, Time: %f sec, Speedup: %f\n", total_sum, num_threads, TN,
               speedup);
    }

    free(arr);
    return 0;
}
