#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 32

int old_nice(const char *str) {
  int vowel_count = 0, has_doubles = 0, has_forbidden = 0;

  for (; *str != '\0' && !has_forbidden; str++) {
    vowel_count += (*str == 'a') || (*str == 'e') || (*str == 'i') ||
                   (*str == 'o') || (*str == 'u');
    has_doubles |= *str == *(str + 1);
    has_forbidden |=
        (*str == 'a' || *str == 'c' || *str == 'p' || *str == 'x') &&
        *(str + 1) == *str + 1;
  }
  return (vowel_count > 2) && has_doubles && !has_forbidden;
}

int keycmp(const void *k1, const void *k2) {
  return strcmp((const char *)k1, (const char *)k2);
}

int new_nice(const char *str) {
  int has_doubles = 0, has_sandwich = 0;
  char pair[BUFFER_LENGTH][2];
  size_t pairs = 0;
  for (; *(str + 1) != '\0'; str++) {
    pair[pairs][0] = *str;
    pair[pairs++][1] = *(str + 1);
    has_sandwich |= (*str == *(str + 2));
  }
  // strings are small so iterating is cheap
  for (int i = 0; i < pairs; i++) {
    for (int j = i + 2; j < pairs; j++) {
      has_doubles |= (pair[i][0] == pair[j][0] && pair[i][1] == pair[j][1]);
    }
  }
  return has_doubles && has_sandwich;
}

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char buffer[BUFFER_LENGTH];
  int old_count = 0, new_count = 0;
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    old_count += old_nice(buffer);
    new_count += new_nice(buffer);
  }
  printf("nice strings with old rule: %d\n", old_count);
  printf("nice strings with new rule: %d\n", new_count);
}
