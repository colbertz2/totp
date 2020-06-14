/**********************************************************
 * otp.c - Calculates TOTP of key stored in "secret" file
 * 
 * AUTHOR
 *      Zach Colbert <colbertz@oregonstate.edu>
 **********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <openssl/hmac.h>

#include "otp.h"

#define KEY_LENGTH 32

int get_otp() {
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

    // HMAC seems to be returning the same digest
    // for different messages (T) with the same key
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

    // Copy byte values (hex) into a string literal
    sprintf(trunc, "%x%x%x%x",
            (int)(digest[i]   & 0x7f),
            (int)(digest[i+1] & 0xff),
            (int)(digest[i+2] & 0xff),
            (int)(digest[i+3] & 0xff));
    printf("TRUNC = %s\n", trunc);

    // Cast to unsigned long and take mod 10^6
    totp = strtoul(trunc, NULL, 16);
    printf("TRUNC (ulong) = %lu\n", totp);

    totp = totp % 1000000;
    printf("TOTP = %lu\n", totp);

    free(K);
    free(T);
    // free(digest);
    return 0;
}

unsigned char* _otp_read_key() {
    int fd;
    int buf_size = (KEY_LENGTH + 1) * sizeof(char);
    unsigned char* buffer = calloc(KEY_LENGTH + 1, sizeof(char));
    unsigned char* key = calloc(KEY_LENGTH + 1, sizeof(char));

    memset(buffer, '\0', buf_size);
    memset(key, '\0', buf_size);

    // Open key file
    fd = open("secret", O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(2);
    }

    // Read key from key file into "key" string
    while (read(fd, buffer, buf_size - 1) > 0) {
        strcat(key, buffer);
        memset(buffer, '\0', buf_size);
    }

    free(buffer);
    return key;
}

unsigned char* _otp_get_time() {
    time_t T, tsec = time(NULL);
    unsigned char* result = calloc(20, sizeof(char));
    memset(result, '\0', 20 * sizeof(char));

    // T = (time - T0) / X
    // X = 30 sec
    T = tsec / 30;

    // Convert to hex string
    sprintf(result, "%016lx", (long int) T);
    // fprintf(stderr, "%016lx\n", (long int) T);      // @DEV
    return result;
}