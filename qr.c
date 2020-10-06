/*******************************************************
 * qr.c - Shared secret and qr code generation methods
 *
 * AUTHOR
 *      Zach Colbert <colbertz@oregonstate.edu>
 *******************************************************/

#include "qr.h"

#include <errno.h>
#include <fcntl.h>
#include <gd.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <pwd.h>
#include <qrencode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define KEY_LENGTH 32

#define KEY_REUSE 0  // 0 --> reuse key FOR DEVELOPMENT ONLY

// "Private" methods
char *_qr_keygen();
gdImagePtr _qr_img(QRcode *, int[3], int[3], int, int);

// Generate key for TOTP, compile info for GA URI,
// and encode this URI in a QR code
int qr_generate() {
  char *key, *uname, *keyfname = "secret";
  char *hostname = calloc(101, sizeof(char));
  char *uri = calloc(275, sizeof(char));
  int status;
  FILE *keyfile;
  struct passwd *pw;

  int QRversion = 0;                 // Allow library to choose code size
  QRecLevel QRlevel = QR_ECLEVEL_M;  // Error correction level
  QRencodeMode QRmode = QR_MODE_8;   // UTF-8 mode
  int QRcase = 1;                    // Case-sensitive data
  QRcode *qrData;

  int int_bg_color[3] = {255, 255, 255};  // QR Code image background color
  int int_fg_color[3] = {0, 0, 0};        // QR Code image foreground color
  int img_size = 1000;                    // Image size
  int img_margin = 25;                    // Image margin
  int img_quality = 100;                  // JPEG quality (for GD)
  char *imgfname = "scanme.jpg";          // Filename for image output
  FILE *imgfile;                          // File descriptor for image out
  gdImagePtr qrImg;

  // Generate key and write to file
  /************************************************
   * NOTE: This app overwrites the key file each
   * time a new one is generated. We only support
   * one user at a time.
   ************************************************/
  if ((access(keyfname, F_OK) == -1) || (KEY_REUSE != 0)) {
    // Generate new key
    key = _qr_keygen();

    // Write key to file
    keyfile = fopen(keyfname, "w");
    if (keyfile == NULL) {
      fprintf(stderr, "Unable to open secret key file: %s\n", keyfname);
      return 1;
    }
    fputs(key, keyfile);
    fclose(keyfile);
  } else {
    // Reuse existing key
    keyfile = fopen(keyfname, "r");
    if (keyfile == NULL) {
      fprintf(stderr, "Unable to open secret key file: %s\n", keyfname);
      return 1;
    }

    key = calloc(KEY_LENGTH, sizeof(char));
    if (fgets(key, KEY_LENGTH, keyfile) == NULL) {
      fprintf(stderr, "Unable to read secret key from file: %s\n", keyfname);
      fclose(keyfile);
      return 1;
    }

    fclose(keyfile);
  }
  fprintf(stderr, "DEBUG: secret %s\n", key);  // @DEV

  // Get user id
  pw = getpwuid(getuid());
  uname = pw->pw_name;

  // Get hostname
  memset(hostname, '\0', 101 * sizeof(char));
  status = gethostname(hostname, 100);
  if (status == -1) {
    memset(hostname, '\0', 101 * sizeof(char));
    sprintf(hostname, "localhost");
  }

  // Compile uri
  memset(uri, '\0', 275 * sizeof(char));
  sprintf(uri, "otpauth://totp/CS370:%s@%s?secret=%s&issuer=CS370", uname,
          hostname, key);
  // fprintf(stderr, "DEBUG: uri %s\n", uri);    // @DEV

  // Encode uri into QR Code
  qrData = QRcode_encodeString(uri, QRversion, QRlevel, QRmode, QRcase);
  if (qrData == NULL) {
    fprintf(stderr, "Error encoding uri to bytes: %s\n", strerror(errno));
    return 1;
  }

  // Open file for QR image
  imgfile = fopen(imgfname, "w");
  if (imgfile == NULL) {
    fprintf(stderr, "Unable to open output file: %s\n", imgfname);
    return 1;
  }

  // Encode qr data as gd image
  qrImg = _qr_img(qrData, int_fg_color, int_bg_color, img_size, img_margin);

  // Write to file and close
  gdImageJpeg(qrImg, imgfile, img_quality);

  // Free buffers
  free(key);
  free(hostname);
  free(uri);
  QRcode_free(qrData);
  gdImageDestroy(qrImg);
  fclose(imgfile);

  printf("TOTP barcode saved to: %s\n", imgfname);

  return 0;
}

// Generate key for TOTP, convert to base32, and save to file
char *_qr_keygen() {
  int i, status = 0;
  unsigned char *buffer = calloc(KEY_LENGTH + 1, sizeof(char));
  char *b32 = calloc(KEY_LENGTH + 1, sizeof(char));

  // Initialize key buffer
  memset(buffer, '\0', sizeof(char) * (KEY_LENGTH + 1));
  memset(b32, '\0', sizeof(char) * (KEY_LENGTH + 1));

  // Generate random bytes for key
  status = RAND_pseudo_bytes(buffer, KEY_LENGTH);

  // Warn users if bytes aren't sufficiently random
  if (status != 1) {
    fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), 0));
  }

  // Convert each byte to a base32 char reperesentation
  for (i = 0; i < KEY_LENGTH; i++) {
    b32[i] = ((int)buffer[i] % 32);

    // Offset value to appropriate ASCII char set
    if ((int)b32[i] <= 5) {
      b32[i] += 50;
    } else {
      b32[i] += 59;
    }
  }

  free(buffer);
  return b32;
}

// Based on
// https://github.com/dengshiyong/a-example-about-libqd-and-libqrencode/blob/master/qrencode.c
gdImagePtr _qr_img(QRcode *code, int fgColor[3], int bgColor[3], int size,
                   int margin) {
  int code_size = size / code->width;
  if (code_size == 0) {
    code_size = 1;
  }
  int img_width = code->width * code_size + 2 * margin;
  gdImagePtr img = gdImageCreate(img_width, img_width);
  int img_fgcolor =
      gdImageColorAllocate(img, fgColor[0], fgColor[1], fgColor[2]);
  int img_bgcolor =
      gdImageColorAllocate(img, bgColor[0], bgColor[1], bgColor[2]);
  gdImageFill(img, 0, 0, img_bgcolor);
  u_char *p = code->data;
  int x, y, posx, posy;

  for (y = 0; y < code->width; y++) {
    for (x = 0; x < code->width; x++) {
      if (*p & 1) {
        posx = x * code_size + margin;
        posy = y * code_size + margin;
        gdImageFilledRectangle(img, posx, posy, posx + code_size,
                               posy + code_size, img_fgcolor);
      }
      p++;
    }
  }

  return img;
}