#include <stdint.h>
#include <stdio.h>

void s(int *x, int *y) {
  int t = *x;
  *x = *y;
  *y = t;
}

int main(int argc, char **argv) {
  int p = 0, r = 0;
  int x, y, z;
  while (scanf(" %ux%ux%u", &x, &y, &z) > 0) {
    if (x > y)
      s(&x, &y);
    if (y > z)
      s(&y, &z);
    if (x < y)
      s(&x, &y);
    p += 3 * x * y + 2 * y * z + 2 * z * x;
    r += 2 * x + 2 * y + x * y * z;
  }
  printf("total paper: %u\n", p);
  printf("total ribbon: %u\n", r);
}
