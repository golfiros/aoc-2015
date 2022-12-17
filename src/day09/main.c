#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 256
#define MAX_NODES 16

int held_karp(int32_t graph[MAX_NODES][MAX_NODES], uint8_t n_nodes) {
  // we'll be representing subsets as powers of 2
  // and we'll actually run it on a graph of n_nodes+1 nodes,
  // where this final node has distance 0 to everyone else, and
  // starting and ending at the final node.
  int32_t g[1 << MAX_NODES][MAX_NODES] = {0};
  for (int s = 2; s <= n_nodes; s++) {
    // iterate over subsets in increasing
    // cardinality using Gosper's hack
    uint32_t set = (1 << s) - 1;
    uint32_t limit = (1 << n_nodes);
    while (set < limit) {
      for (int k = 0; k < n_nodes; k++) {
        uint32_t flag = 1 << k;
        if (!(set & flag)) {
          continue;
        }
        int32_t min = INT32_MAX;
        for (int m = 0; m < n_nodes; m++) {
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
  for (int i = 0; i < n_nodes; i++) {
    int32_t dist = g[(1 << n_nodes) - 1][i];
    if (dist < min) {
      min = dist;
    }
  }
  return min;
}

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char buffer[BUFFER_LENGTH];
  uint8_t n_nodes;
  // get the number of cities
  {
    n_nodes = 1;
    char city1[BUFFER_LENGTH];
    char city2[BUFFER_LENGTH];
    fscanf(fp, "%s to %*s = %*d", city1);
    strcpy(city2, city1);
    while (!strcmp(city1, city2)) {
      n_nodes++;
      fscanf(fp, "%s to %*s = %*d", city2);
    }
  }
  rewind(fp);
  // read the graph weights
  int32_t graph[MAX_NODES][MAX_NODES] = {0};
  for (int i = 0; i < n_nodes; i++) {
    graph[i][i] = 0;
    for (int j = i + 1; j < n_nodes; j++) {
      fscanf(fp, "%*s to %*s = %d", &(graph[i][j]));
      graph[j][i] = graph[i][j];
    }
  }
  fclose(fp);
  // we now run Held-Karp
  // https://en.wikipedia.org/wiki/Held%E2%80%93Karp_algorithm
  printf("min distance: %d\n", held_karp(graph, n_nodes));
  // and now we flip the signs of every distance
  for (int i = 0; i < n_nodes; i++) {
    for (int j = 0; j < n_nodes; j++) {
      graph[i][j] *= -1;
    }
  }
  // and run again
  printf("max distance: %d\n", -held_karp(graph, n_nodes));

  return 0;
}
