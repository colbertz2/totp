/***********************************************
 * main.c - Main program for TOTP demo
 *
 * SYNOPSIS
 *      totp <subcommand>
 *
 * AUTHOR
 *      Zach Colbert <colbertz@oregonstate.edu>
 ***********************************************/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "otp.h"
#include "qr.h"

#define EXE_NAME "totp"

typedef struct {
  char* name;
  char* description;
  char* help;
  int (*function)(char**);
} SubCommand;

void usage() {
  fprintf(stderr, "usage: %s <command>\n", EXE_NAME);
  exit(EXIT_FAILURE);
}

int getcmd() {
  const char* func = "getcmd";
  fprintf(stderr, "%s: %s not yet implemented\n", EXE_NAME, func);
  return -5;
}

int keygen(char** argv) {
  const char* func = "keygen";
  fprintf(stderr, "%s: %s not yet implemented\n", EXE_NAME, func);
  return -5;
}
int qrgen(char** argv) {
  const char* func = "qrgen";
  fprintf(stderr, "%s: %s not yet implemented\n", EXE_NAME, func);
  return -5;
}
int code(char** argv) {
  const char* func = "code";
  fprintf(stderr, "%s: %s not yet implemented\n", EXE_NAME, func);
  return -5;
}
int test(char** argv) {
  const char* func = "test";
  fprintf(stderr, "%s: %s not yet implemented\n", EXE_NAME, func);
  return -5;
}

int main(int argc, char* argv[]) {
  int opt;
  struct option longopts[] = {{"help", optional_argument, NULL, 'h'}};
  SubCommand CMDS[] = {
      {"keygen", "generates a new private key",
       "usage: %s keygen [-o | --output <file>]", &keygen},
      {"qrgen", "generates a client registration QR code (jpg)",
       "usage: %s qrgen -k <keyfile> [-o | --output <file>]", &qrgen},
      {"code", "returns the current 6-digit TOTP", "usage: %s code", &code},
      {"test", "'authenticate' with 6-digit TOTP", "usage: %s test <totp>",
       &test}};

  if (argc < 2) {
    fprintf(stderr, "argc < 2\n");
    usage();
  }

  opterr = 0;

  while ((opt = getopt_long(argc, argv, "h::", longopts, NULL)) != -1) {
    switch (opt) {
      case 'h':
        fprintf(stderr, "case h\n");
        usage();
        return EXIT_FAILURE;
        break;
      case '?':
        // Unknown option
        fprintf(stderr, "case ?\n");
        return EXIT_FAILURE;
        break;
      default:
        // All hell breaks loose
        fprintf(stderr, "case default\n");
        return EXIT_FAILURE;
    }
  }

  if (argv[optind] == NULL) {
    fprintf(stderr, "optind condition\n");
    usage();
    return EXIT_FAILURE;
  }

  for (opt = 0; opt < (sizeof(CMDS) / sizeof(SubCommand)); opt++) {
    if (strcmp(argv[optind], CMDS[opt].name) == 0) {
      // Call function associated with this subcommand
      return CMDS[opt].function(argv + optind);
    }
  }

  return EXIT_FAILURE;
}