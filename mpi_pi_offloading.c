#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// Defina a proporção da carga de trabalho para a GPU (em %)
#define GPU_WORKLOAD 70

static long num_trials = 10000000000; // Número total de pontos aleatórios

int main(int argc, char *argv[])
{
    long int i, local_count_gpu = 0, local_count_cpu = 0, global_count = 0;
    int rank, size, provided;
    double x, y, z, pi;
    double start_time, run_time;

    // Inicializa o MPI com suporte para threads
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    if (provided < MPI_THREAD_FUNNELED) {
        printf("Nível de suporte para threads não é suficiente!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // O rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // O número de processos

    // Inicializa o gerador de números aleatórios
    srand(time(NULL) + rank);

    // Define a carga de trabalho para a GPU e para as threads CPU
    long int gpu_trials = num_trials * GPU_WORKLOAD / 100;
    long int cpu_trials = num_trials - gpu_trials;

    start_time = omp_get_wtime(); // Tempo de início da execução

    // Offloading com OpenMP para a GPU - Master thread
#pragma omp parallel num_threads(8)
    {
        // Thread master para comunicação e offloading para a GPU
        #pragma omp master
        #pragma omp target data map(tofrom:local_count_gpu) device(1)
        #pragma omp target teams distribute parallel for nowait reduction(+:local_count_gpu)
            for (i = rank; i < gpu_trials; i += size) {
                x = (double)rand() / RAND_MAX;
                y = (double)rand() / RAND_MAX;
                z = x * x + y * y;
                if (z <= 1.0) local_count_gpu++; // Verifica se o ponto está no círculo
            }
            // Threads secundárias para execução no CPU
        #pragma omp for reduction(+:local_count_cpu)
            for (i = rank; i < cpu_trials; i += size) {
                x = (double)rand() / RAND_MAX;
                y = (double)rand() / RAND_MAX;
                z = x * x + y * y;
                if (z <= 1.0) local_count_cpu++;
            }
    }

    // MPI_Reduce para somar os resultados de todos os processos MPI
    long int local_count_total = local_count_gpu + local_count_cpu;
    MPI_Reduce(&local_count_total, &global_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi = ((double)global_count / (double)num_trials) * 4.0; // Estimação de Pi
        run_time = omp_get_wtime() - start_time;
        printf("pi = %3.15f, %ld amostras, computados em %lf segundos\n", pi, num_trials, run_time);
    }

    MPI_Finalize(); // Finaliza o MPI
    return 0;
}
