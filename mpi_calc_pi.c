#include <stdio.h>
#include <mpi.h>
static long num_steps = 10000000000;
double step;
int main(int argc, char *argv[])
{
  long int i;
  int rank, size, provided;
  double x, pi, sum = 0.0, global_sum = 0.0;
  double start_time, run_time;
  // Inicia o MPI com suporte para threads
  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
  if (provided < MPI_THREAD_FUNNELED) {
    printf("Nível de suporte para threads não é suficiente!\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // O rank do processo
  MPI_Comm_size(MPI_COMM_WORLD, &size); // O número de processos
  step = 1.0 / (double)num_steps;
  start_time = MPI_Wtime(); // Tempo de início da execução
  for (i = rank; i < num_steps; i += size) { // Saltos de acordo com o número de processos
    x = (i + 0.5) * step;
    sum += 4.0 / (1.0 + x * x);
  }
  // MPI_Reduce para somar os resultados de todos os processos MPI
  MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    pi = step * global_sum; // Só o processo 0 calcula o valor final de Pi
    run_time = MPI_Wtime() - start_time;
    printf("pi = %2.15f, %ld passos, computados em %lf segundos\n", pi, num_steps, run_time);
  }  
MPI_Finalize(); // Finaliza o MPI
  return 0;
}
