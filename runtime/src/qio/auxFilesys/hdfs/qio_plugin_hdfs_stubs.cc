#define QIOPLUGIN_HDFS_C

#ifndef _DARWIN_C_SOURCE
// get fcntl(F_GETPATH)
#define _DARWIN_C_SOURCE
#endif

#ifndef _GNU_SOURCE
// get O_DIRECT, fopencookie
#define _GNU_SOURCE
#endif

#include "sys_basic.h"
#include <string.h>

#ifndef SIMPLE_TEST
#include "chplrt.h"
#endif

#include "qio.h"
#include "sys.h"
#include "qio_plugin_hdfs.h"
#include "qbuffer.h"
#include "error.h"
#undef exit

#define HDFS_ERROR { \
  chpl_internal_error("No HDFS Support");\
  exit(EXIT_FAILURE);\
}


qioerr hdfs_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs) HDFS_ERROR

qioerr hdfs_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out, void* fs) HDFS_ERROR

qioerr hdfs_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs) HDFS_ERROR

qioerr hdfs_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs) HDFS_ERROR

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_connect(void** fs_out, const char* pathname, int port) HDFS_ERROR

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_disconnect(void* fs) HDFS_ERROR

qioerr hdfs_open(void** file, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs) HDFS_ERROR

qioerr hdfs_close(void* fl, void* fs) HDFS_ERROR

qioerr hdfs_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs) HDFS_ERROR

qioerr hdfs_fsync(void* fl, void* fs) HDFS_ERROR

qioerr hdfs_getcwd(void* file, const char** path_out, void* fs) HDFS_ERROR

qioerr hdfs_getpath(void* file, const char** string_out, void* fs) HDFS_ERROR

// ----- multilocale ------
void hdfs_create_locale_mapping(char ***char_arr, int num, const char *loc_name) HDFS_ERROR

char** hdfs_alloc_array(int num_locales) HDFS_ERROR

qioerr hdfs_get_owners_for_bytes(qio_file_t* file, hdfs_block_byte_map_t** locs, int* out_num_blocks, char** locale_array, int num_locales, off_t start_byte, off_t len) HDFS_ERROR

qioerr hdfs_get_owners(qio_file_t* file, hdfs_block_byte_map_t** loc, int* out_num_blocks, char** arr, int n) HDFS_ERROR

hdfs_block_byte_map_t hdfs_index_array(hdfs_block_byte_map_t* locs, int index) HDFS_ERROR

qio_file_functions_t hdfs_function_struct = {
    &hdfs_writev,
    &hdfs_readv,
    NULL,
    &hdfs_preadv,
    &hdfs_close,
    &hdfs_open,
    &hdfs_seek,
    NULL,
    &hdfs_getpath,
    &hdfs_fsync,
    &hdfs_getcwd,
};

const qio_file_functions_ptr_t hdfs_function_struct_ptr = &hdfs_function_struct;
