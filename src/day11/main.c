#include <stdio.h>

#define PASS_LENGTH 8

unsigned long to_number(const char *password) {
  unsigned long number = 0;
  for (const char *str = password; *str != 0; str++) {
    number = 26 * number + (*str - 'a');
  }
  return number;
}

void to_string(char *password, unsigned long number) {
  for (int i = PASS_LENGTH - 1; i >= 0; i--) {
    password[i] = 'a' + (number % 26);
    number /= 26;
  }
}

int is_valid(const char *pass) {
  // we'll be iterating over the string
  // three times, one for each condition

  // check if it contains ascending triples
  int triple_flag = 0;
  for (int i = 0; i < PASS_LENGTH - 2; i++) {
    if (pass[i] + 1 == pass[i + 1] && pass[i + 1] + 1 == pass[i + 2]) {
      triple_flag = 1;
      break;
    }
  }
  if (!triple_flag) {
    return 0;
  }
  // check for forbidden letters
  for (int i = 0; i < PASS_LENGTH; i++) {
    if (pass[i] == 'i' || pass[i] == 'o' || pass[i] == 'l') {
      return 0;
    }
  }
  // check for pairs
  int pair_flag = 0;
  for (int i = 0; i < PASS_LENGTH - 3; i++) {
    if (pass[i] != pass[i + 1]) {
      continue;
    }
    // found one pair
    for (int j = i + 2; j < PASS_LENGTH - 1; j++) {
      if (pass[j] == pass[j + 1]) {
        // found other pair
        return 1;
      }
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char password[PASS_LENGTH + 1];
  fgets(password, PASS_LENGTH + 1, fp);
  fclose(fp);
  while (!is_valid(password)) {
    to_string(password, to_number(password) + 1);
  }
  printf("1st new password: %s\n", password);
  to_string(password, to_number(password) + 1);
  while (!is_valid(password)) {
    to_string(password, to_number(password) + 1);
  }
  printf("2nd new password: %s\n", password);
  return 0;
}
