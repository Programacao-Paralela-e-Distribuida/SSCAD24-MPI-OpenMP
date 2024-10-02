#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int N = 1000;
  int a[N], b[N], c[N];
  // Valores iniciais dos vetores no hospedeiro
  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = i * 2;
  }
// Região paralela no dispositivo (GPU)
#pragma omp target teams distribute parallel for
  for (int i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
  }
  // Exibindo os primeiros 10 resultados
  for (int i = 0; i < 10; i++) {
    printf("%d ", c[i]);
  }
  printf("\n");
  return 0;
}
