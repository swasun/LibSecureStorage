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
#include <sstorage/writer.h>
#include <sstorage/reader.h>
#include <sstorage/entry.h>
#include <uecm/uecm.h>
#include <ueum/ueum.h>
#include <ei/ei.h>

#include <stdlib.h>
#include <stdio.h>

#define STORAGE_FILE_NAME "storage_example.sst"
#define DATA_TYPE_EXAMPLE 12
#define DATA_EXAMPLE "Hello world !"

static bool write_storage(const char *file_name, uecm_crypto_metadata *crypto_metadata) {
    bool result;
    sstorage *storage;
    sstorage_entry *entry;

    result = false;
    storage = NULL;
    entry = NULL;

    if ((storage = sstorage_open_write(file_name, crypto_metadata)) == NULL) {
        ei_stacktrace_push_msg("Failed open new storage in write mode");
        return false;
    }

    if ((entry = sstorage_entry_create(DATA_TYPE_EXAMPLE)) == NULL) {
        ei_stacktrace_push_msg("Failed to create new entry with DATA_TYPE_EXAMPLE");
        goto clean_up;
    }

    if (!sstorage_entry_add_string(entry, DATA_EXAMPLE)) {
        ei_stacktrace_push_msg("Failed to add new string DATA_EXAMPLE to entry");
        goto clean_up;
    }

    if (!sstorage_push_entry(storage, entry)) {
        ei_stacktrace_push_msg("Failed to push new entry to storage");
        goto clean_up;
    }

    result = true;

clean_up:
    if (entry) {
        sstorage_entry_destroy(entry);
    }
    if (storage) {
        sstorage_close(storage);
    }
    return result;
}

static bool read_storage(const char *file_name, uecm_crypto_metadata *crypto_metadata) {
    bool result;
    sstorage *storage;
    sstorage_entry *entry;

    result = false;
    storage = NULL;
    entry = NULL;

    if ((storage = sstorage_open_read(file_name, crypto_metadata)) == NULL) {
        ei_stacktrace_push_msg("Failed to open new storage in read mode");
        return false;
    }

    if (!sstorage_has_next(storage)) {
        ei_stacktrace_push_msg("has_next() returned false, but it shouldn't");
        goto clean_up;
    }

    if ((entry = sstorage_next(storage)) == NULL) {
        ei_stacktrace_push_msg("Failed to read next entry");
        goto clean_up;
    }

    /* Dump the plain data of the entry */
    ueum_byte_stream_print_string(sstorage_entry_get_data(entry), stdout);

    result = true;

clean_up:
    if (entry) {
        sstorage_entry_destroy(entry);
    }
    if (storage) {
        sstorage_close(storage);
    }
    return result;
}

int main() {
    uecm_crypto_metadata *crypto_metadata;

    if (!ei_init()) {
        fprintf(stderr, "[ERROR] Failed to init LibErrorInterceptor");
        exit(EXIT_FAILURE);
    }

    if (!uecm_init()) {
        fprintf(stderr, "[ERROR] Failed to init LibUnknownEchoCryptoModule");
        exit(EXIT_FAILURE);
    }

    crypto_metadata = NULL;

    ei_logger_info("Generating crypto metadata...");
    if ((crypto_metadata = uecm_crypto_metadata_create_default()) == NULL) {
        ei_stacktrace_push_msg("Failed to create random crypto metadata");
        goto clean_up;
    }
    ei_logger_info("Crypto metadata generated.");

    ei_logger_info("Writing new storage...");
    if (!write_storage(STORAGE_FILE_NAME, crypto_metadata)) {
        ei_stacktrace_push_msg("Failed to write example storage");
        goto clean_up;
    }
    ei_logger_info("New storage wrote.");

    ei_logger_info("Reading new storage...");
    if (!read_storage(STORAGE_FILE_NAME, crypto_metadata)) {
        ei_stacktrace_push_msg("Failed to read example storage");
        goto clean_up;
    }
    ei_logger_info("New storage read.");

    ei_logger_info("Done.");

clean_up:
    if (ei_stacktrace_is_filled()) {
        ei_logger_stacktrace("Stacktrace is filled with following error(s):");
        ei_stacktrace_print();
    }
    uecm_crypto_metadata_destroy(crypto_metadata);
    uecm_uninit();
    ei_uninit();
    return EXIT_SUCCESS;
}
