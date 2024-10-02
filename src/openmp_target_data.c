#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
  int N = 100;
  double A[N], B[N];
  // Iniciando as matrizes no hospedeiro
  for (int i = 0; i < N; i++) {
    A[i] = i * 1.0;
    B[i] = 0.0;
  }
// Copiar A para o dispositivo, e depois B de volta ao hospedeiro
#pragma omp target data map(to : A[0 : N]) map(from : B[0 : N])
  {
// Executa no dispositivo
#pragma omp target teams distribute parallel for
    for (int i = 0; i < N; i++) {
      B[i] = A[i] * 2.0;
    }
  }
  // Imprimindo o resultado no hospedeiro
  for (int i = 0; i < N; i++) {
    printf("B[%d] = %f\n", i, B[i]);
  }
  return 0;
}
