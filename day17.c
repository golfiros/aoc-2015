#include <stdio.h>

#define M(x, y) ((x < y) ? x : y)

int main(int argc, char **argv) {
  int c[32], n = 0, f[32][256] = {}, g[32][256] = {};
  while (scanf(" %d", c + n + 1) > 0)
    n++;
  for (int i = 0; i <= n; i++) {
    f[i][0] = 1;
    for (int k = 1; k <= 150; k++)
      g[i][k] = 1 << 30;
  }
  for (int i = 1; i <= n; i++)
    for (int k = 0; k <= 150; k++) {
      f[i][k] = f[i - 1][k] + ((k - c[i] >= 0) ? f[i - 1][k - c[i]] : 0);
      if (c[i] == k)
        g[i][k] = 1;
      else if (c[i] > k)
        g[i][k] = g[i - 1][k];
      else
        g[i][k] = M(g[i - 1][k], 1 + g[i - 1][k - c[i]]);
    }
  printf("total ways: %d\n", f[n][150]);
  int s = (1 << g[n][150]) - 1, l = 1 << n, m = 0;
  while (s < l) {
    int t = 0;
    for (int i = 0; i < n; i++)
      t += !!(s & (1 << i)) * c[i + 1];
    m += t == 150;
    int c = s & -s, r = s + c;
    s = (((r ^ s) >> 2) / c) | r;
  }
  printf("total optimal: %d\n", m);
}
