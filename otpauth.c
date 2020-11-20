// Generates otpauth uri

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "otp.h"

// Convert strings to ABNF format.
// NOTE: This implementation only replaces space characters.
char* otp_abnfify(const char* in) {
  int out_len;
  char* out;
  const char* cursor;

  // Calculate length of output string
  out_len = strlen(in);
  cursor = in;
  while (*cursor != '\0') {
    if (*cursor == 0x20) {
      out_len += 2;
    }
    cursor++;
  }

  out = calloc(out_len + 1, sizeof(char));
  memset(out, '\0', out_len + 1);

  // Replace spaces with ABNF representation
  cursor = in;
  while (*cursor != '\0') {
    if (*cursor == 0x20) {
      strcpy(strchr(out, '\0'), "%20");
    } else {
      memcpy(strchr(out, '\0'), cursor, 1);
    }
    cursor++;
  }

  return out;
}

// Resolve enum OTP_TYPE to string.
char* otp_type_resolve(int type) {
  int t_len = 4;
  char* t = calloc(t_len + 1, sizeof(char));
  memset(t, '\0', t_len + 1);

  switch (type) {
    case OTP_HOTP:
      strcpy(t, "hotp");
      break;
    case OTP_TOTP:
      strcpy(t, "totp");
      break;
    default:
      free(t);
      errno = EINVAL;
      return NULL;
  }

  return t;
}

// Resolve enum OTP_ALG to string.
char* otp_alg_resolve(int algorithm) {
  int alg_len = 6;
  char* alg = calloc(alg_len + 1, sizeof(char));
  memset(alg, '\0', alg_len + 1);

  switch (algorithm) {
    case OTP_SHA512:
      strcpy(alg, "SHA512");
      break;
    case OTP_SHA256:
      strcpy(alg, "SHA256");
      break;
    case OTP_SHA1:
      strcpy(alg, "SHA1");
      break;
    default:
      free(alg);
      errno = EINVAL;
      return NULL;
  }

  return alg;
}

char* otp_uri(int auth_type, const char* issuer, const char* account_name,
              const char* secret, int algorithm, int digits, int counter) {
  int i, counter_len, uri_len;
  char *type_str, *issuer_abnf, *account_abnf, *secret_b32, *alg_str,
      *uri = NULL;

  // Documented default arguments
  algorithm = OTP_SHA1;
  if (digits != 8) {
    digits = 6;
  }

  // input validation
  if ((auth_type != OTP_HOTP && auth_type != OTP_TOTP) ||  // Validate auth type
      (auth_type == OTP_HOTP && counter < 0)) {  // Require counter > 0 for HOTP
    errno = EINVAL;
    return uri;
  }

  // Prohibit colons in the issuer or account name strings
  if (strchr(issuer, ':') || strchr(account_name, ':')) {
    errno = EINVAL;
    return uri;
  }

  // Input encoding
  type_str = otp_type_resolve(auth_type);
  if (type_str == NULL) {
    goto TYPE_ERROR;  // cleanup and return NULL on error
  }

  issuer_abnf = otp_abnfify(issuer);
  if (issuer_abnf == NULL) {
    goto ISSUER_ERROR;  // cleanup and return NULL on error
  }

  account_abnf = otp_abnfify(account_name);
  if (account_abnf == NULL) {
    goto ACCOUNT_ERROR;  // cleanup and return NULL on error
  }

  secret_b32 = b32_encode(secret, strlen(secret));
  if (secret_b32 == NULL) {
    goto SECRET_ERROR;  // cleanup and return NULL on error
  }

  alg_str = otp_alg_resolve(algorithm);
  if (alg_str == NULL) {
    goto ALGORITHM_ERROR;  // cleanup and return NULL on error
  }

  // Calculate uri length
  uri_len = 0;
  uri_len += strlen("otpauth://xotp/");
  uri_len += strlen(issuer_abnf) + 1;
  uri_len += strlen(account_abnf) + 1;
  uri_len += strlen("secret=") + strlen(secret_b32);
  uri_len += strlen("&issuer=") + strlen(issuer_abnf);
  uri_len += strlen("&algorithm=") + strlen(alg_str);
  uri_len += strlen("&digits=") + 1;

  if (auth_type == OTP_HOTP) {
    counter_len = 0;
    i = counter;
    while (i != 0) {
      i /= 10;
      ++counter_len;
    }

    uri_len += strlen("&counter=") + counter_len;
  }

  // Construct null-terminated uri
  uri = calloc(uri_len + 1, sizeof(char));
  memset(uri, '\0', uri_len + 1);

  if (sprintf(uri,
              "otpauth://%s/%s:%s?secret=%s&issuer=%s&algorithm=%s&digits=%d",
              type_str, issuer_abnf, account_abnf, secret_b32, issuer_abnf,
              alg_str, digits) < 0) {
    // cleanup and return NULL on error
    free(uri);
    uri = NULL;
    goto CLEANUP_ALL;
  }

  if (auth_type == OTP_HOTP) {
    if (sprintf(uri, "&counter=%d", counter) < 0) {
      // cleanup and return NULL on error
      free(uri);
      uri = NULL;
    }
  }

CLEANUP_ALL:
ALGORITHM_ERROR:
  free(alg_str);

SECRET_ERROR:
  free(secret_b32);

ACCOUNT_ERROR:
  free(account_abnf);

ISSUER_ERROR:
  free(issuer_abnf);

TYPE_ERROR:
  free(type_str);

  return uri;
}