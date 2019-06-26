// TODO: header ifdef guard
// TODO: these should have "plugin" in the name

syserr chpl_qio_setup_plugin_channel(void* file, void** plugin_ch, int64_t start, int64_t end, qio_channel_t* qio_ch);
syserr chpl_qio_channel_close(void* ch);

// Reads amt bytes (or more) into the channel buffer
syserr chpl_qio_read_atleast(void* plugin_ch, int64_t amt);
// Writes amt bytes from the channel buffer
syserr chpl_qio_write(void* plugin_ch, int64_t amt);

//syserr chpl_qio_write...(void* plugin_ch, qio_channel_t* qio_ch);

// These are not necessary.
/*syserr chpl_qio_writev(void* file, struct iovec* iov, int iovcnt, ssize_t* amtWritten);
syserr chpl_qio_readv(void* file, struct iovec* iov, int iovcnt, ssize_t* amtRead);

syserr chpl_qio_pwritev(void* file, struct iovec* iov, int iovcnt, int64_t offset, ssize_t* amtWritten);
syserr chpl_qio_preadv(void* file, struct iovec* iov, int iovcnt, int64_t offset, ssize_t* amtRead);

syserr chpl_qio_seek(void* file, int64_t amount, int whence, int64_t* offset);
*/

syserr chpl_qio_filelength(void* file, int64_t* length);
syserr chpl_qio_getpath(void* file, const char** str, ssize_t* len);
syserr chpl_qio_fsync(void* file);
syserr chpl_qio_get_chunk(void* file, int64_t* length);
syserr chpl_qio_get_locales_for_region(void* file, int64_t start, int64_t end,
    const char*** localeNames, int64_t* nLocales);
syserr chpl_qio_file_close(void* file);

syserr chpl_qio_open(void* filesystem, const char* path, ssize_t pathlen, int* flags, int64_t mode, int64_t hints, void** file);
syserr chpl_qio_get_cwd(void* filesystem, const char** path, ssize_t* len);
syserr chpl_qio_get_fs_type(void* filesystem, int64_t* fsType);
