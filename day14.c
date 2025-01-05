#include <stdio.h>

int main(int argc, char **argv) {
  int v[16], r[16], s[16], n;
  for (n = 0;
       scanf(" %*s can fly %u km/s for %u seconds, but then must rest for %u "
             "seconds.",
             v + n, r + n, s + n) > 0;
       n++)
    ;
  int d[16] = {}, p[16] = {};
  for (int t = 0; t < 2503; t++) {
    for (int i = 0; i < n; i++)
      if (t % (r[i] + s[i]) < r[i])
        d[i] += v[i];
    int l = 0;
    for (int i = 0; i < n; i++)
      if (d[i] > l)
        l = d[i];
    for (int i = 0; i < n; i++)
      p[i] += d[i] == l;
  }
  int d0 = 0, p0 = 0;
  for (int i = 0; i < n; i++) {
    if (d[i] > d0)
      d0 = d[i];
    if (p[i] > p0)
      p0 = p[i];
  }
  printf("distance lead: %d\n", d0);
  printf("point lead: %d\n", p0);
}
