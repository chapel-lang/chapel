#define QIOPLUGIN_HDFS_C

#ifndef _DARWIN_C_SOURCE
// get fcntl(F_GETPATH)
#define _DARWIN_C_SOURCE
#endif

#ifndef _GNU_SOURCE
// get O_DIRECT, fopencookie
#define _GNU_SOURCE
#endif

#include <string.h>

#ifndef SIMPLE_TEST
#include "chplrt.h"
#endif

#include "qio_plugin_curl.h"

#define CURL_ERROR(ret){\
  chpl_internal_error("No Curl Support");\
  return ret;\
}

qio_file_functions_t curl_function_struct = {
    &curl_writev,
    &curl_readv,
    NULL,
    &curl_preadv,
    &curl_close,
    &curl_open,
    &curl_seek,
    &curl_getlength,
    &curl_getpath,
    NULL,
    NULL,
};

const qio_file_functions_ptr_t curl_function_struct_ptr = &curl_function_struct;

qioerr curl_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs) CURL_ERROR(0)

qioerr curl_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs) CURL_ERROR(0)

qioerr curl_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out, void* fs) CURL_ERROR(0)

qioerr curl_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs) CURL_ERROR(0)

qioerr curl_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs) CURL_ERROR(0)

qioerr curl_close(void* fl, void* fs) CURL_ERROR(0)

qioerr curl_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs) CURL_ERROR(0)

qioerr curl_getpath(void* file, const char** string_out, void* fs) CURL_ERROR(0)

qioerr curl_getlength(void* fl, int64_t* len_out, void* fs) CURL_ERROR(0)

qioerr chpl_curl_set_opt(qio_file_t* fl, int opt, ...) CURL_ERROR(0)

qioerr chpl_curl_perform(qio_file_t* fl) CURL_ERROR(0)

qioerr chpl_curl_stream_string(qio_file_t* fl, const char** str) CURL_ERROR(0)

qioerr chpl_curl_stream_file(qio_file_t* fl_curl, qio_file_t* fl_local) CURL_ERROR(0)
