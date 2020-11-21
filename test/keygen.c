// Tests for otp/keygen.c

#include <string.h>

#include "otp.h"

int main() {
  char *key = otp_keygen();

  if (key != NULL) {
    if (strlen(key) == OTP_KEY_LEN) {
      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}