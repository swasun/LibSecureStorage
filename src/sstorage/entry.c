#include <sstorage/entry.h>

#include <ei/ei.h>

sstorage_entry *sstorage_entry_create_from_stream(ueum_byte_stream *stream) {
	sstorage_entry *entry;

	ei_check_parameter_or_return(stream);

	ueum_safe_alloc(entry, sstorage_entry, 1);
	entry->stream = ueum_byte_stream_copy(stream);

	return entry;
}

void sstorage_entry_destroy(sstorage_entry *entry) {
	if (entry) {
		ueum_byte_stream_destroy(stream);
		ueum_safe_free(entry);
	}
}

ueum_byte_stream *sstorage_entry_get_stream(sstorage_entry *entry) {
	ei_check_parameter_or_return(entry);
	ei_check_parameter_or_return(entry->stream);

	return entry->stream;
}
