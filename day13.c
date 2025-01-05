#include <stdio.h>
#include <string.h>

int h(int g[16][16], int n) {
  int f[1 << 16][16] = {};
  for (int k = 0; k < n - 1; k++)
    f[1 << k][k] = g[n - 1][k];
  for (int u = 2; u < n; u++) {
    int s = (1 << u) - 1, l = (1 << (n - 1));
    while (s < l) {
      for (int k = 0; k < n - 1; k++) {
        if (!(s & 1 << k))
          continue;
        int d0 = 1 << 30;
        for (int m = 0; m < n - 1; m++) {
          if (m == k || !(s & (1 << m)))
            continue;
          int d = f[s ^ (1 << k)][m] + g[m][k];
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
  for (int i = 0; i < n - 1; i++) {
    int d = f[(1 << (n - 1)) - 1][i] + g[i][n - 1];
    if (d < d0)
      d0 = d;
  }
  return d0;
}

int main(int argc, char **argv) {
  char str[256];
  char c[16][16], a[16], b[16], s[8];
  int g[16][16] = {}, n = 0, d;
  while (scanf(" %s would %s %d happiness units by sitting next to %[^.].", a,
               s, &d, b) > 0) {
    int i, j;
    for (i = 0; i < n && strcmp(a, c[i]); i++)
      ;
    if (i == n)
      strcpy(c[n++], a);
    for (j = 0; j < n && strcmp(b, c[j]); j++)
      ;
    if (j == n)
      strcpy(c[n++], b);
    if (s[0] == 'g')
      g[i][j] -= d, g[j][i] -= d;
    else
      g[i][j] += d, g[j][i] += d;
  }
  printf("max happiness: %d\n", -h(g, n));
  printf("with extra: %d\n", -h(g, n + 1));
}
