#ifndef SECURESTORAGE_READER_H
#define SECURESTORAGE_READER_H

#include <sstorage/sstorage.h>
#include <sstorage/entry.h>

#include <ueum/ueum.h>

bool sstorage_has_next(sstorage *storage);

sstorage_entry *sstorage_next(sstorage *storage);

#endif
