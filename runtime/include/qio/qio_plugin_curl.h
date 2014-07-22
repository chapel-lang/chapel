#ifndef QIOPLUGIN_CURL_H_
#define QIOPLUGIN_CURL_H_

#include "sys_basic.h"
#include "qio.h"
#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif

#define QIO_LOCALE_MAP_PTR_T_NULL NULL
typedef struct curl_handle curl_handle;
typedef curl_handle* curl_handle_ptr;

extern qio_file_functions_t glob_struct;
extern const qio_file_functions_ptr_t ptr_glob_struct;

qioerr curl_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs);
qioerr curl_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs);
qioerr curl_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out, void* fs);
qioerr curl_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs);
qioerr curl_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs);
qioerr curl_close(void* fl, void* fs);
qioerr curl_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs);
qioerr curl_getpath(void* file, const char** string_out, void* fs);
qioerr curl_getlength(void* fl, int64_t* len_out, void* fs);

qioerr chpl_curl_perform(qio_file_t* fl);
qioerr chpl_curl_stream_file(qio_file_t* fl_curl, qio_file_t* fl_local);
qioerr chpl_curl_stream_string(qio_file_t* fl, const char** str);
qioerr chpl_curl_set_opt(qio_file_t* fl, int opt, ...);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif



