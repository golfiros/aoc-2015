#include <stdio.h>

int main(int argc, char **argv) {
  int p = 0, b = 0, i = 0;
  char c;
  while ((c = fgetc(stdin)) > 0) {
    if (c == '(')
      p++;
    if (c == ')')
      p--;
    i++;
    if (p < 0 && !b)
      b = i;
  }
  printf("final position: %d\n", p);
  printf("first basement: %d\n", b);
}
