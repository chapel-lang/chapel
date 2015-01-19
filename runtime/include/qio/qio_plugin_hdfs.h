/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
  
// the struct that holds all the functions for our FS
extern qio_file_functions_t hdfs_function_struct;
extern const qio_file_functions_ptr_t hdfs_function_struct_ptr;

// The "fd" for HDFS
typedef struct hdfs_file hdfs_file;
// The "fs" for HDFS
typedef struct hdfs_fs hdfs_fs;

typedef struct hdfs_block_byte_map_t {
  int locale_id; // locale ID
  off_t start_byte; // Start byte for this block
  off_t len; // Length in bytes of this block
} hdfs_block_byte_map_t;

typedef hdfs_block_byte_map_t* qio_locale_map_ptr_t;

qioerr hdfs_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs);

qioerr hdfs_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out, void* fs);

qioerr hdfs_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs);

qioerr hdfs_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs);

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_connect(void** fs_out, const char* pathname, int port);

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_disconnect(void* fs);

qioerr hdfs_open(void** file, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs);

qioerr hdfs_close(void* fl, void* fs);

void hdfs_do_release(void* fs);

qioerr hdfs_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs);

qioerr hdfs_getlength(void* fl, int64_t* len_out, void* fs);

qioerr hdfs_fsync(void* fl, void* fs);

int hdfs_get_fs_type(void* fl, void* fs);

qioerr hdfs_getcwd(void* file, const char** path_out, void* fs);

qioerr hdfs_getpath(void* file, const char** string_out, void* fs);

qio_file_functions_ptr_t hdfs_create_file_functions(void* fs);

// ----- multilocale ------
typedef char** char_ptr_ptr;

void hdfs_create_locale_mapping(char ***char_arr, int num, const char *loc_name);

char** hdfs_alloc_array(int num_locales); 

qioerr hdfs_get_owners_for_bytes(qio_file_t* file, hdfs_block_byte_map_t** locs, int* out_num_blocks, char** locale_array, int num_locales, off_t start_byte, off_t len);

qioerr hdfs_get_owners(qio_file_t* file, hdfs_block_byte_map_t** loc, int* out_num_blocks, char** arr, int n);

hdfs_block_byte_map_t hdfs_index_array(hdfs_block_byte_map_t* locs, int index);

qioerr hdfs_get_chunk(void* fl, int64_t* len_out, void* fs);

qioerr hdfs_locales_for_range(void* file, off_t start_byte, off_t end_byte, const char*** loc_names_out, int* num_loc_out, void* fs);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif

