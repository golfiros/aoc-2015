#include <stdint.h>
#include <stdio.h>

static const int s[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
};
static const uint32_t k[] = {
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

void md5(char output[16], const void *p, long long n) {
  long long l = (n + 8) / 64 + 1;
  uint32_t a0 = 0x67452301, b0 = 0xefcdab89, c0 = 0x98badcfe, d0 = 0x10325476;

  for (long long i = 0; i < l; i++) {
    long long t = i * 64;
    uint8_t p0[64] = {};
    const uint8_t *q;
    if (i + 1 > n / 64) {
      for (int j = t; j < n; j++)
        p0[j - t] = ((uint8_t *)p)[j];
      if (i <= n / 64)
        p0[n - t] = 0x80;
      if (i == l - 1) {
        long long m = n * 8;
        for (int j = 56; j < 64; j++, m /= (1 << 8))
          p0[j] = m % (1 << 8);
      }
      q = p0;
    } else
      q = (const uint8_t *)p + t;

    uint32_t a = a0, b = b0, c = c0, d = d0, m[16];
    for (int j = 0; j < 16; j++)
      m[j] = (q[4 * j + 0] << (0 * 8)) + (q[4 * j + 1] << (1 * 8)) +
             (q[4 * j + 2] << (2 * 8)) + (q[4 * j + 3] << (3 * 8));
    for (int j = 0; j < 4 * 16; j++) {
      uint32_t f, g;
      if (j / 16 < 1)
        f = b & c | ~b & d, g = j % 16;
      else if (j / 16 < 2)
        f = b & d | c & ~d, g = (5 * j + 1) % 16;
      else if (j / 16 < 3)
        f = b ^ c ^ d, g = (3 * j + 5) % 16;
      else
        f = c ^ (b | ~d), g = (7 * j) % 16;
      f = f + a + k[j] + m[g];
      a = d, d = c, c = b;
      b = b + (f << (s[j]) | f >> (32 - s[j]));
    }
    a0 += a;
    b0 += b;
    c0 += c;
    d0 += d;
  }
  for (int i = 0; i < 4; i++) {
    output[0 + i] = a0, a0 >>= 8;
    output[4 + i] = b0, b0 >>= 8;
    output[8 + i] = c0, c0 >>= 8;
    output[12 + i] = d0, d0 >>= 8;
  }
}

int main(int argc, char **argv) {
  char str[16], m[16];
  int s = 0;
  char h[16] = {};
  scanf("%s", str);
  do
    md5(h, m, sprintf(m, "%s%d", str, ++s));
  while (h[0] || h[1] || (unsigned char)h[2] > 0x0F);
  printf("five zeroes: %d\n", s);
  do
    md5(h, m, sprintf(m, "%s%d", str, ++s));
  while (h[0] || h[1] || h[2]);
  printf("six zeroes: %d\n", s);
}
