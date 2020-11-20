// Tests for otp/otpauth.c

#include <stdio.h>
#include <string.h>

#include "otp.h"

void print_result(const char* func, int result) {
  fprintf(stderr, "%s : ", func);
  if (result) {
    fprintf(stderr, "PASS\n");
  } else {
    fprintf(stderr, "FAIL\n");
  }
}

int test_otp_type_resolve() {
  const int n_types = 2;
  const int in_types[] = {OTP_HOTP, OTP_TOTP};
  const char* test_types[] = {"hotp", "totp"};
  char* out_types;
  int result_types, i;

  result_types = 0 == 0;
  for (i = 0; i < n_types; i++) {
    out_types = otp_type_resolve(in_types[i]);
    result_types = result_types && (0 == strcmp(out_types, test_types[i]));
    free(out_types);
  }

  print_result("otp_type_resolve", result_types);
  return result_types;
}

int test_otp_alg_resolve() {
  const int n_algs = 3;
  const int in_algs[] = {OTP_SHA1, OTP_SHA256, OTP_SHA512};
  const char* test_algs[] = {"SHA1", "SHA256", "SHA512"};
  char* out_algs;
  int result_algs, i;

  result_algs = 0 == 0;
  for (i = 0; i < n_algs; i++) {
    out_algs = otp_alg_resolve(in_algs[i]);
    result_algs = result_algs && (0 == strcmp(out_algs, test_algs[i]));
    free(out_algs);
  }

  print_result("otp_alg_resolve", result_algs);
  return result_algs;
}

int test_otp_abnfify() {
  const char* in_abnf = "This is my string.";
  const char* test_abnf = "This%20is%20my%20string.";
  char* out_abnf;
  int result_abnf;

  out_abnf = otp_abnfify(in_abnf);
  result_abnf = 0 == strcmp(out_abnf, test_abnf);
  free(out_abnf);

  print_result("otp_abnfify", result_abnf);
  return result_abnf;
}

int main() {
  int type, alg, abnf;

  type = test_otp_type_resolve();
  alg = test_otp_alg_resolve();
  abnf = test_otp_abnfify();

  if (type && alg && abnf) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}