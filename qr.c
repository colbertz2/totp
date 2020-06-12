/*******************************************************
 * qr.c - Shared secret and qr code generation methods
 * 
 * AUTHOR
 *      Zach Colbert <colbertz@oregonstate.edu>
 *******************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "qr.h"
#include "qrgen/qrcodegen.h"       // Thanks to @nayuki for the library!

#define KEY_LENGTH 32

// "Private" methods
char* _qr_keygen();
int _qr_img(const char* uri);

// Generate key for TOTP, compile info for GA URI,
// and encode this URI in a QR code
int qr_generate() {
    char *key, *uname, *keyfname = "secret";
    char *hostname = calloc(101, sizeof(char));
    char *uri = calloc(275, sizeof(char));
    int status, keyfd;
    struct passwd *pw;
    
    // Generate key and write to file
    /************************************************
     * NOTE: This app overwrites the key file each
     * time a new one is generated. We only support
     * one user at a time.
     ************************************************/
    key = _qr_keygen();
    do {
        keyfd = open(keyfname, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    } while (keyfd == -1);
    write(keyfd, key, strlen(key) * sizeof(char));
    close(keyfd);
    // fprintf(stderr, "DEBUG: secret %s\n", key); // @DEV

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
    sprintf(uri, "otpauth://totp/CS370:%s@%s?secret=%s&issuer=CS370",
            uname, hostname, key);
    // fprintf(stderr, "DEBUG: uri %s\n", uri);    // @DEV

    // Generate QR code
    status = _qr_img(uri);

    // Free buffers
    free(key);
    free(hostname);
    free(uri);

    return 0;
}

// Generate key for TOTP, convert to base32, and save to file
char* _qr_keygen() {
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
        fprintf(stderr, "%s\n", 
            ERR_error_string( ERR_get_error(), 0 ));
    }

    // Convert each byte to a base32 char reperesentation
    for (i = 0; i < KEY_LENGTH; i++) {
        b32[i] = ((int) buffer[i] % 32);
        
        // Offset value to appropriate ASCII char set
        if ((int) b32[i] <= 5) { b32[i] += 50; }
        else { b32[i] += 59; }
    }

    free(buffer);
    return b32;
}

// Generate QR code for given uri and print to console
// Based on demo code from @nayuki
int _qr_img(const char* uri) {
    int x, y, size, border;
    uint8_t qr0[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(uri, tempBuffer,
        qr0, qrcodegen_Ecc_LOW, qrcodegen_VERSION_MIN,
        qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (!ok) {
        fprintf(stderr, "Problem encoding uri to bytes\n");
        return 1;
    }

    size = qrcodegen_getSize(qr0);
	border = 2;
	for (y = -border; y < size + border; y++) {
		for (x = -border; x < size + border; x++) {
			fputs((qrcodegen_getModule(qr0, x, y) ? "##" : "  "), stdout);
		}
		fputs("\n", stdout);
	}
	fputs("\n", stdout);

    return 0;
}
