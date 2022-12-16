#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 1 << 16
#define MAX_DEPTH 8

int main(int argc, char **argv) {
  char a[20];
  char string[BUFFER_LENGTH];
  // push entire file on the stack
  // for convenience of manipulation
  FILE *fp = fopen(argv[1], "r"), *out = fopen("out", "w");
  fgets(string, BUFFER_LENGTH, fp);
  string[strlen(string) - 1] = 0;
  fclose(fp);
  // keep a copy of the string on the heap because we'll
  // need it later
  char *string_copy = malloc(BUFFER_LENGTH * sizeof(char));
  strcpy(string_copy, string);
  // now solve part 1
  int obj_sum, found_obj = 1;
  while (1) {
    char *obj_start, *obj_end;
    // find first end of an object
    for (obj_end = string; *obj_end != '}' && *obj_end != 0; obj_end++)
      ;
    found_obj = *obj_end == '}';
    *obj_end++ = 0;
    // and so the start of the corresponding object
    for (obj_start = obj_end; *obj_start != '{' && obj_start > string;
         obj_start--)
      ;
    obj_sum = 0;
    // compute the sum of the numbers within the object
    int num, n_bytes;
    for (char *p = obj_start; sscanf(p, "%*[^-0-9]%d%n", &num, &n_bytes) == 1;
         p += n_bytes) {
      obj_sum += num;
    }
    // and replace the entire object by a string with the sum
    sprintf(obj_start, "%d%n", obj_sum, &n_bytes);
    obj_start += n_bytes;
    if (!found_obj) {
      break;
    }
    while (*obj_end != 0) {
      *obj_start++ = *obj_end++;
    }
    *obj_start = 0;
  }
  printf("sum with red: %s\n", string);
  // now we copy back our string into the stack
  strcpy(string, string_copy);
  // and do it all over again
  found_obj = 1;
  while (1) {
    char *obj_start, *obj_end;
    // find first end of an object
    for (obj_end = string; *obj_end != '}' && *obj_end != 0; obj_end++)
      ;
    found_obj = *obj_end == '}';

    *obj_end++ = 0;
    // and so the start of the corresponding object
    // also check if it contains "red" as a value
    int is_red = 0;
    for (obj_start = obj_end; *obj_start != '{' && obj_start > string;
         obj_start--) {
      is_red |=
          (obj_start[0] == ':' && obj_start[1] == '\"' && obj_start[2] == 'r' &&
           obj_start[3] == 'e' && obj_start[4] == 'd' && obj_start[5] == '\"');
    }
    // compute the sum of the numbers within the object
    // unless it's a red object, that we replace with 0
    obj_sum = 0;
    int num, n_bytes;
    if (!is_red || !found_obj) {
      for (char *p = obj_start; sscanf(p, "%*[^-0-9]%d%n", &num, &n_bytes) == 1;
           p += n_bytes) {
        obj_sum += num;
      }
    }
    // and replace the entire object by a string with the sum
    sprintf(obj_start, "%d%n", obj_sum, &n_bytes);
    obj_start += n_bytes;
    if (!found_obj) {
      break;
    }
    while (*obj_end != 0) {
      *obj_start++ = *obj_end++;
    }
    *obj_start = 0;
  }
  printf("sum without red: %s\n", string);
  return 0;
}
