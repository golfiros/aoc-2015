#include <stdio.h>

#define PASS_LENGTH 8

long long n(const char *p) {
  unsigned long n;
  for (n = 0; *p != 0; p++)
    n = 26 * n + (*p - 'a');
  return n;
}

void s(char *p, unsigned long n) {
  for (int i = 8 - 1; i >= 0; i--, n /= 26)
    p[i] = 'a' + (n % 26);
}

int v(const char *p) {
  int a = 0;
  for (int i = 0; !a && i < 8 - 2; i++)
    a = p[i] + 1 == p[i + 1] && p[i + 1] + 1 == p[i + 2];
  if (!a)
    return 0;

  for (int i = 0; i < 8; i++)
    if (p[i] == 'i' || p[i] == 'o' || p[i] == 'l')
      return 0;

  for (int i = 0; i < 8 - 3; i++) {
    if (p[i] != p[i + 1])
      continue;
    for (int j = i + 2; j < 8 - 1; j++)
      if (p[j] == p[j + 1])
        return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  char p[16] = {};
  scanf("%s", p);
  while (!v(p))
    s(p, n(p) + 1);
  printf("1st password: %s\n", p);
  s(p, n(p) + 1);
  while (!v(p))
    s(p, n(p) + 1);
  printf("2nd password: %s\n", p);
}
