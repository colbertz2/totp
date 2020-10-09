// Test app for otp.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "otp.h"
const unsigned char k[] = "12345678901234567890";
const int COL_WIDTH = 11;
const char HEAD_FMT[] = "| %*s | %*s | %*s | %*s |\n";
const char ROW_FMT[] = "| %*lu | %*d | %*d | %*s |\n";
const int T4_WIDTH = (4 * COL_WIDTH) + (4 * 2) + 5;

void t4_header(const char *, const char *, const char *, const char *);
void t4_row(long, int, int, const char *);
void t4_line();

int main() {
  const int n_times = 6;
  const int n_counts = 10;
  const int test_counts[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const long test_times[] = {59,         1111111109, 1111111111,
                             1234567890, 2000000000, 20000000000};
  const int test_hotps[] = {755224, 287082, 359152, 969429, 338314,
                            254676, 287922, 162583, 399871, 520489};
  const int test_totps[] = {94287082, 7081804,  14050471,
                            89005924, 69279037, 65353130};
  unsigned char *test_key = (unsigned char *)"12345678901234567890";

  int i, val;
  char *pass_str;

  // HOTP TEST VECTORS
  printf("=== HOTP TESTS ===\n");
  t4_header("Count", "Test Val", "Result", "P/F");
  for (i = 0; i < n_counts; i++) {
    val = hotp6(test_key, test_counts[i]);

    if (val == test_hotps[i]) {
      pass_str = "PASS";
    } else {
      pass_str = "FAIL";
    }

    t4_row(test_counts[i], test_hotps[i], val, pass_str);
  }
  t4_line();

  // TOTP TEST VECTORS
  printf("=== TOTP TESTS ===\n");
  t4_header("Time", "Test Val", "Result", "P/F");
  for (i = 0; i < n_times; i++) {
    val = totp8(test_key, test_times[i]);

    if (val == test_totps[i]) {
      pass_str = "PASS";
    } else {
      pass_str = "FAIL";
    }

    t4_row(test_times[i], test_totps[i], val, pass_str);
  }
  t4_line();

  return 0;
}

void t4_header(const char *a, const char *b, const char *c, const char *d) {
  t4_line();
  printf(HEAD_FMT, COL_WIDTH, a, COL_WIDTH, b, COL_WIDTH, c, COL_WIDTH, d);
  t4_line();
}

void t4_row(long a, int b, int c, const char *d) {
  printf(ROW_FMT, COL_WIDTH, a, COL_WIDTH, b, COL_WIDTH, c, COL_WIDTH, d);
}

void t4_line() {
  int i;

  for (i = 0; i < T4_WIDTH; i++) {
    printf("-");
  }
  printf("\n");
}