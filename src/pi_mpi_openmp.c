#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <omp.h>

static long num_steps = 1000000000;
const double PI = 3.141592653589793238462643;

int main(int argc, char *argv[])
{
  const double h = 1.0 / num_steps;
  double x, sum, pi, start_time, run_time, mypi;
  long int i;
  int rank, size, thr_status;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &thr_status);
  if (thr_status != MPI_THREAD_MULTIPLE) {
    printf("Erro ao iniciar no modo MPI_THREAD_MULTIPLE\n");
    exit(-1);
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  start_time = omp_get_wtime();
  sum = 0.0;
#pragma omp parallel for default(none) shared(num_steps, h, rank, size), private(i, x), reduction(+ : sum)
  // Paralelismo dentro de cada processo MPI
  for (i = rank; i <= num_steps; i += size) { // Saltos de acordo com o número de processos
    x = h * (double)i;
    sum += 4.0 / (1.0 + x * x);
  }
  mypi = h * sum;
  MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  run_time = omp_get_wtime() - start_time;
  if (rank == 0)
    printf("pi = %lf, %ld passos, computados em %lf segundos, diferença %lf\n", pi, num_steps, run_time, pi - PI);
  MPI_Finalize();
  return 0;
}
