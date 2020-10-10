// Header file for otp.c

#include <stdint.h>

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