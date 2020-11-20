// Tests for base32.c

#include <stdio.h>
#include <string.h>

#include "otp.h"

// Compare encoding/decoding of string d by function (*fun_ptr) to string cmp.
// Returns 0 if pass, non-zero if fail.
int test(char* (*fun_ptr)(const char*, size_t), const char* d,
         const char* cmp) {
  int result;
  char* b32 = (*fun_ptr)(d, strlen(d));

  result = strcmp(cmp, b32);
  free(b32);
  return result;
}

// Test base32 encoding/decoding.
// Returns 0 if pass, non-zero if fail.
int main() {
  const char test_string[] = {'H',  'e',  'l',  'l',  'o', '!',
                              0xDE, 0xAD, 0xBE, 0xEF, '\0'};
  const char test_base32[] = "JBSWY3DPEHPK3PXP";
  int enc = -5, dec = -5;

  // ENCODE Base32
  enc = test(&b32_encode, test_string, test_base32);

  // DECODE Base32
  dec = test(&b32_decode, test_base32, test_string);

  return (enc && dec);
}