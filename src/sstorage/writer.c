#include <sstorage/writer.h>
#include <uecm/uecm.h>
#include <ei/ei.h>

#include <stddef.h>
#include <string.h>

bool sstorage_push_entry(sstorage *storage, sstorage_entry *entry) {
	ueum_byte_stream *stream;
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
	stream = sstorage_entry_get_stream(entry);

	if (!uecm_cipher_plain_data_default(ueum_byte_stream_get_data(stream), ueum_byte_stream_get_size(stream),
		uecm_crypto_metadata_get_cipher_public_key(storage->crypto_metadata),
		&cipher_data, &cipher_data_size)) {

		ei_stacktrace_push_msg("Failed to cipher plain data of specified entry with our crypto metadata");
		return false;
	}

	if (fwrite(cipher_data, cipher_data_size, 1, storage->fd) != 1) {
		ei_stacktrace_push_errno();
		goto clean_up;
	}

	result = true;

clean_up:
	ueum_safe_free(cipher_data);
	return result;
}
