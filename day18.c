#include <stdio.h>

int main(int argc, char **argv) {
  int x, y = 0, a[128][128], b[128][128];
  char str[128];
  while (scanf(" %s", str) > 0) {
    for (x = 0; str[x]; x++)
      a[x][y] = b[x][y] = str[x] == '#';
    y++;
  }
  for (int r = 0; r < 100; r++) {
    for (int i = 0; i < x; i++)
      for (int j = 0; j < y; j++) {
        int k = 0, l = 0;
        for (int d = 0; d < 8; d++) {
          int u = i + (int[]){0, 1, 1, 1, 0, -1, -1, -1}[d],
              v = j + (int[]){1, 1, 0, -1, -1, -1, 0, 1}[d];
          if (u >= 0 && u < x && v >= 0 && v < y) {
            k += (a[u][v] == 1 || a[u][v] == 3);
            l += (b[u][v] == 1 || b[u][v] == 3);
          }
        }
        if (a[i][j] == 1 && (k < 2 || k > 3))
          a[i][j] = 3;
        else if (a[i][j] == 0 && k == 3)
          a[i][j] = 2;
        if (b[i][j] == 1 && (l < 2 || l > 3))
          b[i][j] = 3;
        else if (b[i][j] == 0 && l == 3)
          b[i][j] = 2;
      }
    for (int i = 0; i < x; i++)
      for (int j = 0; j < y; j++) {
        if (a[i][j] == 2)
          a[i][j] = 1;
        else if (a[i][j] == 3)
          a[i][j] = 0;
        if (b[i][j] == 2)
          b[i][j] = 1;
        else if (b[i][j] == 3)
          b[i][j] = 0;
      }
    b[0][0] = b[0][y - 1] = b[x - 1][0] = b[x - 1][y - 1] = 1;
  }
  int m = 0, n = 0;
  for (int i = 0; i < x; i++)
    for (int j = 0; j < y; j++)
      m += a[i][j], n += b[i][j];
  printf("original: %d\n", m);
  printf("modified: %d\n", n);
}
