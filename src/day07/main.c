#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 256
#define MAX_GATES 512
#define MAX_WIRES 27 * 27 + 1

enum src_type { VALUE, WIRE };

enum operation { CON, NOT, AND, OR, LSH, RSH };

struct gate {
  enum operation op;
  enum src_type t1, t2;
  uint16_t s1, s2;
  uint16_t d;
};

void emulate(uint16_t *wires, int *initialized, struct gate *gates,
             uint32_t gate_count) {
  // keep track of when we're done;
  int visited[MAX_GATES] = {0};
  int updated = 1;
  while (updated) {
    updated = 0;
    for (int i = 0; i < gate_count; i++) {
      // don't revisit old gates
      if (visited[i]) {
        continue;
      }
      struct gate *gate = &(gates[i]);
      // don't compute gates that don't have
      // their dependencies satisfied
      // or that lead to already initialized wires
      if ((gate->t1 == WIRE && !initialized[gate->s1]) ||
          (gate->t2 == WIRE && !initialized[gate->s2]) ||
          initialized[gate->d]) {
        continue;
      }
      // mark update, good wire and gate as visited
      updated = 1;
      initialized[gate->d] = 1;
      visited[i] = 1;
      uint16_t val1, val2;
      val1 = (gate->t1 == WIRE) ? wires[gate->s1] : gate->s1;
      val2 = (gate->t2 == WIRE) ? wires[gate->s2] : gate->s2;
      // actually do the operation
      switch (gate->op) {
      case CON:
        wires[gate->d] = val1;
        break;
      case NOT:
        wires[gate->d] = ~val1;
        break;
      case AND:
        wires[gate->d] = val1 & val2;
        break;
      case OR:
        wires[gate->d] = val1 | val2;
        break;
      case LSH:
        wires[gate->d] = val1 << val2;
        break;
      case RSH:
        wires[gate->d] = val1 >> val2;
        break;
      }
    }
  }
}

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char buffer[BUFFER_LENGTH];
  // build the circuit
  struct gate gates[MAX_GATES];
  uint32_t gate_count = 0;
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    struct gate *gate = &(gates[gate_count]);
    char str1[BUFFER_LENGTH] = {0}, str2[BUFFER_LENGTH] = {0},
         str3[BUFFER_LENGTH] = {0};
    if (sscanf(buffer, "%s -> %s", str1, str3) == 2) {
      gate->op = CON;
    } else if (sscanf(buffer, "NOT %s -> %s", str1, str3) == 2) {
      gate->op = NOT;
    } else if (sscanf(buffer, "%s AND %s -> %s", str1, str2, str3) == 3) {
      gate->op = AND;
    } else if (sscanf(buffer, "%s OR %s -> %s", str1, str2, str3) == 3) {
      gate->op = OR;
    } else if (sscanf(buffer, "%s LSHIFT %s -> %s", str1, str2, str3) == 3) {
      gate->op = LSH;
    } else if (sscanf(buffer, "%s RSHIFT %s -> %s", str1, str2, str3) == 3) {
      gate->op = RSH;
    } else {
      printf("failed to read: %s", buffer);
    }
    uint16_t s;
    if (sscanf(str1, "%hu", &s)) {
      gate->t1 = VALUE;
      gate->s1 = s;
    } else {
      gate->t1 = WIRE;
      gate->s1 = (str1[0] - 0x60) + (str1[1] ? 27 * (str1[1] - 0x60) : 0);
    }
    if (sscanf(str2, "%hu", &s)) {
      gate->t2 = VALUE;
      gate->s2 = s;
    } else {
      gate->t2 = WIRE;
      gate->s2 = (str2[0] - 0x60) + (str2[1] ? 27 * (str2[1] - 0x60) : 0);
    }
    gate->d = (str3[0] - 0x60) + (str3[1] ? 27 * (str3[1] - 0x60) : 0);
    gate_count++;
  }
  /*
  for (int i = 0; i < gate_count; i++) {
    struct gate *gate = &(gates[i]);
    printf("%d: (%d, %d) * (%d, %d) -> %d\n", gate->op, gate->t1, gate->s1,
           gate->t2, gate->s2, gate->d);
  }
  */
  // now we emulate
  uint16_t wires[MAX_WIRES];
  int initialized[MAX_WIRES] = {0};
  emulate(wires, initialized, gates, gate_count);
  printf("old value of wire a: %u\n", wires[1]);
  // reset initialized wires
  for (int i = 0; i < MAX_WIRES; i++) {
    initialized[i] = 0;
  }
  // and override wire b with the old value of wire a
  wires[2] = wires[1];
  initialized[2] = 1;
  // run the simulation again
  emulate(wires, initialized, gates, gate_count);
  printf("new value of wire a: %u\n", wires[1]);
  fclose(fp);
  return 0;
}
