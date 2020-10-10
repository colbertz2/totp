/***********************************************
 * submission.c - Main program for TOTP demo
 * 
 * SYNOPSIS
 *      ./submission --generate-qr | --get-otp
 * 
 * AUTHOR
 *      Zach Colbert <colbertz@oregonstate.edu>
 ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "qr.h"
#include "otp.h"

void _usage(const char* exe) {
    printf("Usage: %s --generate-qr | --get-otp\n", exe);
    exit(1);
}

int main(int argc, char *argv[]) {
    int opt = 0;
    
    if (argc < 2) { 
        _usage(argv[0]);
    }

    // List the expected command line options
    static struct option long_options[] = {
        {"generate-qr", no_argument, 0, 0},
        {"get-otp", no_argument, 0, 1},
        {0, 0, 0, 0}
    };

    // Get command-line option and call appropriate branch function
    opt = getopt_long(argc, argv, "01", long_options, NULL);
    switch (opt) {
        case 0:
            qr_generate();
            break;
        case 1:
            get_otp();
            break;
        default: _usage(argv[0]);
    }

    return 0;
}