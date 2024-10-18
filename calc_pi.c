#include <stdio.h>
#include <time.h>
static long num_steps = 10000000000;
double step;
int main(int argc, char *argv[]) {
    long int i;
    double x, pi, sum = 0.0;
    clock_t start_time, end_time;
    step = 1.0 / (double)num_steps;
    start_time = clock(); // Tempo de início da execução
    for (i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    end_time = clock(); // Tempo final
    double run_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Tempo em segundos
    printf("pi = %2.15f, %ld passos, computados em %lf segundos\n", pi, num_steps, run_time);
    return 0;
}
