#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUNDS 40
#define EXTRA_ROUNDS 10
#define BUFFER_LENGTH 1 << 23 // log2(10*(1.3035..)^50) ~ 22.5

int main(int argc, char **argv) {
  // computers nowadays are fast enough to
  // just put everything on the heap and brute
  // force stuff
  char *old_string = malloc(sizeof(char) * BUFFER_LENGTH);
  char *new_string = malloc(sizeof(char) * BUFFER_LENGTH);

  FILE *fp = fopen(argv[1], "r");
  fgets(old_string, BUFFER_LENGTH, fp);
  fclose(fp);
  old_string[strlen(old_string) - 1] = '\0';

  for (int i = 0; i < ROUNDS; i++) {
    char *old_str = old_string, *new_str = new_string;
    while (*old_str != 0) {
      char c = *old_str;
      int count = 0;
      while (*old_str == c) {
        count++;
        old_str++;
      }
      sprintf(new_str, "%d%c", count, c);
      new_str += strlen(new_str);
    }
    strcpy(old_string, new_string);
  }
  printf("first %d rounds: %lu\n", ROUNDS, strlen(old_string));

  for (int i = 0; i < EXTRA_ROUNDS; i++) {
    char *old_str = old_string, *new_str = new_string;
    while (*old_str != 0) {
      char c = *old_str;
      int count = 0;
      while (*old_str == c) {
        count++;
        old_str++;
      }
      sprintf(new_str, "%d%c", count, c);
      new_str += strlen(new_str);
    }
    strcpy(old_string, new_string);
  }
  printf("after %d rounds: %lu\n", EXTRA_ROUNDS, strlen(old_string));
  return 0;
}
