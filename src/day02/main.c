#include <stdint.h>
#include <stdio.h>

#define BUFFER_LENGTH 256

int main(int argc, char **argv) {
  FILE *fp;
  fp = fopen(argv[1], "r");
  char buffer[BUFFER_LENGTH];
  uint32_t paper = 0, ribbon = 0;
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    uint32_t x, y, z;
    sscanf(buffer, "%ux%ux%u", &x, &y, &z);
    if (x > y) {
      uint32_t temp = x;
      x = y;
      y = temp;
    }
    if (y > z) {
      uint32_t temp = y;
      y = z;
      z = temp;
    }
    if (x < y) {
      uint32_t temp = x;
      x = y;
      y = temp;
    }
    paper += 3 * x * y + 2 * y * z + 2 * z * x;
    ribbon += 2 * x + 2 * y + x * y * z;
  }
  printf("total paper: %u\n", paper);
  printf("total ribbon: %u\n", ribbon);
}
