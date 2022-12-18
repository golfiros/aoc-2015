#include <stdint.h>
#include <stdio.h>

#define BUFFER_LENGTH 256
#define MAX_REINDEER 16

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  printf("input the time: ");
  uint32_t total_time;
  scanf("%u", &total_time);
  char reindeer[MAX_REINDEER][BUFFER_LENGTH];
  uint32_t speed[MAX_REINDEER], run_time[MAX_REINDEER], rest_time[MAX_REINDEER];
  uint8_t n_reindeer;
  for (n_reindeer = 0;
       fscanf(fp,
              "%s can fly %u km/s for %u seconds, but then must rest for %u "
              "seconds.",
              reindeer[n_reindeer], &speed[n_reindeer], &run_time[n_reindeer],
              &rest_time[n_reindeer]) == 4;
       n_reindeer++)
    ;
  fclose(fp);
  uint32_t distance[MAX_REINDEER] = {0}, points[MAX_REINDEER] = {0};
  for (uint32_t time = 0; time < total_time; time++) {
    uint32_t leading_dist = 0;
    for (uint8_t deer = 0; deer < n_reindeer; deer++) {
      uint32_t cycle_time = run_time[deer] + rest_time[deer];
      uint32_t remaining_time = time % cycle_time;
      if (remaining_time < run_time[deer]) {
        distance[deer] += speed[deer];
      }
    }
    for (uint8_t deer = 0; deer < n_reindeer; deer++) {
      if (distance[deer] > leading_dist) {
        leading_dist = distance[deer];
      }
    }
    for (uint8_t deer = 0; deer < n_reindeer; deer++) {
      if (distance[deer] == leading_dist) {
        points[deer]++;
      }
    }
  }
  printf("results:\n");
  for (uint8_t deer; deer < n_reindeer; deer++) {
    printf("%s\t%d\t%d\n", reindeer[deer], distance[deer], points[deer]);
  }
  uint32_t max_dist = 0, max_points = 0;
  uint8_t dist_deer, point_deer;
  for (uint8_t deer = 0; deer < n_reindeer; deer++) {
    if (distance[deer] > max_dist) {
      max_dist = distance[deer];
      dist_deer = deer;
    }
    if (points[deer] > max_points) {
      max_points = points[deer];
      point_deer = deer;
    }
  }
  printf("%s in the lead with %u distance\n", reindeer[dist_deer],
         distance[dist_deer]);
  printf("%s in the lead with %u points\n", reindeer[point_deer],
         points[point_deer]);
  /*
  uint32_t cycle_time = run_time + rest_time;
  uint32_t n_cycles = total_time / cycle_time;
  uint32_t remaining_time = total_time % cycle_time;
  uint32_t distance =
     speed * (n_cycles * run_time +
              ((remaining_time > run_time) ? run_time : remaining_time));
  printf("%u\n", distance);
  */

  return 0;
}
