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

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "qio.h"
#include "sys.h"
#include "qio_plugin_hdfs.h"
#include "qbuffer.h"
#include "error.h"

#define HDFS_ERROR(ret) { \
  chpl_internal_error("No HDFS Support");\
  ret;\
}


qioerr hdfs_readv(void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_preadv(void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs) HDFS_ERROR(return 0)

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_connect(void** fs_out, const char* pathname, int port) HDFS_ERROR(return 0)

// This doesn't need to become an fptr since we call this from chapel
qioerr hdfs_disconnect(void* fs) HDFS_ERROR(return 0)

qioerr hdfs_open(void** file, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs) HDFS_ERROR(return 0)

void hdfs_do_release(void* fs) HDFS_ERROR(return)

qioerr hdfs_close(void* fl, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_getlength(void* fl, int64_t* len_out, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_fsync(void* fl, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_getcwd(void* file, const char** path_out, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_getpath(void* file, const char** string_out, void* fs) HDFS_ERROR(return 0)

int hdfs_get_fs_type(void* file, void* s) HDFS_ERROR(return 0)

// ----- multilocale ------
void hdfs_create_locale_mapping(char ***char_arr, int num, const char *loc_name) HDFS_ERROR(return)

char** hdfs_alloc_array(int num_locales) HDFS_ERROR(return NULL)

qioerr hdfs_get_owners_for_bytes(qio_file_t* file, hdfs_block_byte_map_t** locs, int* out_num_blocks, char** locale_array, int num_locales, off_t start_byte, off_t len) HDFS_ERROR(return 0)

qioerr hdfs_get_owners(qio_file_t* file, hdfs_block_byte_map_t** loc, int* out_num_blocks, char** arr, int n) HDFS_ERROR(return 0)

hdfs_block_byte_map_t hdfs_index_array(hdfs_block_byte_map_t* locs, int index) HDFS_ERROR(return locs[0])

qioerr hdfs_get_chunk(void* fl, int64_t* len_out, void* fs) HDFS_ERROR(return 0)

qioerr hdfs_locales_for_range(void* file, off_t start_byte, off_t end_byte, const char*** loc_name, int* num_loc_out, void* fs) HDFS_ERROR(return 0)

qio_file_functions_t hdfs_function_struct = {
    &hdfs_writev,
    &hdfs_readv,
    NULL,
    &hdfs_preadv,
    &hdfs_close,
    &hdfs_open,
    &hdfs_seek,
    &hdfs_getlength,
    &hdfs_getpath,
    &hdfs_fsync,
    &hdfs_getcwd,
    &hdfs_get_fs_type,
    &hdfs_get_chunk,
    &hdfs_locales_for_range,
};

const qio_file_functions_ptr_t hdfs_function_struct_ptr = &hdfs_function_struct;
