#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  const char *k;
  int v;
  struct n *l, *r;
} *g(struct n **r, const char *k) {
  struct n *x = *r, *y = NULL;
  while (x) {
    y = x;
    if (strcmp(k, x->k) < 0)
      x = x->l;
    else if (strcmp(k, x->k) > 0)
      x = x->r;
    else
      return x;
  }
  x = malloc(sizeof(*x));
  *x = (struct n){.k = malloc(strlen(k) + 1), .v = -1};
  strcpy((char *)x->k, k);
  if (y) {
    if (strcmp(k, y->k) < 0)
      y->l = x;
    else
      y->r = x;
  } else
    *r = x;
  return x;
}

int main(int argc, char **argv) {
  char str[1024], s[1024], a[64][16], b[64][16];
  int p = 0;
  while (fgets(str, sizeof(str), stdin) && str[0] != '\n')
    sscanf(str, " %s => %s", a[p], b[p]), p++;
  fgets(str, sizeof(str), stdin);
  struct n *t = NULL;
  for (char *c = str; *c; c++)
    for (int i = 0; i < p; i++)
      if (!strncmp(c, a[i], strlen(a[i]))) {
        sprintf(s, "%.*s%s%s", (int)(c - str), str, b[i], c + strlen(a[i]));
        g(&t, s);
      }
  int m = 0;
  T(t, x, m++; free(x));
  printf("different molecules: %d\n", m);
  int n = 0, k = 0, c = 0;
  char *l = str;
  while (*l != '\n') {
    char *r = l + 1;
    while (*r != '\n' && *r > 'Z')
      r++;
    k += !strncmp("Rn", l, r - l) || !strncmp("Ar", l, r - l);
    c += !strncmp("Y", l, r - l);
    n++;
    l = r;
  }
  printf("number of steps: %d\n", n - k - 2 * c - 1);
}
