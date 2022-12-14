#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256
#define X_DIM 1000
#define Y_DIM 1000

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char buffer[BUFFER_SIZE];
  int offset[3] = {7, 8, 9};

  int old_state[X_DIM][Y_DIM] = {0};
  int new_state[X_DIM][Y_DIM] = {0};

  while (fgets(buffer, BUFFER_SIZE, fp)) {
    int mode;
    if (buffer[1] == 'o') {
      mode = 0;
    } else if (buffer[6] == 'n') {
      mode = 1;
    } else {
      mode = 2;
    }
    int x_min, y_min, x_max, y_max;
    sscanf(buffer + offset[mode], "%d,%d through %d,%d", &x_min, &y_min, &x_max,
           &y_max);
    for (int i = x_min; i <= x_max; i++) {
      for (int j = y_min; j <= y_max; j++) {
        if (mode == 0) {
          old_state[i][j] = 1 - old_state[i][j];
          new_state[i][j] += 2;
        } else if (mode == 1) {
          old_state[i][j] = 1;
          new_state[i][j] += 1;
        } else {
          old_state[i][j] = 0;
          if (new_state[i][j] > 0) {
            new_state[i][j] -= 1;
          }
        }
      }
    }
  }
  int count = 0;
  int brightness = 0;
  for (int i = 0; i < X_DIM; i++) {
    for (int j = 0; j < Y_DIM; j++) {
      count += old_state[i][j];
      brightness += new_state[i][j];
    }
  }
  printf("old instructions: %d lights on\n", count);
  printf("new instructions: %d total brightness\n", brightness);
}
