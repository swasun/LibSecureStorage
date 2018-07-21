#ifndef SECURESTORAGE_SSTORAGE_H
#define SECURESTORAGE_SSTORAGE_H

#include <uecm/uecm.h>

#include <stdio.h>

typedef enum {
    SSTORAGE_READ,
    SSTORAGE_WRITE
} sstorage_mode;

typedef struct {
    const char *file_name;
    uecm_crypto_metadata *crypto_metadata;
    FILE *fd;
    sstorage_mode mode;
} sstorage;

sstorage *sstorage_open_read(const char *file_name, uecm_crypto_metadata *crypto_metadata);

sstorage *sstorage_open_write(const char *file_name, uecm_crypto_metadata *crypto_metadata);

void sstorage_close(sstorage *storage);

bool ssorage_delete(sstorage *storage);

#endif
