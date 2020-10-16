// Test app for otp.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "otp.h"

// Compares the OTP value returned by (*fun_ptr)
// given shared secret k and counter d.
//
// Returns 0 on pass, non-zero on fail.
int test(int (*fun_ptr)(const unsigned char *, uint64_t),
         const unsigned char *k, uint64_t d, int cmp) {
  int otp, result;
  otp = (*fun_ptr)(k, d);
  result = (cmp == otp);
  return result;
}

int main() {
  const int n_counts = 10;
  const int n_times = 6;
  const int n_funcs = 2;
  const int test_counts[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const long test_times[] = {59,         1111111109, 1111111111,
                             1234567890, 2000000000, 20000000000};
  const int test_hotps[] = {755224, 287082, 359152, 969429, 338314,
                            254676, 287922, 162583, 399871, 520489};
  const int test_totps[] = {94287082, 7081804,  14050471,
                            89005924, 69279037, 65353130};
  unsigned char *test_key = (unsigned char *)"12345678901234567890";

  int i, result;
  int hotp_results[n_counts];
  int totp_results[n_times];
  int all_results[n_funcs];

  // Test hotp() function for all test_counts values
  all_results[0] = 1;
  for (i = 0; i < n_counts; i++) {
    hotp_results[i] = test(&hotp, test_key, test_counts[i], test_hotps[i]);
    all_results[0] =
        all_results[0] &&
        hotp_results[i];  // AND results for all test values together
  }

  // Test totp() function for all test_times values
  all_results[1] = 1;
  for (i = 0; i < n_times; i++) {
    totp_results[i] = test(&totp, test_key, test_times[i], test_totps[i]);
    all_results[1] = all_results[1] && totp_results[i];
  }

  // AND all test results together
  result = 1;
  for (i = 0; i < n_funcs; i++) {
    result = result && all_results[i];
  }

  return result;
}