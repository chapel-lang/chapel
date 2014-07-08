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

qioerr hdfs_connect(void** fs_out, const char* pathname, int port) 
{
  chpl_internal_error("No HDFS Support");
  return qio_int_to_err(ENOSYS);
}

qioerr hdfs_disconnect(void* fs) 
{
  chpl_internal_error("No HDFS Support");
  return qio_int_to_err(ENOSYS);
}

char **hdfs_alloc_array(int num_locales) 
{
  chpl_internal_error("No HDFS Support");
  return NULL;
}

void hdfs_create_locale_mapping(char ***char_arr, int num, char *loc_name) 
{
  chpl_internal_error("No HDFS Support");
}

hdfs_block_byte_map_t hdfs_index_array(hdfs_block_byte_map_t* locs, int index) 
{
  chpl_internal_error("No HDFS Support");
  return locs[0];
}

qio_file_functions_ptr_t hdfs_create_file_functions(void* fs) 
{
  chpl_internal_error("No HDFS Support");
  return NULL;
}

qioerr hdfs_get_owners(qio_file_t* file, hdfs_block_byte_map_t** locs, int* out_num_blocks, char** locale_array, int num_locales)
{
  chpl_internal_error("No HDFS Support");
  return qio_int_to_err(ENOSYS);
}
