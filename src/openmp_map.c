#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {
  int N = 10;
  int a[N], b[N], c[N];
  // Iniciando os vetores no hospedeiro
  for (int i = 0; i < N; i++)
  {
    a[i] = i;
    b[i] = i * 2;
  }
// Regiao paralela no dispositivo (GPU)
#pragma omp target map(to : a, b) map(from : c)
  {
    for (int i = 0; i < N; i++) {
      c[i] = a[i] + b[i];
    }
  }
  // Exibindo os resultados no hospedeiro
  for (int i = 0; i < N; i++) {
    printf("%d ", c[i]);
  }
  printf("\n");
  return 0;
}
