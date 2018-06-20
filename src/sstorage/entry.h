#ifndef SECURESTORAGE_ENTRY_H
#define SECURESTORAGE_ENTRY_H

#include <ueum/ueum.h>

typedef struct {
	int id;
	ueum_byte_stream *stream;
} sstorage_entry;

sstorage_entry *sstorage_entry_create_from_stream(int id, ueum_byte_stream *stream);

void sstorage_entry_destroy(sstorage_entry *entry);

ueum_byte_stream *sstorage_entry_get_stream(sstorage_entry *entry);

#endif
