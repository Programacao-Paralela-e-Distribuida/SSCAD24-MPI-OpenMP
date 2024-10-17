#include <stdio.h>
#include <omp.h>
#include <math.h>

int main() {
  long int num_steps = 10000000000;
  double step = 1.0 / (double) num_steps;
  double pi = 0.0, begin, end;
  begin=omp_get_wtime();
  #pragma omp parallel shared(pi) num_threads(8)
  {
   #pragma omp for reduction(+:pi)
    for (long int i = 0; i < num_steps; i++) {
      double x = (i + 0.5) * step;
      pi += 4.0 / (1.0 + x * x);
    }
  }
  pi *= step;

  end=omp_get_wtime();
  printf("Valor de Pi calculado: %3.15f. O tempo de execução foi %3.15f \n", pi, end-begin);

  return 0;
}
