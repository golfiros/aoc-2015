#include <stdio.h>

int main(int argc, char **argv) {
  int n, p[8][5] = {};
  for (n = 0; scanf(" %*s capacity %d, durability %d, flavor %d, texture %d, "
                    "calories %d",
                    &p[n][0], &p[n][1], &p[n][2], &p[n][3], &p[n][4]) > 0;
       n++)
    ;
  int a[8] = {}, b[8] = {}, c[8] = {}, k = 100, s = 0, t = 0, l = 0;
  a[n - 1] = c[n - 1] = k;
  for (int i = 0; i < n; i++)
    b[i] = k;
  while (l >= 0) {
    int u = 1;
    for (int r = 0; r < 4; r++) {
      int q = 0;
      for (int i = 0; i < n; i++)
        q += c[i] * p[i][r];
      if (q > 0)
        u *= q;
      else
        u = 0;
    }
    if (s < u)
      s = u;
    int v = 0;
    for (int i = 0; i < n; i++)
      v += c[i] * p[i][4];
    if (v == 500 && t < u)
      t = u;

    l = n - 1;
    while (l >= 0) {
      c[l]++;
      if (c[l] <= b[l]) {
        if (l < n - 1) {
          int s = 0;
          for (int i = 0; i <= l; i++)
            s += c[i];
          for (int i = l + 1; i < n; i++)
            b[i] = k - s, a[i] = c[i] = 0;
          c[n - 1] = a[n - 1] = k - s;
        }
        break;
      }
      c[l] = a[l], l--;
    }
  }
  printf("full maximum: %d\n", s);
  printf("cons maximum: %d\n", t);
}
