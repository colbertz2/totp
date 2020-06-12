// Tests for otp.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "otp.h"

int main() {
    char* key = _otp_read_key();
    printf("%s\n", key);

    char* T = _otp_get_time();
    printf("%s\n", T);

    get_otp();

    free(key);
    return 0;
}