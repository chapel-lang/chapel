// This defines the HDFS implementation/interface to the QIO filesystem plugin.
// Documentation can be found in $CHPL_HOME/doc/release/technotes/
#ifndef QIOPLUGIN_HDFS_H_
#define QIOPLUGIN_HDFS_H_ 

#include "sys_basic.h"
#include "qio.h"
#ifdef __cplusplus
extern "C" {
#endif

#define QIO_LOCALE_MAP_PTR_T_NULL NULL
  
// The "fd" for HDFS
typedef struct hdfs_file hdfs_file;

typedef struct hdfs_block_byte_map_t {
  int locale_id; // locale ID
  off_t start_byte; // Start byte for this block
  off_t len; // Length in bytes of this block
} hdfs_block_byte_map_t;

typedef hdfs_block_byte_map_t* qio_locale_map_ptr_t;

qioerr hdfs_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out);

qioerr hdfs_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out);

qioerr hdfs_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out);

qioerr hdfs_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out);

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_connect(void** fs_out, const char* pathname, int port);

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_disconnect(void* fs);

qioerr hdfs_open(void** file, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs);

qioerr hdfs_close(void* fl);

qioerr hdfs_seek(void* fl, off_t offset, int whence, off_t* offset_out);

qioerr hdfs_fsync(void* fl);

qioerr hdfs_getcwd(void* file, const char** path_out);

qioerr hdfs_getpath(void* file, const char** string_out);

qio_file_functions_ptr_t hdfs_create_file_functions(void* fs);

// ----- multilocale ------
typedef char** char_ptr_ptr;

void hdfs_create_locale_mapping(char ***char_arr, int num, char *loc_name);

char** hdfs_alloc_array(int num_locales); 

qioerr hdfs_get_owners_for_bytes(qio_file_t* file, hdfs_block_byte_map_t** locs, int* out_num_blocks, char** locale_array, int num_locales, off_t start_byte, off_t len);

qioerr hdfs_get_owners(qio_file_t* file, hdfs_block_byte_map_t** loc, int* out_num_blocks, char** arr, int n);

hdfs_block_byte_map_t hdfs_index_array(hdfs_block_byte_map_t* locs, int index);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif

