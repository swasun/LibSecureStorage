## Description

C library to store data or multiple files in an encrypted single-file storage.

## API

Example of use of API without error handling for sake of simplicity. For a detailed example, see `examples` folder.

### Create entries

```c
sstorage_entry *first = sstorage_entry_create(<DATA_TYPE_INTEGER>);
sstorage_entry_add_string(first, "Hello world !");

// write first entry
sstorage_entry_destroy(first);

sstorage_entry *second = sstorage_entry_create(<DATA_TYPE_INTEGER>);
sstorage_entry_add_file(second, "data.bin");

// write second entry
sstorage_entry_destroy(second);
```

### Write entries to storage

```c
sstorage *storage = sstorage_open_write("example.sst", <crypto_metadata>);

sstorage_push_entry(storage, first);
sstorage_push_entry(storage, second);

sstorage_close(storage);
```

### Read an entry from storage

```c
sstorage *storage = sstorage_open_read("example.sst", <crypto_metadata>);

if (!sstorage_has_next(storage)) {
    <error_handling>
}

entry = sstorage_next(storage);

sstorage_close(storage);
```

## Dependencies

* [LibErrorInterceptor](https://github.com/swasun/LibErrorInterceptor), a lightweight and cross-plateform library to handle stacktrace and logging in C99. Last version.
* [LibUnknownEchoUtilsModule](https://github.com/swasun/LibUnknownEchoUtilsModule) Utils module of [LibUnknownEcho](https://github.com/swasun/LibUnknownEcho). Last version.
* [LibUnknownEchoCryptoModule](https://github.com/swasun/LibUnknownEchoCryptoModule) Crypto module of [LibUnknownEcho](https://github.com/swasun/LibUnknownEcho). Last version.
* [Libssl](https://github.com/openssl/openssl) Provides the client and server-side implementations for SSLv3 and TLS. Version 1.1
* [Libcrypto](https://github.com/openssl/openssl) Provides general cryptographic and X.509 support needed by SSL/TLS but
    not logically part of it. Version 1.1.
* [Zlib](https://github.com/madler/zlib) A massively spiffy yet delicately unobtrusive compression library. Version 1.2.11.

## Cross-plateform

Tested on Windows 64, but it should also work on Windows x86 and Linux.
