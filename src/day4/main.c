#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void md5_hash(uint8_t output[16], const void *message, size_t size) {
  // "derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm"
  // ported from the dotnet source code
  uint32_t shifts[16] = {7, 12, 17, 22, 5, 9,  14, 20,
                         4, 11, 16, 23, 6, 10, 15, 21};

  uint32_t sines[64] = {
      0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
      0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
      0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
      0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
      0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
      0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
      0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
      0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
      0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
      0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
      0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

  uint32_t blocks = 1 + (size + 8) / 64;
  uint8_t *block = (uint8_t *)message;
  uint32_t aa = 0x67452301, bb = 0xefcdab89, cc = 0x98badcfe, dd = 0x10325476;

  for (int i = 0; i < blocks; i++) {
    uint32_t offset = 64 * i;
    if (offset + 64 > size) {
      block = malloc(64 * sizeof(uint8_t));
      for (int j = offset; j < size; j++) {
        block[j - offset] = ((uint8_t *)message)[j];
      }
      if (offset <= size) {
        block[size - offset] = 0x80;
      }
      if (i == blocks - 1) {
        block[56] = (uint8_t)(size << 3);
        block[57] = (uint8_t)(size >> 5);
        block[58] = (uint8_t)(size >> 13);
        block[59] = (uint8_t)(size >> 21);
      }
      offset = 0;
    }
    uint32_t a = aa, b = bb, c = cc, d = dd;
    uint32_t f, g;
    for (int j = 0; j < 64; j++) {
      if (j < 16) {
        f = b & c | ~b & d;
        g = j;
      } else if (j < 32) {
        f = b & d | c & ~d;
        g = 5 * j + 1;
      } else if (j < 48) {
        f = b ^ c ^ d;
        g = 3 * j + 5;
      } else {
        f = c ^ (b | ~d);
        g = 7 * j;
      }
      g = (g & 0x0f) * 4 + offset;
      uint32_t hold = d;
      d = c;
      c = b;
      b = a + f + sines[j] +
          (uint32_t)(block[g] + (block[g + 1] << 8) + (block[g + 2] << 16) +
                     (block[g + 3] << 24));
      b = b << (shifts[j & 3 | j >> 2 & ~3]) |
          b >> (32 - shifts[j & 3 | j >> 2 & ~3]);
      b += c;
      a = hold;
    }
    aa += a;
    bb += b;
    cc += c;
    dd += d;
  }
  free(block);
  for (int i = 0; i < 4; i++) {
    output[0 + i] = aa;
    output[4 + i] = bb;
    output[8 + i] = cc;
    output[12 + i] = dd;
    aa >>= 8;
    bb >>= 8;
    cc >>= 8;
    dd >>= 8;
  }
}

#define BUFFER_LENGTH 256

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char buffer[BUFFER_LENGTH];
  fgets(buffer, BUFFER_LENGTH, fp);
  buffer[strlen(buffer) - 1] = '\0';

  uint8_t hash[16] = {1};
  uint32_t salt = 0;
  char message[BUFFER_LENGTH];
  for (; hash[0] || hash[1] || hash[2] > 0x0F; salt++) {
    sprintf(message, "%s%u", buffer, salt);
    md5_hash(hash, message, strlen(message));
  }
  printf("five zeroes: %d\n", salt);
  for (; hash[0] || hash[1] || hash[2]; salt++) {
    sprintf(message, "%s%u", buffer, salt);
    md5_hash(hash, message, strlen(message));
  }
  printf("six zeroes: %d\n", salt);
  return 0;
}
