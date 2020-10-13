// Header file for qr.c

#include <stdlib.h>

int qr_generate();

// Encode bytes as base32 string.
// NOTE: Does not include '=' padding.
char* b32_encode(const char*, size_t);

// Decode base32 string and return bytes.
// NOTE: Can handle '=' padding.
char* b32_decode(const char*, size_t);