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

#ifndef SECURESTORAGE_ENTRY_H
#define SECURESTORAGE_ENTRY_H

#include <ueum/ueum.h>

#include <stddef.h>

typedef struct {
    int data_type;
    ueum_byte_stream *data;
} sstorage_entry;

sstorage_entry *sstorage_entry_create(int data_type);

bool sstorage_entry_add_stream(sstorage_entry *entry, ueum_byte_stream *data);

bool sstorage_entry_add_bytes(sstorage_entry *entry, unsigned char *data, size_t data_size);

bool sstorage_entry_add_string(sstorage_entry *entry, const char *string);

bool sstorage_entry_add_file(sstorage_entry *entry, const char *file_name);

void sstorage_entry_destroy(sstorage_entry *entry);

int sstorage_entry_get_data_type(sstorage_entry *entry);

ueum_byte_stream *sstorage_entry_get_data(sstorage_entry *entry);

#endif
