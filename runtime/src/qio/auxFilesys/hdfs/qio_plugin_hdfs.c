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

#ifndef SIMPLE_TEST
#include "chplrt.h"
#endif

#include <hdfs.h>
#include "qio_plugin_hdfs.h"

//// So we can deal with the void*'s passed in without messy casting all over the place
#define to_hdfs_file(f) ((hdfs_file*)f)

#define CREATE_ERROR(test, var, errno, string, goto_name) { \
  if(test) { \
    QIO_GET_CONSTANT_ERROR(var, errno, string); \
    goto goto_name; \
  } \
}

// The "fd" for HDFS
struct hdfs_file {
  hdfsFS fs;          // Our filesystem
  hdfsFile file;      // file info
  const char* pathnm; // The path to our file. Need this for locality 
};

qioerr hdfs_readv (void* file, const struct iovec *vector, int count, ssize_t* num_read_out)
{
  ssize_t got;
  ssize_t got_total;
  qioerr err_out = 0;
  int i;

  STARTING_SLOW_SYSCALL;

  got_total = 0;
  for( i = 0; i < count; i++) {
    got = hdfsRead(to_hdfs_file(file)->fs, to_hdfs_file(file)->file, (void*)vector[i].iov_base, vector[i].iov_len);
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

qioerr hdfs_pwritev(void* fd, const struct iovec* iov, int iovcnt, off_t see_to_offset, ssize_t* num_read_out) 
{
  qioerr t = 0; 
  QIO_GET_CONSTANT_ERROR(t, ENOSYS, "pwrites in HDFS are not supported");
  fprintf(stderr, "positional writes in HDFS are not supported!!\n");
  return t;
}

qioerr hdfs_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out) 
{
  ssize_t got;
  ssize_t got_total = 0;
  qioerr err_out = 0;
  int i;

  STARTING_SLOW_SYSCALL;
  for (i = 0; i < iovcnt; i++) {
    got = hdfsWrite(to_hdfs_file(fl)->fs, to_hdfs_file(fl)->file, (void*)(iov[i].iov_base), iov[i].iov_len);

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

qioerr hdfs_preadv (void* file, const struct iovec *vector, int count, off_t offset, ssize_t* num_read_out)
{
  ssize_t got;
  ssize_t got_total;
  qioerr err_out = 0;
  int i;

  STARTING_SLOW_SYSCALL;

  err_out = 0;
  got_total = 0;
  for(i = 0; i < count; i++) {
    got = hdfsPread(to_hdfs_file(file)->fs, to_hdfs_file(file)->file, offset + got_total, (void*)vector[i].iov_base, vector[i].iov_len);
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

qioerr hdfs_close(void* fl) 
{
  int got;
  qioerr err_out = 0;

  STARTING_SLOW_SYSCALL;
  got = hdfsCloseFile(to_hdfs_file(fl)->fs, to_hdfs_file(fl)->file);

  if(got == -1)
    err_out = qio_mkerror_errno();
  DONE_SLOW_SYSCALL;

  return err_out;
}

qioerr hdfs_connect(void** fs_out, const char* pathname, int port) 
{
  qioerr err_out = 0;
  hdfsFS fs;

  STARTING_SLOW_SYSCALL;
  fs = hdfsConnect(pathname, port);

  CREATE_ERROR((fs == NULL), err_out, ECONNREFUSED,"Unable to connect HDFS", error);
  DONE_SLOW_SYSCALL;

  *fs_out = fs;

error:
  return err_out;
}

qioerr hdfs_disconnect(void* fs) 
{
  qioerr err_out = 0;
  int ret = 0;

  STARTING_SLOW_SYSCALL;
  ret = hdfsDisconnect((hdfsFS)fs);

  if (ret == -1) 
    err_out = qio_mkerror_errno();

  DONE_SLOW_SYSCALL;

  return err_out;
}


qioerr hdfs_open(void** fd, const char* path, int* flags, mode_t mode, qio_hint_t iohints, void* fs) 
{
  qioerr err_out = 0;
  int rc;
  hdfs_file* fl = (hdfs_file*)qio_calloc(sizeof(hdfs_file), 1);

  STARTING_SLOW_SYSCALL;
  fl->fs = fs;
  
  // assert that we connected 
  CREATE_ERROR((fl->fs == NULL), err_out, ECONNREFUSED,"Unable to open HDFS file", error);

  fl->file =  hdfsOpenFile(fl->fs, path, *flags, 0, 0, 0);

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

qioerr hdfs_seek(void* fl, off_t offset, int whence, off_t* offset_out)
{
  off_t got;
  qioerr err_out = 0;

  STARTING_SLOW_SYSCALL;
  got = (off_t)hdfsSeek(to_hdfs_file(fl)->fs, to_hdfs_file(fl)->file, offset);

  if( got != (off_t) -1) {
    *offset_out = got;
  } else {
    *offset_out = got;
    err_out = qio_mkerror_errno();
  }
  DONE_SLOW_SYSCALL;

  return err_out;
}

qioerr hdfs_fsync(void* fl)
{
  int got;
  qioerr err_out = 0;

  STARTING_SLOW_SYSCALL;
  got = hdfsFlush(to_hdfs_file(fl)->fs, to_hdfs_file(fl)->file);

  if(got == -1)     
    err_out = qio_mkerror_errno();
  DONE_SLOW_SYSCALL;

  return err_out;
}

qioerr hdfs_getcwd(void* file, const char** path_out)
{
  int sz = 128;
  char* buf;
  char* got;
  qioerr err = 0;

  buf = (char*) qio_malloc(sz);
  if( !buf ) 
    QIO_GET_CONSTANT_ERROR(err, ENOMEM, "Out of memory in hdfs_getcwd");
  while( 1 ) {
    got = hdfsGetWorkingDirectory(to_hdfs_file(file)->fs, buf, sz);
    if( got != NULL ) break;
    else if( errno == ERANGE ) { 
      // keep looping but with bigger buffer.
      sz = 2*sz;
      got = (char*) qio_realloc(buf, sz);
      if( ! got ) {
        qio_free(buf);
        QIO_GET_CONSTANT_ERROR(err, ENOMEM, "Out of memory in hdfs_getcwd");
      }
    } else {
      // Other error, stop.
        QIO_GET_CONSTANT_ERROR(err, EREMOTEIO, "Unable to get path to file in HDFS");
    }
  }

  *path_out = buf;
  return err;
}

qioerr hdfs_getpath(void* file, const char** string_out) 
{
  qioerr err = 0;
  *string_out = to_hdfs_file(file)->pathnm;
  return err;
}

qio_file_functions_ptr_t hdfs_create_file_functions(void* fs) 
{
  qio_file_functions_t* s = (qio_file_functions_t*)qio_calloc(sizeof(qio_file_functions_t), 1);

  s->writev     = &hdfs_writev;
  s->pwritev    = NULL; // No pwrite in HDFS!
  s->readv      = &hdfs_readv;
  s->preadv     = &hdfs_preadv;
  s->close      = &hdfs_close;
  s->open       = &hdfs_open;
  s->seek       = &hdfs_seek; 
  s->fsync      = &hdfs_fsync;
  s->filelength = NULL; // Don't need this for HDFS 
  s->getpath    = &hdfs_getpath; 
  s->getcwd     = &hdfs_getcwd;

  s->fs = fs;

  return s;
}

// ----- multilocale -------

static 
char* get_locale_name(char *loc) 
{
  const char *keys = "."; // We lop off the .us.cray...
  int i = strcspn (loc, keys); 

  return strndup(loc, i);
}

// char_arr is already allocated
void hdfs_create_locale_mapping(char ***char_arr, int num, char *loc_name) 
{
  (*char_arr)[num] = loc_name;
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

  hdfsFileInfo* f_info = hdfsGetPathInfo(to_hdfs_file(file->info)->fs, to_hdfs_file(file->info)->pathnm);

  if (start_byte == 0 && len == -1) // We want the whole thing
    info = hdfsGetHosts(to_hdfs_file(file->info)->fs, to_hdfs_file(file->info)->pathnm, start_byte, f_info->mSize);
  else info = hdfsGetHosts(to_hdfs_file(file->info)->fs, to_hdfs_file(file->info)->pathnm, start_byte, start_byte + len);

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
