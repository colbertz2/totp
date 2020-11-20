#include <stdlib.h>
#include <string.h>

#include "qr.h"

const char B32_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
const char B32_PAD = '=';

char *b32_encode(const char *d, size_t d_len) {
  char *b32;
  unsigned char buf;
  int b32_len, i, j, rem, next, b32Index;

  b32_len = (8 * d_len) / 5;  // base32 output is groups of 5 bits

  b32 = malloc(b32_len + 1);
  memset(b32, '\0', b32_len + 1);

  rem = 8;   // # of bits remaining at current byte
  next = 0;  // # of bits to get from big end of next byte
  j = 0;     // Index for input bytes
  for (i = 0; i < b32_len; i++) {
    buf = *(d + j);
    b32Index = 0;

    if (rem >= 5) {  // Get all 5 base32 bits from the current input byte
      buf &= (0xff >> next);  // Mask big-end bits that have already been used
      rem -= 5;
      b32Index = buf >> rem;
    } else {
      next = 5 - rem;
      buf &= (0xff >> (8 - rem));  // Mask to get little-end bits
      b32Index = buf << next;      // Stash most significant bits
      buf = *(d + (++j));
      rem = 8 - next;
      buf &= (0xff ^ (0xff >> next));  // Mask to get the big-end bits
      b32Index |= buf >> rem;          // Shift buf bits to little-end and stash
    }

    b32[i] = B32_ALPHABET[b32Index];
  }

  return b32;
}

char *b32_decode(const char *b32, size_t b32_len) {
  char *ascii;
  unsigned char buf;
  int a_len, i, j, rem;

  a_len = (5 * b32_len) / 8;

  ascii = malloc(a_len + 1);
  memset(ascii, '\0', a_len + 1);

  rem = 8;  // # of bits that still need to be populated in this output byte
  j = 0;    // ascii index
  for (i = 0; i < b32_len; i++) {  // Loop through base32 bytes
    buf = strchr(B32_ALPHABET, b32[i]) - B32_ALPHABET;

    if (rem >= 5) {
      rem -= 5;
      ascii[j] |= buf << rem;  // fill the most significant open bits
    } else {
      ascii[j++] |= buf >> (5 - rem);  // fill ascii byte with the (rem) MSb's
      buf &= (0xff >> (3 + rem));      // mask away the bits we already used
      rem = 8 - (5 - rem);             // new ascii byte, reset
      ascii[j] = buf << rem;           // fill the most sig end of ascii byte
    }
  }

  return ascii;
}