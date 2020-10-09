/**********************************************************
 * otp.c - Calculates TOTP of key stored in "secret" file
 *
 * AUTHOR
 *      Zach Colbert <colbertz@oregonstate.edu>
 **********************************************************/

#include "otp.h"

#include <endian.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <string.h>

#define TOTP_TIME_STEP 30;  // Defined in TOTP standard

unsigned char* hotp_hmac_sha1(const unsigned char* k, int k_len, uint64_t c);
int hotp_trunc6(const unsigned char* d, int d_len);
int hotp_trunc8(const unsigned char* d, int d_len);

// Alias for totp6()
// Returns the 6-digit TOTP at Unix time t using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
int totp(const unsigned char* k, uint64_t t) { return totp6(k, t); }

// Returns the 6-digit TOTP at Unix time t using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
int totp6(const unsigned char* k, uint64_t t) {
  uint64_t timeCounter = t / TOTP_TIME_STEP;
  return hotp(k, timeCounter);
}

// Returns the 8-digit TOTP at Unix time t using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
int totp8(const unsigned char* k, uint64_t t) {
  uint64_t timeCounter = t / TOTP_TIME_STEP;
  return hotp8(k, timeCounter);
}

// Alias for hotp6()
// Returns the 6-digit HOTP for counter c using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
//       k MUST BE null-terminated.
int hotp(const unsigned char* k, uint64_t c) { return hotp6(k, c); }

// Returns the 6-digit HOTP for counter c using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
//       k MUST BE null-terminated.
int hotp6(const unsigned char* k, uint64_t c) {
  unsigned char* hmac;
  int k_len, hotp;

  k_len = strlen((char*)k);

  hmac = hotp_hmac_sha1(k, k_len, c);
  hotp =
      hotp_trunc6(hmac, 20);  // 20 is the standard length of HMAC-SHA-1 output

  return hotp;
}

// Returns the 8-digit HOTP for counter c using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
//       k MUST BE null-terminated.
int hotp8(const unsigned char* k, uint64_t c) {
  unsigned char* hmac;
  int k_len, hotp;

  k_len = strlen((char*)k);

  hmac = hotp_hmac_sha1(k, k_len, c);
  hotp =
      hotp_trunc8(hmac, 20);  // 20 is the standard length of HMAC-SHA-1 output

  return hotp;
}

// Returns the HMAC-SHA-1 digest of counter c using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
// NOTE: This function takes the counter as a uint64_t, converts it to
//       little-endian format before providing the bytes to OpenSSL's
//       HMAC implementation.
unsigned char* hotp_hmac_sha1(const unsigned char* k, int k_len, uint64_t c) {
  // Union enables us to read uint64 bytes as unsigned char bytes
  union Count {
    uint64_t l;
    unsigned char ch[sizeof(uint64_t)];
  };

  union Count countBigEndian;
  size_t countSize;
  unsigned char* digest;

  // HOTP test vectors indicate that the counter is HMAC'd in Big-Endian
  countBigEndian.l = htobe64(c);  // Convert counter to Big-Endian bytes
  countSize = sizeof(uint64_t);

  digest = HMAC(EVP_sha1(), k, k_len, countBigEndian.ch, countSize, NULL, NULL);
  return digest;
}

// Dynamically truncates HMAC bits and returns 6-digit HOTP
int hotp_trunc6(const unsigned char* d, int d_len) {
  int offset, bin_code;

  offset = d[d_len - 1] & 0xf;
  bin_code = (d[offset] & 0x7f) << 24 | (d[offset + 1] & 0xff) << 16 |
             (d[offset + 2] & 0xff) << 8 | (d[offset + 3] & 0xff);

  return (bin_code % 1000000);
}

// Dynamically truncates HMAC bits and returns 8-digit HOTP
int hotp_trunc8(const unsigned char* d, int d_len) {
  int offset, bin_code;

  offset = d[d_len - 1] & 0xf;
  bin_code = (d[offset] & 0x7f) << 24 | (d[offset + 1] & 0xff) << 16 |
             (d[offset + 2] & 0xff) << 8 | (d[offset + 3] & 0xff);

  return (bin_code % 100000000);
}

// Print a 6-digit OTP to stdout.
// For convenience, because remembering format specifiers is hard sometimes.
void otp_print6(int otp) { printf("%06d", otp); }
void hotp_print6(int hotp) { otp_print6(hotp); }
void totp_print6(int totp) { otp_print6(totp); }
void otp_print(int otp) { otp_print6(otp); }

// Print a 8-digit OTP to stdout.
// For convenience, because remembering format specifiers is hard sometimes.
void otp_print8(int otp) { printf("%08d", otp); }
void hotp_print8(int hotp) { otp_print8(hotp); }
void totp_print8(int totp) { otp_print8(totp); }