#include <stdio.h>

int main(int argc, char **argv) {
  char str[256];
  int a = 0, b = 0;
  while (fgets(str, sizeof(str), stdin)) {
    char *c;
    int s = 2, m = 0, e = 2;
    for (c = str + 1; *(c + 1) != '\0'; c++, s++, m++)
      if (*c == '\\') {
        c++, s++;
        if (*c == 'x')
          s += 2, c += 2;
      }
    for (c = str; *c != '\0'; c++, e++)
      e += *c == '\\' || *c == '\"';
    a += (s - m);
    b += (e - s);
  }
  printf("memory: %d\n", a);
  printf("encoding: %d\n", b);
}
