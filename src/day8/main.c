#include <stdio.h>

#define BUFFER_SIZE 256

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char buffer[BUFFER_SIZE];
  int diff_mem = 0, diff_enc = 0;
  while (fgets(buffer, BUFFER_SIZE, fp)) {
    int str_len = 2, mem_len = 0, enc_len = 2;
    char *str;
    // iterate over the intermediate characters
    for (str = buffer + 1; *(str + 1) != '\0'; str++) {
      if (*str == '\\') {
        str++;
        str_len++;
        if (*str == 'x') {
          str_len += 2;
          str += 2;
        }
      }
      str_len++;
      mem_len++;
    }
    for (str = buffer; *str != '\0'; str++) {
      if (*str == '\\' || *str == '\"') {
        enc_len++;
      }
      enc_len++;
    }
    diff_mem += (str_len - mem_len);
    diff_enc += (enc_len - str_len);
  }
  // now iterate over everything
  fclose(fp);
  printf("difference between length and memory: %d\n", diff_mem);
  printf("difference between length and encoding: %d\n", diff_enc);
  return 0;
}
