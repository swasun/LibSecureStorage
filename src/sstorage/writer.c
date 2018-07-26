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

#include <sstorage/writer.h>
#include <uecm/uecm.h>
#include <ei/ei.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>

bool sstorage_push_entry(sstorage *storage, sstorage_entry *entry) {
    ueum_byte_stream *stream, *data_entry;
    unsigned char *cipher_data;
    size_t cipher_data_size;
    bool result;

    ei_check_parameter_or_return(storage);
    ei_check_parameter_or_return(entry);

    if (storage->mode != SSTORAGE_WRITE) {
        ei_stacktrace_push_msg("File mode isn't write");
        return false;
    }

    result = false;
    cipher_data = NULL;
    stream = ueum_byte_stream_create();
    data_entry = sstorage_entry_get_data(entry);

    if (!uecm_cipher_plain_data_default(ueum_byte_stream_get_data(data_entry), ueum_byte_stream_get_size(data_entry),
        uecm_crypto_metadata_get_cipher_public_key(storage->crypto_metadata),
        &cipher_data, &cipher_data_size)) {

        ei_stacktrace_push_msg("Failed to cipher plain data of specified entry with our crypto metadata");
        goto clean_up;
    }

    if (!ueum_byte_writer_append_int(stream, sstorage_entry_get_data_type(entry))) {
        ei_stacktrace_push_msg("Failed to add to stream data type");
        goto clean_up;
    }

    if (!ueum_byte_writer_append_int(stream, (int)cipher_data_size)) {
        ei_stacktrace_push_msg("Failed to add to stream cipher data size");
        goto clean_up;
    }

    if (!ueum_byte_writer_append_bytes(stream, cipher_data, cipher_data_size)) {
        ei_stacktrace_push_msg("Failed to add to stream cipher data");
        goto clean_up;
    }

    if (fwrite(ueum_byte_stream_get_data(stream), ueum_byte_stream_get_size(stream), 1, storage->fd) != 1) {
        ei_stacktrace_push_errno();
        goto clean_up;
    }

    result = true;

clean_up:
    ueum_byte_stream_destroy(stream);
    ueum_safe_free(cipher_data);
    return result;
}
