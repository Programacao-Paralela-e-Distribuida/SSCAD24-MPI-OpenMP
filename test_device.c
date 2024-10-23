#include <stdio.h>
#include <omp.h>
//  Use este programa para verficar se as opções de compilação estão corretas 
//  e a(s) GPUS(s) conseguem ser identificadas.
int main() {
  int numdevices = omp_get_num_devices();
  int device= omp_get_device_num();
  printf("number of devices= %d *** device= %d\n", numdevices, device);
}
