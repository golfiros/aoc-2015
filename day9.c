#include <stdio.h>
#include <string.h>

int h(int g[8][8], int n) {
  int f[1 << 8][8] = {};
  for (int u = 2; u <= n; u++) {
    int s = (1 << u) - 1, l = 1 << n;
    while (s < l) {
      for (int k = 0; k < n; k++) {
        if (!(s & (1 << k)))
          continue;
        int d0 = 1 << 30;
        for (int v = 0; v < n; v++) {
          if (v == k || !(s & (1 << v)))
            continue;
          int d = f[s ^ (1 << k)][v] + g[v][k];
          if (d < d0)
            d0 = d;
        }
        f[s][k] = d0;
      }
      int c = s & -s, r = s + c;
      s = (((r ^ s) >> 2) / c) | r;
    }
  }
  int d0 = 1 << 30;
  for (int i = 0; i < n; i++) {
    int d = f[(1 << n) - 1][i];
    if (d < d0)
      d0 = d;
  }
  return d0;
}

int main(int argc, char **argv) {
  char str[256];
  char c[16][16], a[16], b[16];
  int g[8][8] = {}, n = 0, d;
  while (scanf(" %s to %s = %d", a, b, &d) > 0) {
    int i, j;
    for (i = 0; i < n && strcmp(a, c[i]); i++)
      ;
    if (i == n)
      strcpy(c[n++], a);
    for (j = 0; j < n && strcmp(b, c[j]); j++)
      ;
    if (j == n)
      strcpy(c[n++], b);
    g[i][j] = g[j][i] = d;
  }
  printf("min distance: %d\n", h(g, n));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      g[i][j] *= -1;
  printf("max distance: %d\n", -h(g, n));
}
