#include <stdio.h>
#include "mpi.h"
static long num_steps = 100000000;
double step;
int main(int argc, char *argv[])
{
  int i, rank, size;
  double x, pi, sum = 0.0, global_sum = 0.0;
  double start_time, run_time;
  MPI_Init(&argc, &argv);               // Inicia o MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Pega o rank do processo
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Pega o número de processos
  step = 1.0 / (double)num_steps;
  start_time = MPI_Wtime();             // Tempo de início da execução
  // Paralelismo dentro de cada processo MPI
  for (i = rank; i < num_steps; i += size) { // Saltos de acordo com o número de processos
    x = (i + 0.5) * step;
    sum += 4.0 / (1.0 + x * x);
  }
  // MPI_Reduce para somar os resultados de todos os processos MPI
  MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    pi = step * global_sum; // Apenas o processo 0 calcula o valor final de Pi
    run_time = MPI_Wtime() - start_time;
    printf("pi = %lf, %ld passos, computados em %lf segundos\n", pi, num_steps, run_time);
  }
  MPI_Finalize(); // Finaliza MPI
  return 0;
}
