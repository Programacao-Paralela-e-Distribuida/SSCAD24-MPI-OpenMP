#include <stdio.h>
#include <omp.h>
#include <math.h>

int main() {
  static long num_steps = 10000000000;
  double step = 1.0 / (double) num_steps;
  double sum=0.0, pi = 0.0, begin, end;
  begin=omp_get_wtime();
  #pragma omp parallel shared(sum, step, num_steps) num_threads(8)
  {
   #pragma omp for reduction(+:sum)
    for (long int i = 0; i < num_steps; i++) {
      double x = (i + 0.5) * step;
      sum += 4.0 / (1.0 + x * x);
    }
  }
  pi = sum * step;
  end=omp_get_wtime();
  printf("Valor de Pi calculado: %3.15f. O tempo de execução foi %3.15f \n", pi, end-begin);
  return 0;
}
