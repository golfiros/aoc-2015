#include "maps.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int32_t x, y;
} pair_t;

int dictionary_order(const void *p1, const void *p2) {
  pair_t pair1 = *(pair_t *)p1;
  pair_t pair2 = *(pair_t *)p2;
  if (pair1.x != pair2.x) {
    return pair1.x - pair2.x;
  }
  return pair1.y - pair2.y;
}

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");

  map_t *old_strategy = new_map(sizeof(pair_t), 0, dictionary_order, NULL);
  map_t *new_strategy = new_map(sizeof(pair_t), 0, dictionary_order, NULL);

  char c;
  uint8_t turn = 0;
  pair_t just_santa = {0};
  pair_t real_santa = {0};
  pair_t robo_santa = {0};
  map_add(old_strategy, &just_santa, NULL);
  map_add(new_strategy, &just_santa, NULL);
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
    map_add(old_strategy, &just_santa, NULL);
    map_add(new_strategy, turn ? &real_santa : &robo_santa, NULL);
    turn = (turn + 1) % 2;
  }
  printf("houses with just one santa: %lu\n", map_size(old_strategy));
  delete_map(old_strategy);
  printf("houses with real and robo santas: %lu\n", map_size(new_strategy));
  delete_map(new_strategy);
  fclose(fp);
}
