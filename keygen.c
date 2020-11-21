// Key generation function for OTP

#include <openssl/err.h>
#include <openssl/rand.h>
#include <string.h>

#include "otp.h"

char *otp_keygen() {
  static char key[sizeof(char) * (OTP_KEY_LEN + 1)];
  memset(key, '\0', sizeof(char) * (OTP_KEY_LEN + 1));

  // Generate random bytes for key
  if (RAND_priv_bytes(key, OTP_KEY_LEN) != 1) {
    // Warn users if bytes aren't sufficiently random
    fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), 0));
  }

  return key;
}