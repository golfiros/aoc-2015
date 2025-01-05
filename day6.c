#include <stdio.h>

int main(int argc, char **argv) {
  int l[1000][1000] = {}, b[1000][1000] = {};
  char str[16] = {};
  int u, v, x, y;
  while (scanf(" %s %d,%d through %d,%d", str, &u, &v, &x, &y) > 0)
    switch (str[1]) {
    case 'o':
      for (int i = u; i <= x; i++)
        for (int j = v; j <= y; j++)
          l[i][j] = 1 - l[i][j], b[i][j] += 2;
      break;
    case 'n':
      for (int i = u; i <= x; i++)
        for (int j = v; j <= y; j++)
          l[i][j] = 1, b[i][j] += 1;
      break;
    case 'f':
      for (int i = u; i <= x; i++)
        for (int j = v; j <= y; j++) {
          l[i][j] = 0, b[i][j] -= b[i][j] > 0;
        }
      break;
    }
  int n = 0, k = 0;
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 1000; j++)
      n += l[i][j], k += b[i][j];
  printf("lights on: %d\n", n);
  printf("brightness: %d\n", k);
}
