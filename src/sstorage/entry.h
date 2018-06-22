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
