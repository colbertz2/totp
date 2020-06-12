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
    unsigned char* K, T;
    unsigned int digest_len;
    unsigned char digest[EVP_MAX_MD_SIZE];

    // Read key from keyfile
    K = _otp_read_key();

    // Get time value T
    T = _otp_get_time();

    // Can't get HMAC working before submission deadline
    fprintf(stderr, "DEBUG: K %s\n", K);
    fprintf(stderr, "DEBUG: T %s\n", T);

    /***
    // Run HMAC
    HMAC(EVP_sha1(),
         K, strlen(K),
         T, strlen(T),
         digest, &digest_len);
    ***/

    // Truncate HMAC result
    // printf("%s\n", digest);

    free(K);
    free(T);
    return 0;
}

char* _otp_read_key() {
    int fd;
    int buf_size = (KEY_LENGTH + 1) * sizeof(char);
    char* buffer = calloc(KEY_LENGTH + 1, sizeof(char));
    char* key = calloc(KEY_LENGTH + 1, sizeof(char));

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

char* _otp_get_time() {
    time_t T, tsec = time(NULL);
    char* result = calloc(17, sizeof(char));
    memset(result, '\0', 17 * sizeof(char));

    // T = (time - T0) / X
    // X = 30 sec
    T = tsec / 30;

    // Convert to hex string
    sprintf(result, "%016lx\n", (long int) T);
    return result;
}