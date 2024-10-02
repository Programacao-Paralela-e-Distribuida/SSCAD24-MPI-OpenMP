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
  start_time = omp_get_wtime();
  // Paralelismo das iterações nas threads disponiveis na região paralela
#pragma omp parallel for default(none) private(x) firstprivate(num_steps, step) reduction(+ : sum)
  for (i = 0; i < num_steps; i++) {
    x = (i + 0.5) * step;
    sum += 4.0 / (1.0 + x * x);
  }
  pi = step * sum;
  run_time = omp_get_wtime() - start_time;
  printf("pi = %lf, %ld passos, computados em %lf segundos\n", pi, num_steps, run_time);
}
