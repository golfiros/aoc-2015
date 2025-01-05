#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  char l[8] = {};
  int m = 0, n[16] = {}, p = 0, d = 0;
  while ((*l = fgetc(stdin)) > 0) {
    char k[8] = {};
    for (int s = 0; *l == '-' || (*l >= '0' && *l <= '9'); s++)
      k[s] = *l, *l = fgetc(stdin);
    m += atoi(k), n[p] += !d * atoi(k);
    if (*l == '{')
      p++, n[p] = 0, d += !!d;
    else if (*l == '}')
      n[p - 1] += n[p], p--, d -= !!d;
    else if (!d && !strcmp(l, "\"der\":"))
      n[p] = 0, d = 1;
    for (int i = 4; i >= 0; i--)
      l[i + 1] = l[i];
  }
  printf("full sum: %d\n", m);
  printf("no reds: %d\n", n[0]);
}
