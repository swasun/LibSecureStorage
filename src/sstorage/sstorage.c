/*******************************************************************************
 * Copyright (C) 2018 Charly Lamothe                                           *
 *                                                                             *
 * This file is part of LibSecureStorage.                                      *
 *                                                                             *
 *   Licensed under the Apache License, Version 2.0 (the "License");           *
 *   you may not use this file except in compliance with the License.          *
 *   You may obtain a copy of the License at                                   *
 *                                                                             *
 *   http://www.apache.org/licenses/LICENSE-2.0                                *
 *                                                                             *
 *   Unless required by applicable law or agreed to in writing, software       *
 *   distributed under the License is distributed on an "AS IS" BASIS,         *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *   See the License for the specific language governing permissions and       *
 *   limitations under the License.                                            *
 *******************************************************************************/

#include <sstorage/sstorage.h>

#include <ueum/ueum.h>

#include <ei/ei.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>

sstorage *sstorage_open_read(const char *file_name, uecm_crypto_metadata *crypto_metadata) {
    sstorage *storage;
    FILE *fd;

    ei_check_parameter_or_return(file_name);
    ei_check_parameter_or_return(crypto_metadata);

    if ((fd = fopen(file_name, "rb")) == NULL) {
        ei_stacktrace_push_msg("Failed to open specified file with rb mode");
        return NULL;
    }

    storage = NULL;

    ueum_safe_alloc(storage, sstorage, 1);
    storage->file_name = file_name;
    storage->mode = SSTORAGE_READ;
    storage->fd = fd;
    storage->crypto_metadata = crypto_metadata;

    return storage;
}

sstorage *sstorage_open_write(const char *file_name, uecm_crypto_metadata *crypto_metadata) {
    sstorage *storage;
    FILE *fd;

    ei_check_parameter_or_return(file_name);
    ei_check_parameter_or_return(crypto_metadata);

    if ((fd = fopen(file_name, "wb+")) == NULL) {
        ei_stacktrace_push_msg("Failed to open specified file with wb+ mode");
        return NULL;
    }

    storage = NULL;

    ueum_safe_alloc(storage, sstorage, 1);
    storage->file_name = file_name;
    storage->mode = SSTORAGE_WRITE;
    storage->fd = fd;
    storage->crypto_metadata = crypto_metadata;

    return storage;
}

void sstorage_close(sstorage *storage) {
    if (!storage) {
        ei_logger_warn("Specified storage ptr is null");
        return;
    }

    if (!storage->fd) {
        ei_logger_warn("Specified storage object have null fd");
        ueum_safe_free(storage);
        return;
    }

    if (fclose(storage->fd) != 0) {
        ei_logger_warn("Failed to close storage file descriptor with error message: '%s'", strerror(errno));
    }

    ueum_safe_free(storage);
}

bool ssorage_delete(sstorage *storage) {
    ei_check_parameter_or_return(storage);
    ei_check_parameter_or_return(storage->file_name);

    if (remove(storage->file_name) != 0) {
        ei_stacktrace_push_errno();
        return false;
    }

    return true;
}
