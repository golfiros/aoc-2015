#include <stdio.h>

int main(int argc, char **argv) {
  int m = 0, n = 0;
  char str[32];
  while (fgets(str, sizeof(str), stdin)) {
    char p[32][2];
    int k = 0, v = 0, d = 0, f = 0, s = 0, q = 0;
    for (char *c = str; *c; c++)
      v += (*c == 'a') || (*c == 'e') || (*c == 'i') || (*c == 'o') ||
           (*c == 'u');
    for (char *c = str; *(c + 1); c++) {
      d |= *c == *(c + 1);
      f |= (*c == 'a' || *c == 'c' || *c == 'p' || *c == 'x') &&
           *(c + 1) == *c + 1;
      p[k][0] = *c, p[k++][1] = *(c + 1);
    }
    for (char *c = str; *(c + 2); c++)
      s |= *c == *(c + 2);
    m += (v > 2) && d && !f;
    for (int i = 0; i < k; i++)
      for (int j = i + 2; j < k; j++)
        q |= p[i][0] == p[j][0] && p[i][1] == p[j][1];
    n += q && s;
  }
  printf("old nice strings: %d\n", m);
  printf("new nice strings: %d\n", n);
}
