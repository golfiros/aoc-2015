#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct l {
  const char a[8], b[8], c[8];
  int p;
  struct l *n;
};
void r(uint16_t v[], const struct l *l0) {
  struct l *l = NULL;
  for (const struct l *p = l0; p; p = p->n) {
    struct l *q = malloc(sizeof(*q));
    strcpy((char *)q->a, p->a);
    strcpy((char *)q->b, p->b);
    strcpy((char *)q->c, p->c);
    q->p = p->p, q->n = l, l = q;
  }
  int z[1024] = {};
  while (l)
    for (struct l *p = l, *q = NULL; p;) {
      int a = p->a[0] - 'a' + 26 * (p->a[1] ? 1 + p->a[1] - 'a' : 0),
          b = p->b[0] - 'a' + 26 * (p->b[1] ? 1 + p->b[1] - 'a' : 0),
          c = p->c[0] - 'a' + 26 * (p->c[1] ? 1 + p->c[1] - 'a' : 0);
      uint16_t a0, b0;
      if (a < 0)
        a0 = atoi(p->a);
      else if (!z[a]) {
        q = p, p = p->n;
        continue;
      } else
        a0 = v[a];
      if (p->p >= 2) {
        if (b < 0)
          b0 = atoi(p->b);
        else if (!z[b]) {
          q = p, p = p->n;
          continue;
        } else
          b0 = v[b];
      }
      switch (p->p) {
      case 0:
        v[c] = a0;
        break;
      case 1:
        v[c] = ~a0;
        break;
      case 2:
        v[c] = a0 & b0;
        break;
      case 3:
        v[c] = a0 | b0;
        break;
      case 4:
        v[c] = a0 << b0;
        break;
      case 5:
        v[c] = a0 >> b0;
        break;
      }
      z[c] = 1;
      if (!q)
        l = p->n, free(p), p = l;
      else
        q->n = p->n, free(p), p = q->n;
    }
}

int main(int argc, char **argv) {
  char s[5][8], str[64];
  struct l *l = NULL;
  while (fgets(str, sizeof(str), stdin)) {
    sscanf(str, " %s %s %s %s %s", s[0], s[1], s[2], s[3], s[4]);
    struct l *p = malloc(sizeof(*p));
    if (!strcmp(s[1], "->")) {
      strcpy((char *)p->a, s[0]);
      strcpy((char *)p->c, s[2]);
      p->p = 0;
    } else if (!strcmp(s[2], "->")) {
      strcpy((char *)p->a, s[1]);
      strcpy((char *)p->c, s[3]);
      p->p = 1;
    } else {
      strcpy((char *)p->a, s[0]);
      strcpy((char *)p->b, s[2]);
      strcpy((char *)p->c, s[4]);
      if (!strcmp(s[1], "AND"))
        p->p = 2;
      else if (!strcmp(s[1], "OR"))
        p->p = 3;
      else if (!strcmp(s[1], "LSHIFT"))
        p->p = 4;
      else
        p->p = 5;
    }
    p->n = l, l = p;
  }
  uint16_t v[1024] = {};
  r(v, l);
  printf("old value: %u\n", v[0]);
  for (struct l *p = l, *q = NULL; p;)
    if (!strcmp(p->c, "b")) {
      if (!q)
        l = p->n, free(p), p = l;
      else
        q->n = p->n, free(p), p = q->n;
    } else
      q = p, p = p->n;
  struct l *p = malloc(sizeof(*p));
  sprintf((char *)p->a, "%d", v[0]);
  sprintf((char *)p->c, "b");
  p->p = 0, p->n = l, l = p;
  r(v, l);
  printf("new value: %u\n", v[0]);
}
