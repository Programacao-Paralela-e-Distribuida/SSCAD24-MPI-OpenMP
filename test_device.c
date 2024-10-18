#include <stdio.h>
#include <omp.h>

int main() {
  int numdevices = omp_get_num_devices();
  int device= omp_get_device_num();
  printf("number of devices= %d *** device= %d\n", numdevices, device);
}
