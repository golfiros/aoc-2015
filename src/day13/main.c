#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 256
#define MAX_NODES 16

int held_karp(int32_t graph[MAX_NODES][MAX_NODES], uint8_t n_nodes) {
  // we'll be representing subsets as powers of 2
  int32_t g[1 << MAX_NODES][MAX_NODES] = {0};
  for (int k = 0; k < n_nodes - 1; k++) {
    g[1 << k][k] = graph[n_nodes - 1][k];
  }
  for (int s = 2; s < n_nodes; s++) {
    // iterate over subsets in increasing
    // cardinality using Gosper's hack
    uint32_t set = (1 << s) - 1;
    uint32_t limit = (1 << (n_nodes - 1));
    while (set < limit) {
      for (int k = 0; k < n_nodes - 1; k++) {
        uint32_t flag = 1 << k;
        if (!(set & flag)) {
          continue;
        }
        int32_t min = INT32_MAX;
        for (int m = 0; m < n_nodes - 1; m++) {
          if (m == k || !(set & (1 << m))) {
            continue;
          }
          int32_t dist = g[set ^ flag][m] + graph[m][k];
          if (dist < min) {
            min = dist;
          }
        }
        g[set][k] = min;
      }
      uint32_t c = set & -set;
      uint32_t r = set + c;
      set = (((r ^ set) >> 2) / c) | r;
    }
  }
  int32_t min = INT32_MAX;
  for (int i = 0; i < n_nodes - 1; i++) {
    int32_t dist = g[(1 << (n_nodes - 1)) - 1][i] + graph[i][n_nodes - 1];
    if (dist < min) {
      min = dist;
    }
  }
  return min;
}
int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  // we have another travelling salesman problem
  // get the number of people
  uint8_t n_nodes;
  {
    n_nodes = 1;
    char name1[BUFFER_LENGTH];
    char name2[BUFFER_LENGTH];
    fscanf(fp, "%s would %*s %*d happiness units by sitting next to %*s",
           name1);
    strcpy(name2, name1);
    while (!strcmp(name1, name2)) {
      n_nodes++;
      fscanf(fp, "%s would %*s %*d happiness units by sitting next to %*s",
             name2);
    }
  }
  rewind(fp);
  // now populate a graph whose weights are the
  // negative happiness provided by a pairing
  int32_t graph[MAX_NODES][MAX_NODES] = {0};
  uint32_t number;
  char buffer[BUFFER_LENGTH];
  for (int i = 0; i < n_nodes; i++) {
    graph[i][i] = 0;
    for (int j = 0; j < n_nodes; j++) {
      if (i == j) {
        continue;
      }
      fscanf(fp, "%*s would %s %d happiness units by sitting next to %*s",
             buffer, &number);
      if (buffer[0] == 'g') {
        graph[i][j] -= number;
        graph[j][i] -= number;
      } else {
        graph[i][j] += number;
        graph[j][i] += number;
      }
    }
  }
  fclose(fp);
  // we now run Held-Karp
  printf("max happiness: %d\n", -held_karp(graph, n_nodes));
  printf("max happiness with extra: %d\n", -held_karp(graph, n_nodes + 1));
  return 0;
}
