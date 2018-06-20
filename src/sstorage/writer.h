#ifndef SECURESTORAGE_WRITER_H
#define SECURESTORAGE_WRITER_H

#include <sstorage/sstorage.h>
#include <sstorage/entry.h>

#include <ueum/ueum.h>

bool sstorage_push_entry(sstorage *storage, sstorage_entry *entry);

#endif
