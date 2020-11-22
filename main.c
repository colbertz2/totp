/***********************************************
 * main.c - Main program for TOTP demo
 *
 * SYNOPSIS
 *      totp <subcommand>
 *
 * AUTHOR
 *      Zach Colbert <colbertz@oregonstate.edu>
 ***********************************************/

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "otp.h"
#include "qr.h"

#define EXE_NAME "totp"

int keygen(int, char**);
int qrgen(int, char**);
int code(int, char**);
int test(int, char**);

typedef struct SubCommand {
  char* name;
  char* note;
  int (*function)(int, char**);
} SubCommand;

SubCommand CMDS[] = {
    {"keygen", "generate new private key", &keygen},
    {"qrgen", "generate qr code for otp client registration", &qrgen},
    {"code", "print 6-digit TOTP code for given private key", &code},
    {"test", "authenticate 6-digit TOTP code against given private key",
     &test}};

void usage() {
  int i;

  fprintf(stderr, "usage: %s <command> [options]\n", EXE_NAME);
  fprintf(stderr, "\ncommands:\n");
  for (i = 0; i < (sizeof(CMDS) / sizeof(SubCommand)); i++) {
    fprintf(stderr, "\t%s\t%s\n", CMDS[i].name, CMDS[i].note);
  }
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  int opt;

  if (argc < 2) {
    fprintf(stderr, "argc < 2\n");
    usage();
  }

  if (strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0) {
    fprintf(stderr, "help option\n");
    usage();
  }

  opterr = 0;  // Suppress getopt default error

  for (opt = 0; opt < (sizeof(CMDS) / sizeof(SubCommand)); opt++) {
    if (strcmp(argv[optind], CMDS[opt].name) == 0) {
      // Call function associated with this subcommand
      return CMDS[opt].function(argc, argv);
    }
  }

  // unknown command
  fprintf(stderr, "%s: '%s' is not a command. See '%s --help'\n", EXE_NAME,
          argv[argc - 1], EXE_NAME);
  return EXIT_FAILURE;
}

int keygen(int argc, char** argv) {
  int ret = EXIT_SUCCESS;    // Return value
  int opt;                   // getopt processing
  int bflag = 0, oflag = 0;  // getopt processing
  char* opath;               // output file path
  FILE* ofile;               // output file pointer
  struct option longopts[] = {{"output", required_argument, NULL, 'o'},
                              {"base32", no_argument, NULL, 'b'},
                              {"help", no_argument, NULL, 'h'}};
  char *key, *out;  // key bytes string, output string

  optind = 1;  // reset getopt
  while ((opt = getopt_long(argc, argv, "o:h", longopts, NULL)) != -1) {
    switch (opt) {
      case 'h':
        // Print usage info
        fprintf(stderr,
                "usage: %s keygen [-h | --help] [-o <file> | --output <file>] "
                "[--base32]\n",
                EXE_NAME);
        return 1;
        break;
      case 'b':
        // Encode key as base32 before outputting
        bflag = 1;
        break;
      case 'o':
        // Output to file
        oflag = 1;
        opath = calloc(strlen(optarg) + 1, sizeof(char));
        memset(opath, '\0', strlen(optarg) + 1);
        strcpy(opath, optarg);
        break;
      default:
        // ignore unknown options
        break;
    }
  }

  key = otp_keygen();

  if (bflag) {
    // Encode key as base32 before outputting
    out = b32_encode(key, strlen(key));
  } else {
    out = key;
  }

  if (oflag) {
    // Output to file
    ofile = fopen(opath, "w+");
    if (ofile == NULL) {
      // Unable to open file for writing
      fprintf(stderr, "totp: %s\n", strerror(errno));
      ret = 1;
      goto ABORT;
    }

    fprintf(ofile, "%s\n", out);
    fclose(ofile);
  } else {
    // Output to stdout
    printf("%s\n", out);
  }

ABORT:
  if (bflag) {
    free(out);
  }
  if (oflag) {
    free(opath);
  }

  return ret;
}

int qrgen(int argc, char** argv) {
  int ret = EXIT_SUCCESS;  // return value
  int opt;
  struct option longopts[] = {{"help", no_argument, NULL, 'h'}};

  optind = 1;  // reset getopt
  while ((opt = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
    switch (opt) {
      case 'h':
        // Print usage info
        fprintf(stderr, "usage: %s qrgen [-h | --help]\n", EXE_NAME);
        return 1;
        break;
      default:
        // ignore unknown options
        break;
    }
  }

  fprintf(stderr, "%s: qrgen not yet implemented\n", EXE_NAME);
  ret = -5;

  return ret;
}

int code(int argc, char** argv) {
  int ret = EXIT_SUCCESS;  // return value
  int opt;
  struct option longopts[] = {{"help", no_argument, NULL, 'h'}};

  optind = 1;  // reset getopt
  while ((opt = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
    switch (opt) {
      case 'h':
        // Print usage info
        fprintf(stderr, "usage: %s qrgen [-h | --help]\n", EXE_NAME);
        return 1;
        break;
      default:
        // ignore unknown options
        break;
    }
  }

  fprintf(stderr, "%s: code not yet implemented\n", EXE_NAME);
  ret = -5;

  return ret;
}

int test(int argc, char** argv) {
  int ret = EXIT_SUCCESS;  // return value
  int opt;
  struct option longopts[] = {{"help", no_argument, NULL, 'h'}};

  optind = 1;  // reset getopt
  while ((opt = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
    switch (opt) {
      case 'h':
        // Print usage info
        fprintf(stderr, "usage: %s test [-h | --help]\n", EXE_NAME);
        return 1;
        break;
      default:
        // ignore unknown options
        break;
    }
  }

  fprintf(stderr, "%s: qrgen not yet implemented\n", EXE_NAME);
  ret = -5;

  return ret;
}