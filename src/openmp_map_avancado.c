#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
  int N = 10;
  int a[N], c[N];
  // Iniciando o vetor 'a' no hospedeiro
  for (int i = 0; i < N; i++) {
    a[i] = i;
  }
// Regiao paralela no dispositivo (GPU)
#pragma omp target map(to : a) map(alloc : c)
  {
    // Inicia o vetor 'c' no dispositivo
    for (int i = 0; i < N; i++) {
      c[i] = a[i] * 2;
    }
  }
  // Exibindo os resultados no hospedeiro
  for (int i = 0; i < N; i++) {
    printf("%d ", c[i]); // Este array nao foi transferido de volta!
  }
  printf("\n");
  return 0;
}
