#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CC(p, s) p##s
#define C(p, s) CC(p, s)
#define V(p) C(p##_, __LINE__)
#define T(t, x, _)                                                             \
  do {                                                                         \
    if (!t)                                                                    \
      break;                                                                   \
    struct n *V(s)[4096], *x = t->l;                                           \
    int V(p);                                                                  \
    if (t->r)                                                                  \
      V(s)[0] = t->r, V(s)[1] = t, V(p) = 2;                                   \
    else                                                                       \
      V(s)[0] = t, V(p) = 1;                                                   \
    while (V(p)) {                                                             \
      while (x) {                                                              \
        if (x->r)                                                              \
          V(s)[V(p)++] = x->r;                                                 \
        V(s)[V(p)++] = x;                                                      \
        x = x->l;                                                              \
      }                                                                        \
      x = V(s)[--V(p)];                                                        \
      if (V(p) && x->r && V(s)[V(p) - 1] == x->r) {                            \
        V(s)[V(p) - 1] = x;                                                    \
        x = x->r;                                                              \
      } else {                                                                 \
        do {                                                                   \
          {                                                                    \
            _;                                                                 \
          }                                                                    \
        } while (0);                                                           \
        x = NULL;                                                              \
      }                                                                        \
    }                                                                          \
  } while (0)

static inline struct n {
  int k[2];
  struct n *l, *r;
} *g(struct n **r, const int *k) {
  struct n *x = *r, *y = NULL;
  while (x) {
    y = x;
    if (k[0] < x->k[0] || k[0] == x->k[0] && k[1] < x->k[1])
      x = x->l;
    else if (k[0] > x->k[0] || k[0] == x->k[0] && k[1] > x->k[1])
      x = x->r;
    else
      return x;
  }
  x = malloc(sizeof(*x));
  *x = (struct n){.k = {k[0], k[1]}};
  if (y) {
    if (k[0] < y->k[0] || k[0] == y->k[0] && k[1] < y->k[1])
      y->l = x;
    else
      y->r = x;
  } else
    *r = x;
  return x;
}

int main(int argc, char **argv) {
  int x[2] = {}, y[2] = {}, z[2] = {}, t = 0;
  struct n *u = NULL, *v = NULL;
  g(&u, (int[]){0, 0}), g(&v, (int[]){0, 0});
  char c;
  while ((c = fgetc(stdin)) > 0) {
    switch (c) {
    case '^':
      x[1]++, t ? y[1]++ : z[1]++;
      break;
    case '<':
      x[0]--, t ? y[0]-- : z[0]--;
      break;
    case 'v':
      x[1]--, t ? y[1]-- : z[1]--;
      break;
    case '>':
      x[0]++, t ? y[0]++ : z[0]++;
      break;
    };
    g(&u, x), g(&v, y), g(&v, z);
    t = (t + 1) % 2;
  }
  int n = 0;
  T(u, x, n++; free(x));
  printf("houses with just one santa: %u\n", n);
  n = 0;
  T(v, x, n++; free(x));
  printf("houses with real and robo santas: %u\n", n);
}
