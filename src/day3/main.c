#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int32_t x, y;
} pair_t;

int dict_order(const void *p1, const void *p2) {
  pair_t pair1 = *(pair_t *)p1;
  pair_t pair2 = *(pair_t *)p2;
  if (pair1.x != pair2.x) {
    return pair1.x - pair2.x;
  }
  return pair1.y - pair2.y;
}

#define MAP_SIZE 8192

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");

  pair_t just_santa = {0};
  pair_t real_santa = {0};
  pair_t robo_santa = {0};

  pair_t old_strategy[MAP_SIZE];
  pair_t new_strategy[MAP_SIZE];

  old_strategy[0] = new_strategy[0] = just_santa;
  uint32_t old_counter = 1, new_counter = 1;

  char c;
  uint8_t turn = 0;

  while ((c = fgetc(fp)) != EOF) {
    if (c == '^') {
      just_santa.y++;
      turn ? (real_santa.y++) : (robo_santa.y++);
    } else if (c == 'v') {
      just_santa.y--;
      turn ? (real_santa.y--) : (robo_santa.y--);
    } else if (c == '>') {
      just_santa.x++;
      turn ? (real_santa.x++) : (robo_santa.x++);
    } else if (c == '<') {
      just_santa.x--;
      turn ? (real_santa.x--) : (robo_santa.x--);
    }
    if (!bsearch(&just_santa, old_strategy, old_counter, sizeof(pair_t),
                 dict_order)) {
      old_strategy[old_counter++] = just_santa;
      qsort(old_strategy, old_counter, sizeof(pair_t), dict_order);
    }
    if (!bsearch(turn ? &real_santa : &robo_santa, new_strategy, new_counter,
                 sizeof(pair_t), dict_order)) {
      new_strategy[new_counter++] = turn ? real_santa : robo_santa;
      qsort(new_strategy, new_counter, sizeof(pair_t), dict_order);
    }
    turn = (turn + 1) % 2;
  }
  printf("houses with just one santa: %u\n", old_counter);
  printf("houses with real and robo santas: %u\n", new_counter);
  fclose(fp);
}
