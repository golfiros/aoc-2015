#include <stdio.h>

int main(int argc, char **argv) {
  FILE *fp;
  fp = fopen(argv[1], "r");
  char c;
  int position = 0;
  int character = 0;
  int basement = 0;
  while ((c = fgetc(fp)) != EOF) {
    if (c == '(') {
      position++;
    }
    if (c == ')') {
      position--;
    }
    character++;
    if (position < 0 && !basement) {
      basement = character;
    }
  }
  fclose(fp);
  printf("final position: %d\n", position);
  printf("first basement: %d\n", basement);
}
