#include <stdio.h>
#include <omp.h>
#include <math.h>

int main() {
  long int num_steps = 10000000000; // Número de passos para a integração
  double step = 1.0 / (double) num_steps;
  double pi = 0.0;
  double begin, end;
  begin = omp_get_wtime();
  #pragma omp target data map(tofrom: pi) map(to:num_steps, step) device(1)// Diretiva para offloading para a GPU
  #pragma omp target teams distribute parallel for reduction(+:pi) // Paralelização com OpenMP
     for (long int i = 0; i < num_steps; i++) {
          double x = (i + 0.5) * step;
          pi += 4.0 / (double) (1.0 + x * x); 
     }
  pi *= step;
  end = omp_get_wtime();
  printf("Valor de Pi calculado: %2.15f\n", pi);
  printf("Tempo de execução: %f segundos\n", end - begin);
  return 0;
}
