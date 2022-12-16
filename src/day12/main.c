#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 1 << 16
#define MAX_DEPTH 8

int main(int argc, char **argv) {
  char string[BUFFER_LENGTH];
  // push entire file on the stack
  // for convenience of manipulation
  FILE *fp = fopen(argv[1], "r");
  fgets(string, BUFFER_LENGTH, fp);
  fclose(fp);
  do {
    char *obj_start, *obj_end;
    // find first end of an object
    for (obj_end = string; *obj_end != '}' && *obj_end != 0; obj_end++)
      ;
    *obj_end++ = 0;
    // and so the start of the corresponding object
    for (obj_start = obj_end - 1; *obj_start != '{' && obj_start >= string;
         obj_start--)
      ;
    printf("%s\n", string);
    printf("%s\n", obj_start);
    int obj_sum = 0;
    {
      int num, n_bytes;
      char *p = obj_start;
      while (sscanf(p, "%*[^-0-9]%d%n", &num, &n_bytes) == 1) {
        obj_sum += num;
        p += n_bytes;
      }
    }
    sprintf(obj_start, "%d", obj_sum);
    obj_start += strlen(obj_start);
    size_t length = strlen(obj_end);
  } while (0);

  printf("sum with red: %d\n", 0);
  printf("sum without red: %d\n", 0);
  return 0;
}
