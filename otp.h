// Header file for otp.c

#include <stdint.h>
#include <stdlib.h>

#define OTP_KEY_LEN 20
#define TOTP_TIME_STEP 30

enum OTP_TYPE { OTP_HOTP, OTP_TOTP };
enum OTP_ALG { OTP_SHA1, OTP_SHA256, OTP_SHA512 };

// Generates a random key of size OTP_KEY_LEN for use in OTP protocols.
// NOTE: Key is a null-terminated string of random bytes.
char* otp_keygen();

// @TODO Docs
char* otp_abnfify(const char* in);

// @TODO Docs
char* otp_type_resolve(int type);

// @TODO Docs
char* otp_alg_resolve(int algorithm);

// Returns the otpauth URI compiled from the given arguments.
//
// @param auth_type     use enum OTP_TYPE
// @param issuer        name of provider or service owning this account
// @param account_name  user account name, email address, etc.
// @param secret        arbitrary key value of length OTP_KEY_LEN
// @param algorithm     ignored, use enum OTP_ALG (default SHA-1)
// @param digits        number of OTP output digits, choose 6 or 8 (default 6)
// @param counter       initial counter value for HOTP (default 0);
//                      ignored for TOTP
char* otp_uri(int auth_type, const char* issuer, const char* account_name,
              const char* secret, int algorithm, int digits, int counter);

// Encode bytes as base32 string.
// NOTE: Does not include '=' padding.
char* b32_encode(const char*, size_t);

// Decode base32 string and return bytes.
// NOTE: Can handle '=' padding.
char* b32_decode(const char*, size_t);

// Returns the 6-digit TOTP at Unix time t using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
// Alias for totp6()
int totp(const unsigned char* k, uint64_t t);

// Returns the 6-digit TOTP at Unix time t using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
int totp6(const unsigned char* k, uint64_t t);

// Returns the 8-digit TOTP at Unix time t using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
int totp8(const unsigned char* k, uint64_t t);

// Alias for hotp6()
// Returns the 6-digit HOTP for counter c using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
//       k MUST BE null-terminated.
int hotp(const unsigned char* k, uint64_t c);

// Returns the 6-digit HOTP for counter c using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
//       k MUST BE null-terminated.
int hotp6(const unsigned char* k, uint64_t c);

// Returns the 8-digit HOTP for counter c using shared secret k.
//
// NOTE: k should be ASCII encoded, not base32 as in the OTP URI.
int hotp8(const unsigned char* k, uint64_t c);

// Print a 6-digit OTP to stdout.
void otp_print6(int otp);

// Alias for otp_print6()
// Print a 6-digit HOTP to stdout.
void hotp_print6(int hotp);

// Alias for otp_print6()
// Print a 6-digit TOTP to stdout.
void totp_print6(int totp);

// Alias for otp_print6()
// Print a 6-digit OTP to stdout.
void otp_print(int otp);

// Print a 8-digit OTP to stdout.
void otp_print8(int otp);

// Alias for otp_print8()
// Print a 8-digit HOTP to stdout.
void hotp_print8(int hotp);

// Alias for otp_print8()
// Print a 8-digit TOTP to stdout.
void totp_print8(int totp);