// Tests for otp.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#include "otp.h"

int main() {
    int i;
    unsigned long totp;
    unsigned char *digest;
    unsigned char trunc[9];
    memset(trunc, '\0', 9 * sizeof(char));

    // unsigned char* K = _otp_read_key();
    unsigned char* K = _otp_read_key();
    printf("K = %s\n", K);

    // unsigned char* T = _otp_get_time();
    unsigned char* T = _otp_get_time();
    printf("T = %s\n", T);

    digest = HMAC(EVP_sha1(),
                K, sizeof(K) - 1,
                T, sizeof(T) - 1,
                NULL, NULL);

    printf("HMAC(K, T) = ");
    for (i = 0; i < strlen(digest); i++) {
        printf("%x ", (int) digest[i]);
    }
    printf("\n");

    // Truncate HMAC result
    i = digest[19] & 0xf;

    sprintf(trunc, "%x%x%x%x",
            (int)(digest[i]   & 0x7f),
            (int)(digest[i+1] & 0xff),
            (int)(digest[i+2] & 0xff),
            (int)(digest[i+3] & 0xff));
    printf("TRUNC = %s\n", trunc);

    // memset(trunc, digest[i] & 0x7f, 1);
    // memset(trunc + 1, digest[i+1] & 0xff, 1);
    // memset(trunc + 2, digest[i+2] & 0xff, 1);
    // memset(trunc + 3, digest[i+3] & 0xff, 1);
    // printf("TRUNC = ");
    // for (i = 0; i < strlen(trunc); i++) {
    //     printf("%x ", (int) trunc[i]);
    // }
    // printf("\n");

    // Cast to unsigned int and take mod 10^6
    // totp = (unsigned int) trunc % 1000000;
    totp = strtoul(trunc, NULL, 16);
    printf("TRUNC (ulong) = %lu\n", totp);

    totp = totp % 1000000;
    printf("TOTP = %lu\n", totp);

    free(K);
    free(T);
    // free(digest);
    return 0;
}