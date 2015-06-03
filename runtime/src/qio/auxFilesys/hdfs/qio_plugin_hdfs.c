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

// Documentation can be found in $CHPL_HOME/doc/release/technotes/IO.hdfs
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

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include <hdfs.h>
#include "qio_plugin_hdfs.h"

//// So we can deal with the void*'s passed in without messy casting all over the place
#define to_hdfs_file(f) ((hdfs_file*)f)
#define to_hdfs_fs(f) ((hdfs_fs*)f)

#define CREATE_ERROR(test, var, errno, string, goto_name) { \
  if(test) { \
    QIO_GET_CONSTANT_ERROR(var, errno, string); \
    goto goto_name; \
  } \
}

// Darwin does not define EREMOTEIO
#if !defined(EREMOTEIO)
#define EREMOTEIO 121
#endif

// The "fd" for HDFS
struct hdfs_file {
  hdfsFile file;      // file info
  const char* pathnm; // The path to our file. Need this for locality
};

// our FS
struct hdfs_fs {
  qbytes_refcnt_t ref_cnt; // ref count on the FS handle
  hdfsFS          hfs;     // the actual HDFS handle
  const char*     fs_name; // name (for later on)
  int             fs_port; // port
};

qioerr hdfs_readv (void* file, const struct iovec *vector, int count, ssize_t* num_read_out, void* fs)
{
  ssize_t got;
  ssize_t got_total;
  qioerr err_out = 0;
  int i;

  STARTING_SLOW_SYSCALL;

  got_total = 0;
  for( i = 0; i < count; i++) {
    got = hdfsRead(to_hdfs_fs(fs)->hfs, to_hdfs_file(file)->file, (void*)vector[i].iov_base, vector[i].iov_len);
    if( got != -1 ) {
      got_total += got;
    } else {
      err_out = qio_mkerror_errno();
      break;
    }
    if( got != sys_iov_total_bytes(&vector[i], i)) {
      break;
    }
  }

  if( err_out == 0 && got_total == 0 && sys_iov_total_bytes(vector, count) != 0 )
    err_out = qio_int_to_err(EEOF);

  *num_read_out = got_total;

  DONE_SLOW_SYSCALL;

  return err_out;
}

qioerr hdfs_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out, void* fs)
{
  qioerr t = 0;
  QIO_GET_CONSTANT_ERROR(t, ENOSYS, "pwrites in HDFS are not supported");
  fprintf(stderr, "positional writes in HDFS are not supported!!\n");
  return t;
}

qioerr hdfs_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs)
{
  ssize_t got;
  ssize_t got_total = 0;
  qioerr err_out = 0;
  int i;

  STARTING_SLOW_SYSCALL;
  for (i = 0; i < iovcnt; i++) {
    got = hdfsWrite(to_hdfs_fs(fs)->hfs, to_hdfs_file(fl)->file, (void*)(iov[i].iov_base), iov[i].iov_len);

    if (got != -1)
      got_total += got;
    else {
      err_out = qio_mkerror_errno();
      break;
    }

    if (got != sys_iov_total_bytes(&iov[i], iov[i].iov_len))
      break;
  }

  *num_written_out = got_total;
  DONE_SLOW_SYSCALL;

  return err_out;
}

qioerr hdfs_preadv (void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out, void* fs)
{
  ssize_t got;
  ssize_t got_total;
  qioerr err_out = 0;
  int i;

  STARTING_SLOW_SYSCALL;

  err_out = 0;
  got_total = 0;
  for(i = 0; i < count; i++) {
    got = hdfsPread(to_hdfs_fs(fs)->hfs, to_hdfs_file(file)->file, offset + got_total, (void*)vector[i].iov_base, vector[i].iov_len);
    if( got != -1 ) {
      got_total += got;
    } else {
      err_out = qio_mkerror_errno();
      break;
    }
    if(got != (ssize_t)vector[i].iov_len ) {
      break;
    }
  }

  if( err_out == 0 && got_total == 0 && sys_iov_total_bytes(vector, count) != 0 )
    err_out = qio_int_to_err(EEOF);

  *num_read_out = got_total;

  DONE_SLOW_SYSCALL;

  return err_out;
}


static
qioerr hdfs_disconnect_and_free(void* fs)
{
  qioerr err = 0;
  int ret= 0;

  STARTING_SLOW_SYSCALL;
  errno = 0;
  ret = hdfsDisconnect(((hdfs_fs*)fs)->hfs);

  if ((ret == -2) || (ret == -1))  {
    err = qio_mkerror_errno();
  } else errno = 0;
  DONE_SLOW_SYSCALL;

  qio_free(fs);
  return err;
}

void hdfs_do_release(void* fs)
{
  DO_RELEASE(to_hdfs_fs(fs), hdfs_disconnect_and_free);
}

qioerr hdfs_close(void* fl, void* fs)
{
  int got = 0;
  qioerr err_out = 0;

  STARTING_SLOW_SYSCALL;
  got = hdfsCloseFile(to_hdfs_fs(fs)->hfs, to_hdfs_file(fl)->file);

  if(got == -1)
    err_out = qio_mkerror_errno();
  DONE_SLOW_SYSCALL;
  DO_RELEASE((to_hdfs_fs(fs)), hdfs_disconnect_and_free);
  return err_out;
}

qioerr hdfs_connect(void** fs_out, const char* pathname, int port)
{
  qioerr err_out = 0;
  hdfsFS fs;
  hdfs_fs* ret = (hdfs_fs*)qio_calloc(sizeof(hdfs_fs), 1);

  STARTING_SLOW_SYSCALL;
  fs = hdfsConnect(pathname, port);

  CREATE_ERROR((fs == NULL), err_out, ECONNREFUSED,"Unable to connect HDFS", error);
  DONE_SLOW_SYSCALL;

  ret->hfs = fs;

  DO_INIT_REFCNT(ret);
  ret->fs_name = pathname;
  ret->fs_port = port;
  *fs_out = ret;

error:
  return err_out;
}

qioerr hdfs_disconnect(void* fs) {
  // TAKZ - Note that this "lazily" disconnects from our system. Since, if the user
  // does not close all files on this FS handle before calling disconnect, we need to
  // wait until that file is closed (either by them, or by the MM runtime). In this
  // case (where we have an open file on this handle when disconnect is called) we
  // simply decrement the reference count on the handle so that when we close the
  // last open file on this FS handle we will also call disconnect.
  DO_RELEASE(((hdfs_fs*)fs), hdfs_disconnect_and_free);
  return 0;
}


qioerr hdfs_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs)
{
  qioerr err_out = 0;
  int rc;
  hdfs_file* fl = (hdfs_file*)qio_calloc(sizeof(hdfs_file), 1);

  STARTING_SLOW_SYSCALL;
  DO_RETAIN(((hdfs_fs*)fs));

  // assert that we connected
  CREATE_ERROR((to_hdfs_fs(fs)->hfs == NULL), err_out, ECONNREFUSED,"Unable to open HDFS file", error);

  fl->file =  hdfsOpenFile(to_hdfs_fs(fs)->hfs, path, *flags, 0, 0, 0);

  // Assert that we opened the file
  if (fl->file == NULL) {
    err_out = qio_mkerror_errno();
    goto error;
  }

  DONE_SLOW_SYSCALL;

  fl->pathnm = path;

  rc = *flags | ~O_ACCMODE;
  rc &= O_ACCMODE;
  if( rc == O_RDONLY ) {
    *flags |= QIO_FDFLAG_READABLE;
  } else if( rc == O_WRONLY ) {
    *flags |= QIO_FDFLAG_WRITEABLE;
  } else if( rc == O_RDWR ) {
    *flags |= QIO_FDFLAG_READABLE;
    *flags |= QIO_FDFLAG_WRITEABLE;
  }

  *fd = fl; // Set fd to fl and return
  return err_out;

error:
  qio_free(fl);
  return err_out;
}

qioerr hdfs_seek(void* fl, off_t offset, int whence, off_t* offset_out, void* fs)
{
  off_t got;
  qioerr err_out = 0;

  // We cannot seek unless we are in read mode! (HDFS restriction)
  if (to_hdfs_file(fl)->file->type != INPUT)
    QIO_RETURN_CONSTANT_ERROR(ENOSYS, "Seeking is not supported in write mode in HDFS");

  STARTING_SLOW_SYSCALL;
  got = (off_t)hdfsSeek(to_hdfs_fs(fs)->hfs, to_hdfs_file(fl)->file, offset);

  if( got != (off_t) -1) {
    *offset_out = got;
  } else {
    *offset_out = got;
  }
  DONE_SLOW_SYSCALL;

  return err_out;
}

qioerr hdfs_getlength(void* fl, int64_t* len_out, void* fs)
{
  hdfsFileInfo* f_info = NULL;
  f_info = hdfsGetPathInfo(to_hdfs_fs(fs)->hfs, to_hdfs_file(fl)->pathnm);
  if (f_info == NULL)
    QIO_RETURN_CONSTANT_ERROR(EREMOTEIO, "Unable to get length of file in HDFS");
  *len_out = f_info->mSize;
  return 0;
}

qioerr hdfs_fsync(void* fl, void* fs)
{
  int got;
  qioerr err_out = 0;

  STARTING_SLOW_SYSCALL;
  got = hdfsFlush(to_hdfs_fs(fs)->hfs, to_hdfs_file(fl)->file);

  if(got == -1)
    err_out = qio_mkerror_errno();
  DONE_SLOW_SYSCALL;

  return err_out;
}

qioerr hdfs_getcwd(void* file, const char** path_out, void* fs)
{
  int    sz   = 128;
  char*  buf  = (char*) qio_malloc(sz);
  qioerr err = 0;

  if ( !buf )
    QIO_GET_CONSTANT_ERROR(err, ENOMEM, "Out of memory in hdfs_getcwd");

  // hdfsGetWorkingDirectory will return 0 if buf[] is not large enough
  // If this happens, grow the buffer and try again
  while (err == 0 && hdfsGetWorkingDirectory(to_hdfs_fs(fs)->hfs, buf, sz) == 0) {
    if (errno == ERANGE) {
      int   newSz  = 2 * sz;
      char* newBuf = (char*) qio_realloc(buf, newSz);

      if (newBuf == 0) {
        QIO_GET_CONSTANT_ERROR(err, ENOMEM, "Out of memory in hdfs_getcwd");
      } else {
        sz  = newSz;
        buf = newBuf;
      }

    } else {
      // Other error, stop.
      QIO_GET_CONSTANT_ERROR(err, EREMOTEIO, "Unable to get path to file in HDFS");
    }
  }

  if (err != 0) {
    qio_free(buf);
    buf = 0;
  }

  *path_out = buf;

  return err;
}

qioerr hdfs_getpath(void* file, const char** string_out, void* fs)  {
  // Speculatively allocate 128 bytes for the string
  int sz = 128;
  int left = 0;
  char* buf;
  char* got;
  qioerr err = 0;

  const char* host = to_hdfs_fs(fs)->fs_name;
  int port = to_hdfs_fs(fs)->fs_port;
  const char* path = to_hdfs_file(file)->pathnm;

  buf = (char*) qio_malloc(sz);

  if( !buf )
    QIO_GET_CONSTANT_ERROR(err, ENOMEM, "Out of memory in hdfs_getpath");

  while (1) {
    left = snprintf(buf, sz, "hdfs://%s:%d/%s", host, port, path);
    if (left > -1 && left < sz) {
      break;
    } else {
      // keep looping but with bigger buffer.
      // We know the size that we need now if n > -1
      sz = left > -1 ? left + 1 : 2*sz;
      got = (char*) qio_realloc(buf, sz);
      if( ! got ) {
        qio_free(buf);
        QIO_GET_CONSTANT_ERROR(err, ENOMEM, "Out of memory in hdfs_getpath");
      }
    }
  }

  *string_out = buf;
  return err;
}

qioerr hdfs_get_chunk(void* fl, int64_t* len_out, void* fs)
{
  hdfsFileInfo* f_info = NULL;
  f_info = hdfsGetPathInfo(to_hdfs_fs(fs)->hfs, to_hdfs_file(fl)->pathnm);

  if (f_info == NULL) {
     QIO_RETURN_CONSTANT_ERROR(EREMOTEIO, "Unable to get length of file in HDFS");
  }

  *len_out = f_info->mBlockSize;
  return 0;
}

int hdfs_get_fs_type(void* fl, void* fs) 
{
  return FTYPE_HDFS;
}

qio_file_functions_t hdfs_function_struct = {
  &hdfs_writev,
  &hdfs_readv,
  NULL, // No pwrite in HDFS!
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

// ----- multilocale -------

static
char* get_locale_name(char *loc)
{
  const char *keys = "."; // We lop off the .us.cray...
  int i = strcspn (loc, keys);

  return strndup(loc, i);
}

qioerr hdfs_locales_for_range(void* file, off_t start_byte, off_t end_byte, const char*** loc_names_out, int* num_locs_out, void* fs) 
{
  int i = 0;
  int j = 0;
  char*** info = NULL;

  info = hdfsGetHosts(to_hdfs_fs(fs)->hfs, to_hdfs_file(file)->pathnm, start_byte, end_byte);

  // unable to get hosts for this byte range
  if (!info || !info[0]) {
    *num_locs_out = 0;
    hdfsFreeHosts(info);
    QIO_RETURN_CONSTANT_ERROR(EREMOTEIO, "Unable to get owners for byterange");
  }

  while(info[0][i]) {
    info[0][i] = get_locale_name(info[0][i]);
    i++;
  }

  *num_locs_out = i - 1;
  *loc_names_out = (const char**)info[0];

  // Free the other hosts that we don't need
  for (i = 1; info[i]; i++) {
    for (j = 0; info[i][j]; j++)
      qio_free(info[i][j]);
    qio_free(info[i]);
  }

  return 0;
}

// char_arr is already allocated
void hdfs_create_locale_mapping(char ***char_arr, int num, const char *loc_name)
{
  (*char_arr)[num] = (char*)loc_name;
}

char **hdfs_alloc_array(int num_locales)
{
  return (char**)qio_calloc(sizeof(char*), num_locales);
}

qioerr hdfs_get_owners_for_bytes(qio_file_t* file, hdfs_block_byte_map_t** locs, int* out_num_blocks, char** locale_array, int num_locales, off_t start_byte, off_t len)
{
  int i;
  int j = 0;
  int k;
  qioerr err = 0;
  char* tmp;
  int rnd;
  int block_count = 0;
  hdfs_block_byte_map_t* loc = NULL;
  char*** info = NULL;

  hdfsFileInfo* f_info = hdfsGetPathInfo(to_hdfs_fs(file->fs_info)->hfs, to_hdfs_file(file->file_info)->pathnm);

  if (start_byte == 0 && len == -1) // We want the whole thing
    info = hdfsGetHosts(to_hdfs_fs(file->fs_info)->hfs, to_hdfs_file(file->file_info)->pathnm, start_byte, f_info->mSize);
  else info = hdfsGetHosts(to_hdfs_fs(file->fs_info)->hfs, to_hdfs_file(file->file_info)->pathnm, start_byte, start_byte + len);

  while(info[block_count] != NULL) { // Get the number of blocks that we have
    block_count++;
  }

  loc = (hdfs_block_byte_map_t*)qio_calloc(sizeof(hdfs_block_byte_map_t), block_count);

  CREATE_ERROR((!info), err, EREMOTEIO, "Unable to get host for HDFS", end);

  for (i = 0; info[i] != NULL; i++) { // Assign block owners
    rnd = rand() % f_info->mReplication;  // pick an owner
    if (info[i][rnd]) {// Valid access
      tmp = get_locale_name(info[i][rnd]); // strip off .___
      for (k = 0; k < num_locales; k++) { // Now find the owner
        if (strcmp(tmp, locale_array[k]) == 0) {
          loc[i].locale_id = k; // return locale ID for that name
          break;
        }
      }
      loc[i].start_byte  = (off_t)(i*f_info->mBlockSize);
      loc[i].len = (off_t)(f_info->mBlockSize);
      j++;
    } else {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "Unable to find address for blocks in hdfs_get_owners_for_bytes");
      qio_free(loc);
      *locs = NULL;
      *out_num_blocks = 0;
      goto end;
    }
  }

  *locs = loc;
  *out_num_blocks = j;

end:
  return err;
}

qioerr hdfs_get_owners(qio_file_t* file, hdfs_block_byte_map_t** locs_out, int* out_num_blocks, char** locale_array, int num_locales)
{
  return hdfs_get_owners_for_bytes(file, locs_out, out_num_blocks, locale_array, num_locales, 0, -1);
}

hdfs_block_byte_map_t hdfs_index_array(hdfs_block_byte_map_t* locs, int index)
{
  return locs[index];
}
