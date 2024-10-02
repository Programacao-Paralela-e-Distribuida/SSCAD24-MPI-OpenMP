#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main(int argc, char *argv[])
{
  long int i;
  double x, pi, sum = 0.0;
  double start_time, run_time;
  step = 1.0 / (double)num_steps;
  start_time = omp_get_wtime(); // Tempo de início da execução
// Offloading com OpenMP para o acelerador (GPU), com paralelismo dentro do processo
#pragma omp target teams distribute parallel for default(none) firstprivate(num_steps, step) reduction(+ : sum) map(tofrom : sum) private(x) device(0)
  for (i = 0; i < num_steps; i++) {
    x = (i + 0.5) * step;
    sum += 4.0 / (1.0 + x * x);
  }
  pi = step * sum; // calcula o valor final de Pi
  run_time = omp_get_wtime() - start_time;
  printf("pi = %lf, %ld passos, computados em %lf segundos\n", pi, num_steps, run_time);
  return 0;
}
