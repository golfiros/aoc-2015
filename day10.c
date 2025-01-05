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
  *x = (struct n){.k = malloc(strlen(k) + 1)};
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
  char str[256];
  scanf("%s", str);
  struct n *s = NULL, *t = NULL;
  g(&s, str)->v = 1;
  for (int i = 0; i < 40; i++) {
    T(s, x, {
      for (int i = 0; i < 256; i++)
        str[i] = 0;
      const char *k = x->k;
      char *l = str;
      while (*k) {
        const char c = *k;
        int n = 0;
        while (*k == c) {
          n++;
          k++;
        }
        l += sprintf(l, "%d%c", n, c);
      }
      for (; l >= str; l--)
        if ((l[-1] == '2') && ((l[0] && l[0] == '1' && l[1] && l[1] != '1' &&
                                l[2] && l[2] != l[1]) ||
                               (l[0] && l[0] == '1' && l[1] && l[1] == '1' &&
                                l[2] && l[2] == '1')) ||
            l == str) {
          g(&t, l)->v += x->v;
          *l = 0;
        }
      free(x);
    });
    s = t, t = NULL;
  }
  int n = 0;
  T(s, x, n += x->v * strlen(x->k));
  printf("%d\n", n);
  for (int i = 0; i < 10; i++) {
    T(s, x, {
      for (int i = 0; i < 256; i++)
        str[i] = 0;
      const char *k = x->k;
      char *l = str;
      while (*k) {
        const char c = *k;
        int n = 0;
        while (*k == c) {
          n++;
          k++;
        }
        l += sprintf(l, "%d%c", n, c);
      }
      for (; l >= str; l--)
        if ((l[-1] == '2') && ((l[0] && l[0] == '1' && l[1] && l[1] != '1' &&
                                l[2] && l[2] != l[1]) ||
                               (l[0] && l[0] == '1' && l[1] && l[1] == '1' &&
                                l[2] && l[2] == '1')) ||
            l == str) {
          g(&t, l)->v += x->v;
          *l = 0;
        }
      free(x);
    });
    s = t, t = NULL;
  }
  n = 0;
  T(s, x, n += x->v * strlen(x->k));
  printf("%d\n", n);
}
