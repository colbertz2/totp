// Tests for qr.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qr.h"

int main() {
  const char test_string[] = {'H',  'e',  'l',  'l',  'o', '!',
                              0xDE, 0xAD, 0xBE, 0xEF, '\0'};
  const char test_base32[] = "JBSWY3DPEHPK3PXP";
  char *c, *b32 = b32_encode(test_string, 10);
  int result;
  // qr_generate();
  printf("=== ENCODE Base32 ===\n");
  printf("Base32 Encoding: %s\n", b32);
  printf("Test Vector    : %s\n", test_base32);

  result = strcmp(test_base32, b32);
  if (result == 0) {
    printf("Pass!\n");
  } else {
    printf("Fail!\n");
  }
  printf("\n");

  printf("=== DECODE Base32 ===\n");
  free(b32);
  b32 = b32_decode(test_base32, strlen(test_base32));
  printf("ASCII Encoding: %s\n", b32);
  printf("Test Vector   : %s\n", test_string);

  result = strcmp(test_string, b32);
  if (result == 0) {
    printf("Pass!\n");
  } else {
    printf("Fail!\n");
  }
  printf("\n");

  free(b32);
  return 0;
}