#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct d {
  const char *k;
  int v;
} m[] = {
    {"children:", 3}, {"cats:", 7},     {"samoyeds:", 2}, {"pomeranians:", 3},
    {"akitas:", 0},   {"vizslas:", 0},  {"goldfish:", 5}, {"trees:", 3},
    {"cars:", 2},     {"perfumes:", 1},
};

int cmp(const void *x1, const void *x2) {
  const struct d *d1 = x1, *d2 = x2;
  return strcmp(d1->k, d2->k);
}

int main(int argc, char **argv) {
  qsort(m, sizeof(m) / sizeof(*m), sizeof(*m), cmp);
  int n, x[3], a = 0, b = 0;
  char k[3][16];
  while (!(a && b) && scanf(" Sue %d: %s %d, %s %d, %s %d", &n, k[0], x + 0,
                            k[1], x + 1, k[2], x + 2) > 0) {
    int t = !a, s = !b;
    for (int i = 0; (t || s) && i < 3; i++) {
      int v = ((struct d *)bsearch(&(struct d){.k = k[i]}, m,
                                   sizeof(m) / sizeof(*m), sizeof(*m), cmp))
                  ->v;
      t = t && v == x[i];
      if (!strcmp(k[i], "cats:") || !strcmp(k[i], "trees:"))
        s = s && v < x[i];
      else if (!strcmp(k[i], "pomeranians:") || !strcmp(k[i], "goldfish:"))
        s = s && v > x[i];
      else
        s = s && v == x[i];
    }
    if (t)
      a = n;
    if (s)
      b = n;
  }
  printf("exact sue %d\n", a);
  printf("range sue %d\n", b);
}
