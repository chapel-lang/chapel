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

#define _QIO_C

#ifndef _DARWIN_C_SOURCE
// get fcntl(F_GETPATH)
#define _DARWIN_C_SOURCE
#endif

#ifndef _GNU_SOURCE
// get O_DIRECT, fopencookie
#define _GNU_SOURCE
#endif

#include "sys_basic.h"

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "qio.h"
#include "qbuffer.h"

#include "error.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/select.h>
//#include <sys/fcntl.h> no sys/fcntl.h on AIX, fcntl.h should cover it.
#include <sys/stat.h>

#include <assert.h>

ssize_t qio_too_small_for_default_mmap = 16*1024;
ssize_t qio_too_large_for_default_mmap = 64*1024*((size_t)1024*1024);
ssize_t qio_mmap_chunk_iobufs = 128; // mmap 128 iobufs at a time (8M)

// Future - possibly set this based on ulimit?
ssize_t qio_initial_mmap_max = 8*1024*1024;
bool qio_allow_default_mmap = true;

#ifdef _chplrt_H_
qioerr qio_lock(qio_lock_t* x) {
  // recursive mutex based on glibc pthreads implementation
  int64_t id = chpl_task_getId();

  assert( id != NULL_OWNER );

  // check whether we already hold the mutex.
  if( x->owner == id ) {
    // just bump the counter.
    ++x->count;
    return 0;
  }

  // we have to get the mutex.
  chpl_sync_lock(&x->sv);

  assert( x->owner == NULL_OWNER );
  x->count = 1;
  x->owner = id;

  return 0;
}
void qio_unlock(qio_lock_t* x) {
  int64_t id = chpl_task_getId();

  // recursive mutex based on glibc pthreads implementation
  if( x->owner != id ) {
    abort();
  }

  if (--x->count != 0 ) {
    // we still hold the mutex.
    return;
  }

  x->owner = NULL_OWNER;
  chpl_sync_unlock(&x->sv);
}
#endif

qioerr qio_readv(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_read)
{
  ssize_t nread = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err = 0;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative count");
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // read into our buffer.
  if (file->fd != -1) // See if we have an fd
    err = qio_int_to_err(sys_readv(file->fd, iov, iovcnt, &nread));
  else // Dont have an fd
    if(file->fsfns){ // We have a foreign function?
      if (file->fsfns->readv) { // Do we have readv?
        err = file->fsfns->readv(file->file_info, iov, iovcnt, &nread, file->fs_info);
      } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing readv");
    } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_read = nread;

  DONE_SLOW_SYSCALL;

  return err;
}

qioerr qio_writev(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_written)
{
  ssize_t nwritten = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative count");
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // write from our buffer
  if (file->fd != -1) // Have fd?
    err = qio_int_to_err(sys_writev(file->fd, iov, iovcnt, &nwritten));
  else // Dont have an fd
    if (file->fsfns) { // We have something 
      if (file->fsfns->writev) {// So see if we have writev
        err = file->fsfns->writev(file->file_info, iov, iovcnt, &nwritten, file->fs_info);
      } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing writev");
    } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_written = nwritten;

  DONE_SLOW_SYSCALL;

  return err;
}

qioerr qio_preadv(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, ssize_t* num_read)
{
  ssize_t nread = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative count");
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // read into our buffer.
  if (file->fd != -1) // Do we have an fd?
    err = qio_int_to_err(sys_preadv(file->fd, iov, iovcnt, seek_to_offset, &nread));
  else 
  if (file->fsfns){ // Have something
    if (file->fsfns->preadv) {// We have preadv
      err = file->fsfns->preadv(file->file_info, iov, iovcnt, seek_to_offset, &nread, file->fs_info);
    } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing preadv");
  } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_read = nread;
  DONE_SLOW_SYSCALL;

  return err;

}

qioerr qio_freadv(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_read)
{
  int64_t total_read = 0;
  size_t nread;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative count");
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  for( i = 0; i < iovcnt; i++ ) {
    nread = fread(iov[i].iov_base, 1, iov[i].iov_len, fp);
    err = 0;
    if( nread == 0 ) {
      if( feof(fp) ) err = QIO_EEOF;
      else err = qio_int_to_err(ferror(fp));
    }
    total_read += nread;
    if( err ) goto error;
  }

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_read = total_read;
  DONE_SLOW_SYSCALL;

  return err;

}

qioerr qio_fwritev(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_written)
{
  ssize_t total_written = 0;
  size_t nwritten;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  for( i = 0; i < iovcnt; i++ ) {
    nwritten = fwrite(iov[i].iov_base, 1, iov[i].iov_len, fp);
    err = 0;
    if( nwritten == 0 ) {
      err = qio_int_to_err(ferror(fp));
    }
    total_written += nwritten;
    if( err ) goto error;
  }

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_written = total_written;
  DONE_SLOW_SYSCALL;

  return err;

}



qioerr qio_pwritev(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, ssize_t* num_written)
{
  ssize_t nwritten = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // write from our buffer
  if (file->fd != -1) // So see if we have an fd we can use
    err = qio_int_to_err(sys_pwritev(file->fd, iov, iovcnt, seek_to_offset, &nwritten));
  else // Don't have an fd
  if (file->fsfns) { // We have something
    if (file->fsfns->pwritev) { // Do we have pwritev
      err = file->fsfns->pwritev(file->file_info, iov, iovcnt, seek_to_offset, &nwritten, file->fs_info);
    } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing pwritev");
  } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_written = nwritten;

  DONE_SLOW_SYSCALL;

  return err;
}

qioerr qio_recv(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int flags,
              sys_sockaddr_t* src_addr_out, /* can be NULL */
              void* ancillary_out, socklen_t* ancillary_len_inout, /* can be NULL */
              ssize_t* num_recvd_out)
{
  struct msghdr msg;
  ssize_t nrecvd = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;

  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  memset(&msg, 0, sizeof(struct msghdr));
  if( src_addr_out ) {
    msg.msg_name = (void*) &src_addr_out->addr;
    msg.msg_namelen = src_addr_out->len;
  }
  msg.msg_iov = iov;
  msg.msg_iovlen = num_parts;
  if( ancillary_out && ancillary_len_inout ) {
    msg.msg_control = ancillary_out;
    msg.msg_controllen = *ancillary_len_inout;
  }

  err = qio_int_to_err(sys_recvmsg(sockfd, &msg, flags, &nrecvd));
  if( ! err ) {
    if( src_addr_out ) src_addr_out->len = msg.msg_namelen;
    if( ancillary_out && ancillary_len_inout ) *ancillary_len_inout = msg.msg_controllen;
  }

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_recvd_out = nrecvd;

  DONE_SLOW_SYSCALL;

  return err;
}

qioerr qio_send(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int flags,
              const sys_sockaddr_t* dst_addr, /* can be NULL */
              const void* ancillary, /* can be NULL */
              socklen_t ancillary_len,
              ssize_t* num_sent_out)
{
  struct msghdr msg;
  ssize_t nsent = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;

  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "range outside of buffer");
    goto error;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  memset(&msg, 0, sizeof(struct msghdr));
  if( dst_addr ) {
    msg.msg_name = (void*) &dst_addr->addr;
    msg.msg_namelen = dst_addr->len;
  }
  msg.msg_iov = iov;
  msg.msg_iovlen = num_parts;
  if( ancillary ) {
    msg.msg_control = (void*) ancillary;
    msg.msg_controllen = ancillary_len;
  }
  err = qio_int_to_err(sys_sendmsg(sockfd, &msg, flags, &nsent));

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_sent_out = nsent;

  DONE_SLOW_SYSCALL;

  return err;
}

static
qio_hint_t choose_io_method(qio_file_t* file, qio_hint_t hints, qio_hint_t default_hints, int64_t file_size, int reading, int writing, int isfilestar)
{
  qio_fdflag_t fdflags = file->fdflags;
  qio_hint_t method = (qio_hint_t) (hints & QIO_METHODMASK);
  qio_chtype_t type = (qio_chtype_t) (hints & QIO_CHTYPEMASK);
  qio_hint_t ret = hints;
 
  ret &= ~(QIO_METHODMASK|QIO_CHTYPEMASK); // clear method number, channel type

  // 'or' in hints from default_hints.
  ret |= (default_hints & ~(QIO_METHODMASK|QIO_CHTYPEMASK));

  if (file->fsfns) { // We have a foreign FS
    if(fdflags & QIO_FDFLAG_SEEKABLE) { // We can seek
    // Read only
    if((fdflags & QIO_FDFLAG_READABLE)    && 
        !(fdflags & QIO_FDFLAG_WRITEABLE) && 
        file->fsfns->preadv)
      method = QIO_METHOD_PREADPWRITE;
    
    // Write only
    if((fdflags & QIO_FDFLAG_WRITEABLE)  && 
        !(fdflags & QIO_FDFLAG_READABLE) &&
        file->fsfns->pwritev)
      method = QIO_METHOD_PREADPWRITE;

    // Read and write
    if((fdflags & QIO_FDFLAG_READABLE)   &&
        (fdflags & QIO_FDFLAG_WRITEABLE) &&
        file->fsfns->preadv && file->fsfns->pwritev)
      method = QIO_METHOD_PREADPWRITE;
    } else method = QIO_METHOD_READWRITE; // Else we can't seek, so set to use readv and writev

  } else { // Regular FS. So do what we did before
    if( method < QIO_MIN_METHOD || method > QIO_MAX_METHOD ) {
      // bad method number. Use default, or choose one.
      if( default_hints & QIO_METHODMASK ) {
        method = default_hints & QIO_METHODMASK;
      } else {
        // Choose one.
        if( isfilestar ) {
          // Always default to fread/fwrite with FILE* file pointers.
          method = QIO_METHOD_FREADFWRITE;
        } else if( fdflags & QIO_FDFLAG_SEEKABLE ) {
          if( hints & QIO_HINT_NOREUSE ) method = QIO_METHOD_PREADPWRITE;
          else if( hints & QIO_HINT_CACHED ) method = QIO_METHOD_MMAP;
          else {
            // default case
            if( qio_allow_default_mmap && (!writing) &&
                qio_too_small_for_default_mmap <= file_size &&
                file_size < SSIZE_MAX &&
                file_size < qio_too_large_for_default_mmap &&
                (qbytes_iobuf_size & 4095) == 0) {
              // not writing, not too small, not too big, iobuf size multiple of 4k.
              method = QIO_METHOD_MMAP;
            } else {
              method = QIO_METHOD_PREADPWRITE;
            }
          }
        } else {
          // TODO: use libevent
          // for now, we just use READWRITE.
          method = QIO_METHOD_READWRITE;
        }
      }
    } else {
      // method already chosen in hints.
    }
  }

  // Always use fread/fwrite with FILE*
  //if( file->fp ) method = QIO_METHOD_FREADFWRITE;
  // we get FILE* from tmpfile() and want to be able to mmap...

  if( type < QIO_CH_MIN_TYPE || type > QIO_CH_MAX_TYPE ) {
    // bad type number. Use default, or choose one.
    if( default_hints & QIO_CHTYPEMASK ) {
      type = (qio_chtype_t) (default_hints & QIO_CHTYPEMASK);
    } else {
      type = QIO_CH_BUFFERED;
    }
  } else {
    // type is O.K.
  }

  return ret | method | type;
}

static
qioerr qio_fadvise_for_hints(qio_file_t* file)
{
  int64_t len = file->initial_length;
  int advice = 0;
  qioerr err;

  if( file->hints & QIO_HINT_DIRECT ) {
    int rc;
    long arg;
    // Set O_DIRECT on our fd.
    err = qio_int_to_err(sys_fcntl(file->fd, F_GETFL, &rc));
    if( err ) return err;

    arg = rc;
#ifdef O_DIRECT
    arg |= O_DIRECT;
#endif
    err = qio_int_to_err(sys_fcntl_long(file->fd, F_SETFL, arg, &rc));
    if( err ) return err;
  } else {
    err = 0;
#if (_XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L)
#ifdef POSIX_FADV_RANDOM
    if( file->hints & QIO_HINT_RANDOM ) advice |= POSIX_FADV_RANDOM;
#endif
#ifdef POSIX_FADV_SEQUENTIAL
    if( file->hints & QIO_HINT_SEQUENTIAL ) advice |= POSIX_FADV_SEQUENTIAL;
#endif
#ifdef POSIX_FADV_NOREUSE
    if( file->hints & QIO_HINT_NOREUSE ) advice |= POSIX_FADV_NOREUSE;
#endif
#ifdef POSIX_FADV_WILLNEED
    if( file->hints & QIO_HINT_CACHED ) advice |= POSIX_FADV_WILLNEED;
#endif

    if( advice == 0 ) err = 0; // do nothing.
    else err = qio_int_to_err(sys_posix_fadvise(file->fd, 0, len, advice));

    if( err ) return err;
#endif
  }

  return 0;
}

static
qioerr qio_madvise_for_hints(void* data, int64_t len, qio_hint_t hints)
{
  int advice = 0;
  qioerr err;

#ifdef POSIX_MADV_RANDOM
  if( hints & QIO_HINT_RANDOM ) advice |= POSIX_MADV_RANDOM;
#endif
#ifdef POSIX_MADV_SEQUENTIAL
  if( hints & QIO_HINT_SEQUENTIAL ) advice |= POSIX_MADV_SEQUENTIAL;
#endif
#ifdef POSIX_MADV_WILLNEED
  if( hints & QIO_HINT_CACHED ) advice |= POSIX_MADV_WILLNEED;
#endif

  if( advice == 0 ) err = 0; // do nothing.
  else err = qio_int_to_err(sys_posix_madvise(data, len, advice));

  return err;
}

static
qioerr qio_mmap_initial(qio_file_t* file)
{
  int64_t len = file->initial_length;
  int do_mmap_initial = 0;
  qioerr err;

  // now, if we're using mmap, go ahead and map the file.
  // note that here file->hints might be the result of choose_io_method.
  if( (file->hints & QIO_METHODMASK) == QIO_METHOD_MMAP ) {
    if( len > 0 ) {
      // Can't mmap a zero-length file initially
      if ( (file->hints & QIO_HINT_PARALLEL) ||
           (len <= qio_initial_mmap_max) ) {
        do_mmap_initial = 1;
      }
    }
  }

  if( do_mmap_initial ) {
    void* data;
    int prot = PROT_READ;
    int populate = 0;

#ifdef MAP_POPULATE
    if( file->hints & QIO_HINT_CACHED ) {
      populate = MAP_POPULATE;
    }
#endif

    if( file->fdflags & QIO_FDFLAG_WRITEABLE ) prot |= PROT_WRITE;

    if( len > SSIZE_MAX ) return QIO_ENOMEM;

    // mmap the initial length of the file.
    err = qio_int_to_err(sys_mmap(NULL, len, prot, MAP_SHARED|populate, file->fd, 0, &data));
    if( err ) return err;

    err = qio_madvise_for_hints(data, len, file->hints);
    if( err ) {
      sys_munmap(data, len);
      return err;
    }
 
    err = qbytes_create_generic(&file->mmap, data, len, qbytes_free_munmap);
    if( err ) {
      // A nonzero err code indicates that allocation of the qbytes buffer
      // failed.  Therefore, we do not have to release it here.
      sys_munmap(data, len);
      return err;
    }
  }

  return 0;
}


qioerr qio_file_init(qio_file_t** file_out, FILE* fp, fd_t fd, qio_hint_t iohints, const qio_style_t* style, int usefilestar)
{
  off_t initial_pos = 0;
  off_t initial_length = 0;
  int rc;
  qioerr err;
  qio_fdflag_t fdflags = (qio_fdflag_t) 0;
  qio_file_t* file = NULL;
  struct stat stats;
  off_t seek_ret;
  qio_chtype_t hinted_type;
  int seekable = 0;
  mode_t ftype;

  if( fp ) {
    fd = fileno(fp);
    if( fd == -1 ) return qio_mkerror_errno();
  }

  err = qio_int_to_err(sys_fstat(fd, &stats));
  if( err ) return err;

  ftype = stats.st_mode & S_IFMT;

  if( ftype == S_IFIFO ||
      ftype == S_IFCHR ||
      ftype == S_IFSOCK ) {
    // FIFO/named pipe, character device, or socket cannot seek
    seekable = 0;
  } else if( ftype == S_IFREG ||
             ftype == S_IFBLK ) {
    // regular file or block device can seek
    seekable = 1;
  } else {
    // ftype == S_IFDIR
    // ftype == S_IFLNK
    // ftype == S_IFWHT on Mac OS X
    // can't open symlink/dir/whiteout
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "file type not openable");
  }

  if( seekable ) {
    // try to seek.
    err = qio_int_to_err(sys_lseek(fd, 0, SEEK_CUR, &seek_ret));
    if( qio_err_to_int(err) == ESPIPE ) {
      // not seekable. Don't worry about it.
      seekable = 0;
    } else if( err ) {
      return err;
    } else {
      seekable = 1;
    }
  }

  if( seekable ) {
    // seekable.
    fdflags = (qio_fdflag_t) (fdflags | QIO_FDFLAG_SEEKABLE);
    initial_pos = seek_ret;
    // get the file length, using stat (not seek)
    // so that this is thread-safe.
    initial_length = stats.st_size;
  } else {
     // Not seekable.
    initial_pos = 0;
  }

  // try to fcntl
  err = qio_int_to_err(sys_fcntl(fd, F_GETFL, &rc));
  if( err ) {
    return err;
  } else {
    rc &= O_ACCMODE;
    // When setting the file mode, we pretend no matter what
    // that stdin is read-only and stdout/stderr are write-only.
    // We do this to avoid program launch bugs with some uses of nohup.
    // Trying to read from an invalid stdin or write to an invalid stdout
    // will lead to an error later.
    if( rc == O_RDONLY || fd == STDIN_FILENO ) {
      fdflags = (qio_fdflag_t) (fdflags | QIO_FDFLAG_READABLE);
    } else if( rc == O_WRONLY || fd == STDOUT_FILENO || fd == STDERR_FILENO){
      fdflags = (qio_fdflag_t) (fdflags | QIO_FDFLAG_WRITEABLE);
    } else if( rc == O_RDWR ) {
      fdflags = (qio_fdflag_t) (fdflags | QIO_FDFLAG_READABLE);
      fdflags = (qio_fdflag_t) (fdflags | QIO_FDFLAG_WRITEABLE);
    }
    // could get other access flags to know if e.g. append only
  }
  
  
  file = (qio_file_t*) qio_calloc(sizeof(qio_file_t), 1);
  if( ! file ) {
    return QIO_ENOMEM;
  }

  DO_INIT_REFCNT(file);
  file->fp = fp;
  file->fd = fd;
  file->use_fp = usefilestar;
  file->buf = NULL;
  file->fdflags = fdflags;
  file->initial_length = initial_length;
  file->initial_pos = initial_pos;
  file->fsfns = NULL; // Dont have anything so set it to NULL

  hinted_type = (qio_chtype_t) (iohints & QIO_METHODMASK);

  file->hints = choose_io_method(file, iohints, 0, initial_length,
                                 (fdflags & QIO_FDFLAG_READABLE) > 0,
                                 (fdflags & QIO_FDFLAG_WRITEABLE) > 0,
                                 file->fp != 0 && file->use_fp );

  file->mmap = NULL;
  err = qio_lock_init(&file->lock);
  if( err ) goto error;
  file->max_initial_position = -1;

  err = qio_fadvise_for_hints(file);
  if( err ) goto error;

  err = qio_mmap_initial(file);
  if( err ) goto error;

  // put file->hints back to DEFAULT if that's what we started with.
  if( hinted_type == QIO_METHOD_DEFAULT ) {
    file->hints &= ~QIO_METHODMASK; // clear the method.
    file->hints |= QIO_METHOD_DEFAULT;
  }

  if( style ) qio_style_copy(&file->style, style);
  else qio_style_init_default(&file->style);

  *file_out = file;
  return 0;

error:
  qio_free(file);
  *file_out = NULL;
  return err;
}

qioerr qio_file_init_usr(qio_file_t** file_out, void* file_info, qio_hint_t iohints, int flags, const qio_style_t* style, void* fs_info, const qio_file_functions_t* fns)
{
  off_t initial_pos = 0;
  int64_t initial_length = 0;
  qioerr err = 0;
  err_t err_code;
  qio_file_t* file = NULL;
  off_t seek_ret = 0;
  int seekable = 0;

  if(fns->seek) { // we have seek in our FS
    // try to seek.
    err = fns->seek(file_info, 0, SEEK_CUR, &seek_ret, fs_info);
    err_code = qio_err_to_int(err);
    if( err_code == ESPIPE || err_code == ENOSYS || err_code == EINVAL ) {
      // not seekable. Don't worry about it.
      seekable = 0;
    } else if( err ) {
      return err;
    } else {
      seekable = 1;
    }
  }

  if( (flags & QIO_FDFLAG_SEEKABLE) > 0 ) {
    seekable = 1;
  }

  if (fns->filelength) { // We can get length in our FS
    err = fns->filelength(file_info, &initial_length, fs_info);
    // Disregard errors in case it is not seekable (and if we need seek to get the
    // length). If we can't get the length, we'll set initial_pos below anyways.
    err = 0;
  }

  if( seekable ) {
    // seekable.
    flags = (qio_fdflag_t) (flags | QIO_FDFLAG_SEEKABLE);
    initial_pos = seek_ret;
  } else {
     // Not seekable.
    initial_pos = 0;
  }
  
  file = (qio_file_t*) qio_calloc(sizeof(qio_file_t), 1);
  if( ! file ) {
    return QIO_ENOMEM;
  }

  DO_INIT_REFCNT(file);
  file->fp = NULL;
  file->fd = -1;
  file->use_fp = 0;
  file->buf = NULL;
  file->fdflags = (qio_fdflag_t) flags;
  file->initial_length = initial_length;
  file->initial_pos = initial_pos;
  file->fs_info = fs_info;
  file->fsfns = fns; // Put our functions in
  file->file_info  = file_info;

  file->hints = choose_io_method(file, iohints, 0, initial_length,
                                 (flags & QIO_FDFLAG_READABLE) > 0,
                                 (flags & QIO_FDFLAG_WRITEABLE) > 0,
                                 file->fp != 0 && file->use_fp );

  file->mmap = NULL;
  err = qio_lock_init(&file->lock);
  if( err ) goto error;
  file->max_initial_position = -1;

  // MPF - qio_file_init puts file->hints back to DEFAULT if
  // that's what we started with, should this function do that
  // as well?

  if( style ) qio_style_copy(&file->style, style);
  else qio_style_init_default(&file->style);

  *file_out = file;
  return 0;

error:
  qio_free(file);
  *file_out = NULL;
  return err;
}

static
qioerr _qio_file_do_close(qio_file_t* f)
{
  qioerr err = 0;
  qioerr newerr;
  const char* path;
  int rc;

  //printf("closing %p fd %i fp %p\n", f, f->fd, f->fp);


  err = qio_lock(& f->lock);
  if( err ) return err;

  if( f->mmap ) {
    // munmap
    qbytes_release(f->mmap);
    f->mmap = NULL;
  }

  if( f->buf ) {
    qbuffer_release(f->buf);
    f->buf = NULL;
  }

  if( f->fp ) {
    if (f->hints & QIO_HINT_OWNED) {
      rc = fclose(f->fp);
      if( rc ) err = qio_mkerror_errno();
    }
    f->hints &= ~QIO_HINT_OWNED;
    f->fp = NULL;
    f->fd = -1;
  }

  if (f->fsfns) {
    if (f->hints & QIO_HINT_OWNED)  // Should always be true
      err = f->fsfns->close(f->file_info, f->fs_info);
    f->hints &= ~QIO_HINT_OWNED;
  }

  if( f->fd >= 0 ) {
    if (f->hints & QIO_HINT_OWNED)
      err = qio_int_to_err(sys_close(f->fd));
    if( err ) {
      newerr = qio_file_path(f, &path);
      if( newerr ) {
        fprintf(stderr, "Warning: error %i on file close; additionally, error getting name of file\n", qio_err_to_int(err));
      } else {
        fprintf(stderr, "Warning: error %i on file close for file %s\n",
                qio_err_to_int(err), path);
      }
    }
    f->hints &= ~QIO_HINT_OWNED;
    f->fp = NULL;
    f->fd = -1;
  }

  qio_unlock(& f->lock);

  return err;
}

qioerr qio_file_close(qio_file_t* f)
{
  // TODO - we could check to see if
  // there are references to the file
  // but we'd have to do it with
  // atomic-safe load.
  //
  // This check is currently disabled
  // b/c of reference counting bugs
  // in Chapel.
  //
  //if( f->ref_cnt > 1 ) return EINVAL;

  return _qio_file_do_close(f);
}

qioerr qio_file_sync(qio_file_t* f)
{
  qioerr err = 0;
  qioerr newerr;

  if( f->fp ) {
    newerr = qio_int_to_err(fflush(f->fp));
    if( ! err ) err = newerr;
    newerr = qio_int_to_err(sys_fsync(fileno(f->fp)));
    if( ! err ) err = newerr;
  } else if( f->fd >= 0 ) {
    err = qio_int_to_err(sys_fsync(f->fd));
  } else if( f->fsfns && f->fsfns->fsync ) {
    err = f->fsfns->fsync(f->file_info, f->fs_info);

  }

  return err;
}

void _qio_file_destroy(qio_file_t* f)
{
  qioerr err;

  if( DEBUG_QIO ) printf("Destroying file %p\n", f);

  err = qio_file_close(f);
  if( err ) {
    fprintf(stderr, "Warning: error %i when automatically closing file.\n", qio_err_to_int(err));
    assert( !err );
    abort();
  }

  qio_lock_destroy(&f->lock);

  qbytes_release(f->mmap); // Does nothing if null.

  qbuffer_release(f->buf); // Does nothing if null.

  DO_DESTROY_REFCNT(f);

  qio_free(f);
}

static
qioerr open_flags_for_string(const char* s, int *flags_out)
{
  if( 0 == strcmp(s, "r") ) {
    *flags_out = O_RDONLY;
  } else if( 0 == strcmp(s, "r+") ) {
    *flags_out = O_RDWR;
  } else if( 0 == strcmp(s, "w") ) {
    *flags_out = O_WRONLY | O_CREAT | O_TRUNC;
  } else if( 0 == strcmp(s, "w+") ) {
    *flags_out = O_RDWR | O_CREAT | O_TRUNC;
  } else if( 0 == strcmp(s, "a") ) {
    *flags_out = O_WRONLY | O_CREAT | O_APPEND;
  } else if( 0 == strcmp(s, "a+") ) {
    *flags_out = O_RDWR | O_CREAT | O_APPEND;
  } else {
    *flags_out = 0;
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "bad access string");
  }
  return 0;
}
static
const char* string_for_open_flags(int flags)
{
  if( flags == O_RDONLY ) return "r";
  else if( flags == O_RDWR ) return "r+";
  else if( flags == (O_WRONLY | O_CREAT | O_TRUNC) ) return "w";
  else if( flags == (O_RDWR | O_CREAT | O_TRUNC) ) return "w+";
  else if( flags == (O_WRONLY | O_CREAT | O_APPEND) ) return "a";
  else if( flags == (O_RDWR | O_CREAT | O_APPEND) ) return "a+";
  else {
    assert(0);
    return "r";
  }
}
// always enable reading (possibly in addition to writing).
static
int flags_for_mmap_open(int flags)
{
  int acc = flags & O_ACCMODE;
  int rest = flags & ~O_ACCMODE;

  if( acc == O_WRONLY ) acc = O_RDWR;

  return acc | rest;
}



// mode should default to S_IRUSR | S_IWUSR | S_IRGRP |  S_IWGRP |  S_IROTH  |  S_IWOTH
// iohints should default to 0
qioerr qio_file_open(qio_file_t** file_out, const char* pathname, int flags, mode_t mode, qio_hint_t iohints, const qio_style_t* style)
{
  FILE* fp = NULL;
  fd_t fd = -1;
  qioerr err;
  qio_method_t method = (qio_method_t) (iohints & QIO_METHODMASK);

  if( method == QIO_METHOD_FREADFWRITE ) {
    //err = sys_fopen(pathname, string_for_open_flags(flags), &fp);
    fp = fopen(pathname, string_for_open_flags(flags));
    if( ! fp ) err = qio_mkerror_errno();
    else err = 0;
  } else {
    // Linux needs O_RDWR if we're writing with MMAP (vs O_WRONLY).
    if( method == QIO_METHOD_MMAP ) {
      flags = flags_for_mmap_open(flags);
    }
    err = qio_int_to_err(sys_open(pathname, flags, mode, &fd));
  }

  if( err ) {
    *file_out = NULL;
    return err;
  }

  // We opened this file, so file_out owns it.
  // On error , file_out is NULL, so deleting it is harmless.
  return qio_file_init(file_out, fp, fd, iohints | QIO_HINT_OWNED, style, fp != NULL);
}

qioerr qio_file_open_usr(qio_file_t** file_out, const char* pathname, int flags, mode_t mode, qio_hint_t iohints, const qio_style_t* style, void* fs_info, const qio_file_functions_t* s)
{
  qioerr err = 0;
  void* file_info;

  if (s->open) { // We have open
    err = s->open(&file_info, pathname, &flags, mode, iohints, fs_info);
    // MPF - commented out the code below because we need
    // a different (more generic) way to handle it. In particular,
    // an HDFS seek function should just return EINVAL, ESPIPE, or ENOSYS
    // for files that can't seek. 
    //if (flags & O_WRONLY) // Specific to HDFS. 
    // s->seek = NULL;  // We can only seek when opened in O_RDONLY
  } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing open");

  if( err ) {
    *file_out = NULL;
    return err;
  }

  // We opened this file, so file_out owns it.
  // On error , file_out is NULL, so deleting it is harmless.
  return qio_file_init_usr(file_out, file_info, iohints | QIO_HINT_OWNED, flags, style, fs_info, s);
}

// If buf is NULL, we create a new buffer. flags indicates readable/writeable/seekable.
// (default fdflags should be QIO_FDFLAG_READABLE|QIO_FDFLAG_WRITEABLE|QIO_FDFLAG_SEEKABLE
qioerr qio_file_open_mem_ext(qio_file_t** file_out, qbuffer_t* buf, qio_fdflag_t fdflags, qio_hint_t iohints, const qio_style_t* style)
{
  qio_file_t* file = NULL;
  qioerr err;

  file = (qio_file_t*) qio_calloc(sizeof(qio_file_t), 1);
  if( ! file ) {
    return QIO_ENOMEM;
  }

  if( !buf ) {
    // create with ref count = 1
    err = qbuffer_create(&file->buf);
    if( err ) goto error;
  } else {
    // retain a copy.
    file->buf = buf;
    qbuffer_retain(file->buf);
  }

  DO_INIT_REFCNT(file); // initialized to 1.
  file->fp = NULL;
  file->fd = -1;
  file->fdflags = fdflags;
  file->hints = choose_io_method(file, iohints, 0, qbuffer_len(file->buf),
                                 (fdflags & QIO_FDFLAG_READABLE) > 0,
                                 (fdflags & QIO_FDFLAG_WRITEABLE) > 0,
                                 0);
  // force method to be QIO_METHOD_MEMORY.
  // force type to be buffered.
  // leave old hints, but for a memory channel we're almost
  // certainly going to ignore them all.
  file->hints = (file->hints & QIO_HINTMASK) | QIO_METHOD_MEMORY | QIO_CH_BUFFERED;

  file->initial_length = qbuffer_end_offset(file->buf);
  file->initial_pos = qbuffer_start_offset(file->buf);
  file->mmap = NULL;
  
  if( style ) qio_style_copy(&file->style, style);
  else qio_style_init_default(&file->style);

  err = qio_lock_init(&file->lock);
  if( err ) goto error;

  file->max_initial_position = -1;

  *file_out = file;
  return 0;

error:
  qbuffer_release(file->buf);
  // With a nonzero return code, the file structure is uninitialized
  // (so the ref count field is meaningless).
  qio_free(file);

  return err;
}

qioerr qio_file_open_mem(qio_file_t** file_out, qbuffer_t* buf, const qio_style_t* style)
{
  return qio_file_open_mem_ext(file_out, buf, (qio_fdflag_t)(QIO_FDFLAG_READABLE|QIO_FDFLAG_WRITEABLE|QIO_FDFLAG_SEEKABLE), 0, style);
}

qioerr qio_file_open_access(qio_file_t** file_out, const char* pathname, const char* access, qio_hint_t iohints, const qio_style_t* style)
{
  qioerr err = 0;
  int flags = 0;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP |  S_IWGRP |  S_IROTH  |  S_IWOTH;

  err = open_flags_for_string(access, &flags);
  if( err ) return err;

  return qio_file_open(file_out, pathname, flags, mode, iohints, style);
}

qioerr qio_file_open_access_usr(qio_file_t** file_out, const char* pathname, const char* access, qio_hint_t iohints, const qio_style_t* style, void* fs_info, const qio_file_functions_t* s)
{
  qioerr err = 0;
  int flags = 0;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP |  S_IWGRP |  S_IROTH  |  S_IWOTH;

  err = open_flags_for_string(access, &flags);
  if( err ) return err;

  return qio_file_open_usr(file_out, pathname, flags, mode, iohints, style, fs_info, s);
}

qioerr qio_file_open_tmp(qio_file_t** file_out, qio_hint_t iohints, const qio_style_t* style)
{
  /*char* tmp;
  char* fname = NULL;
  char* prefix[] = {P_tmpdir, "/tmp", NULL};
  size_t plen;
  int i;
  fd_t fd = -1;
  qioerr err = EINVAL;

  for( i = 0; prefix[i]; i++ ) {
    plen = strlen(prefix[i]);
    tmp = realloc(fname, plen + 1 + 6 + 1); // room for /,XXXXXX,\0
    if( !tmp ) {
      qio_free(fname);
      return QIO_ENOMEM;
    }
    fname = tmp;
    sprintf(fname, "%s/XXXXXX", prefix[i]);
    err = sys_mkstemp(fname, &fd);
    if( err == 0 ) {
      // We have a temp file!
      break;
    }
  }

  if( fd != -1 ) {
    unlink(fname);
    err = qio_file_init(file_out, NULL, fd, iohints);
  }

  qio_free(fname);

  return err;*/

  FILE* fp = NULL;
  qioerr err;

  fp = tmpfile();

  if( ! fp ) {
    return qio_mkerror_errno();
  }

  // The caller is responsible for freeing the returned file.
  // On error , file_out is NULL, so deleting it is harmless.
  err = qio_file_init(file_out, fp, -1, iohints | QIO_HINT_OWNED, style, 0);
  return err;
}

// string_out must be freed by the caller.
qioerr qio_file_path_for_fd(fd_t fd, const char** string_out)
{
#if defined(__linux__) || defined(__CYGWIN__)
  char pathbuf[500];
  qioerr err;
  const char* result;
  sprintf(pathbuf, "/proc/self/fd/%i", fd);
  err = qio_int_to_err(sys_readlink(pathbuf, &result));
  // result is owned by sys_readlink, so has to be copied.
  *string_out = qio_strdup(result);
  return err;
#else
#ifdef __APPLE__
  char pathbuf[4*MAXPATHLEN];
  qioerr err;
  int rc;
  err = qio_int_to_err(sys_fcntl_ptr(fd, F_GETPATH, pathbuf, &rc));
  if( !err ) {
    // pathbuf now contains *a* path to the open file descriptor
    *string_out = qio_strdup(pathbuf);
    return 0;
  } else {
    *string_out = NULL;
    return err;
  }
#else 
  *string_out = qio_strdup("unknown");
  return 0;
#endif
#endif
}

qioerr qio_file_path_for_fp(FILE* fp, const char** string_out)
{
  if( !fp ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "null file pointer");
  return qio_file_path_for_fd(fileno(fp), string_out);
}

// string_out must be freed by the caller.
qioerr qio_file_path(qio_file_t* f, const char** string_out)
{
  if (f->fd != -1)
    return qio_file_path_for_fd(f->fd, string_out);
  else {
    if (f->fsfns){ 
      if(f->fsfns->getpath) {
        return f->fsfns->getpath(f->file_info, string_out, f->fs_info);
      } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing getpath");
    } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");
  }
}

qioerr qio_file_length(qio_file_t* f, int64_t *len_out)
{
  struct stat stats;
  qioerr err;

  // Lock necessary for MEMORY buffers
  // but not necessary for file descriptors
  err = qio_lock(& f->lock);
  if( err ) return err;

  if( f->buf ) {
    err = 0;
    *len_out = qbuffer_len(f->buf);
  } else if (f->fd != -1) {
    stats.st_size = 0;
    err = qio_int_to_err(sys_fstat(f->fd, &stats));
    *len_out = stats.st_size;
  } else if(f->fsfns) {
    if (f->fsfns->filelength){
      err = f->fsfns->filelength(f->file_info, len_out, f->fs_info);
    } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing filelength");
  } else QIO_RETURN_CONSTANT_ERROR(ENOSYS, "no fd or plugin");

  qio_unlock(& f->lock);

  return err;
}

/* CHANNELS ----------------------------- */
static
qioerr _qio_channel_init(qio_channel_t* ch, qio_chtype_t type)
{
  memset(ch, 0, sizeof(qio_channel_t));

  // for emphasis...
  ch->cached_cur = NULL;
  ch->cached_end = NULL;
  ch->cached_end_bits = NULL;
  ch->cached_start = NULL;
  ch->cached_start_pos = 0;

  return qio_lock_init(& ch->lock);
}

static
qioerr _qio_channel_init_file_internal(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  qioerr err;
  qioerr newerr;

  qio_hint_t use_hints;
  //qio_method_t method;
  qio_chtype_t type;
  qio_style_t* use_style;

  // check file access matches up with this request.
  if( readable && ! (file->fdflags & QIO_FDFLAG_READABLE) ) {
    QIO_RETURN_CONSTANT_ERROR(EBADF, "not readable");
  }

  if( writeable && ! (file->fdflags & QIO_FDFLAG_WRITEABLE) ) {
    QIO_RETURN_CONSTANT_ERROR(EBADF, "not writeable");
  }

  // now normalize it!
  use_hints = choose_io_method(file, hints, file->hints,
                               file->initial_length, readable, writeable,
                               file->fp != NULL && file->use_fp);
  //method = use_hints & QIO_METHODMASK;
  type = (qio_chtype_t) (use_hints & QIO_CHTYPEMASK);

  err = _qio_channel_init(ch, type);
  if( err ) return err;

  ch->hints = use_hints;
  ch->flags = file->fdflags;
  if( ! readable ) {
    // channel is not readable... 
    ch->flags = (qio_fdflag_t) (ch->flags & ~QIO_FDFLAG_READABLE);
  }
  if( ! writeable ) {
    // channel is not writeable... 
    ch->flags = (qio_fdflag_t) (ch->flags & ~QIO_FDFLAG_WRITEABLE);
  }

  qio_file_retain(file);
  ch->file = file;

  ch->start_pos = start;
  ch->end_pos = end;
  // update the file with start_pos.
  err = 0;
  newerr = qio_lock(&ch->file->lock);
  if( !err ) err = newerr;
  if( ch->start_pos > ch->file->max_initial_position ) {
    ch->file->max_initial_position = ch->start_pos;
  }

  use_style = style;
  if( ! use_style ) use_style = &ch->file->style;

  qio_style_copy(&ch->style, use_style);

  qio_unlock(&ch->file->lock);

  return err;
}

static
qioerr _qio_channel_makebuffer_unlocked(qio_channel_t* ch)
{
  qioerr err;
  int64_t start = ch->mark_stack[0];
  void* expect_end = NULL;

  // protect against channel position beyond end of file.
  if( start > ch->end_pos ) start = ch->end_pos;

  // If the buffer is not initialized, we have to create it.
  err = qbuffer_init(&ch->buf);
  if( err ) {
    return err; // we know in this case qbuffer_is_initialized is still false..
  }

  qbuffer_reposition(&ch->buf, start);

  // Further matters... if the file has been MMAP'd,
  // put the mmap'd region into our buffer.
  err = 0;

  if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_MMAP &&
      (ch->hints & QIO_CHTYPEMASK) == QIO_CH_BUFFERED ) {
    int64_t uselen;

    if( ch->file->mmap ) {
      uselen = ch->file->mmap->len - start;
      if( ch->end_pos < INT64_MAX ) {
        if( start + uselen > ch->end_pos ) {
          uselen = ch->end_pos - start;
        }
      }
      if( uselen > 0 ) {
        // Put the mmap data into the buffer.
        err = qbuffer_append(&ch->buf, ch->file->mmap, start, uselen);
      }
      expect_end = VOID_PTR_ADD(ch->file->mmap->data, uselen + start);
    }
  }

  // We should only have cached_cur/end set if it's an mmap'd file,
  // and in that case we should know cached_end is the end of the
  // file. If that's the case, we should be all set just by
  // adding to the qbuffer the file data.
  // We do not check cached_start since we might have moved
  // the mark_stack along with changing cached_start
  if ( ch->cached_end != expect_end ) {
    chpl_internal_error("_qio_channel_makebuffer_unlocked() failed");
  }

  ch->mark_stack[0] = qbuffer_start_offset(&ch->buf);

  return err;
}

static inline
qioerr _qio_channel_needbuffer_unlocked(qio_channel_t* ch)
{
  if( qbuffer_is_initialized(&ch->buf) ) {
    return 0;
  } else {
    return _qio_channel_makebuffer_unlocked(ch);
  }
}

qioerr _qio_channel_init_file(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  qioerr err;
  size_t i;

  err = _qio_channel_init_file_internal(ch, file, hints, readable, writeable, start, end, style);
  if( err ) return err;

  start += file->initial_pos;
  end += file->initial_pos;

  ch->bit_buffer = 0;
  ch->bit_buffer_bits = 0;

  ch->mark_cur = 0;
  ch->mark_stack_size = MARK_INITIAL_STACK_SZ;
  ch->mark_stack = ch->mark_space;
  for( i = 0; i < MARK_INITIAL_STACK_SZ; i++ ) ch->mark_space[i] = -1;

  qbuffer_init_uninitialized(&ch->buf);

  ch->mark_stack[0] = start;
  ch->av_end = start;

  if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_MMAP &&
      (ch->hints & QIO_CHTYPEMASK) == QIO_CH_BUFFERED ) {
    int64_t uselen;

    if( file->mmap ) {
      uselen = file->mmap->len - start;
      if( ch->end_pos < INT64_MAX ) {
        if( start + uselen > ch->end_pos ) {
          uselen = ch->end_pos - start;
        }
      }
      if( uselen > 0 ) {
        // Put the mmap data into cached.
        // (not possible to free the file until the channel is released,
        //  so this is OK, even without reference count increasing the
        //  bytes in the file...).
        ch->cached_cur = VOID_PTR_ADD(file->mmap->data, start);
        ch->cached_end = VOID_PTR_ADD(ch->cached_cur, uselen);
        ch->cached_start = ch->cached_cur;
        ch->cached_start_pos = start;
        ch->av_end = start + uselen;
      }
    }
  }

  //_qio_buffered_setup_cached(ch);

  return 0;
}

qioerr qio_channel_create(qio_channel_t** ch_out, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  qio_channel_t* ret;
  qioerr err = 0;

  ret = (qio_channel_t*) qio_calloc(1, sizeof(qio_channel_t));
  if( !ret ) return QIO_ENOMEM;


  err = _qio_channel_init_file(ret, file, hints, readable, writeable, start, end, style);
  if( err ) {
    qio_free(ret);
    return err;
  } else {
    DO_INIT_REFCNT(ret);
    if( DEBUG_QIO ) printf("Creating channel %p\n", ret);
    *ch_out = ret;
    return 0;
  }
}

// This routine always returns a malloc'd string in the path_out pointer.
// The caller must free the passed-back pointer.
qioerr qio_relative_path(const char** path_out, const char* cwd, const char* path)
{
  ssize_t i,j;
  ssize_t cwd_len;
  ssize_t last_common_slash = 0;
  ssize_t later_slashes = 0;
  ssize_t after_len = 0;
  char* tmp = NULL;

  // Not an absolute path.
  if( path[0] != '/' ) {
    *path_out = qio_strdup(path);
    if( ! *path_out ) return QIO_ENOMEM;
    return 0;
  }

  // Make sure cwd does not end with a / unless it == /
  cwd_len = strlen(cwd);
  if( cwd_len > 0 && cwd[cwd_len-1] == '/' ) cwd_len--;

  // Find the common prefix of the two strings.
  for( i = 0; i < cwd_len && path[i]; i++ ) {
    if( cwd[i] == '/' && path[i] == '/' ) last_common_slash = i;
    if( cwd[i] != path[i] ) break;
  }

  // We might've gotten to the end of the string...
  // but there's a character here that should possibly
  // count as the last common slash.
  if( (cwd[i] == '\0' || cwd[i] == '/') &&
      (path[i] == '\0' || path[i] == '/') ) {
    last_common_slash = i;
  }

  if( i == cwd_len && last_common_slash == i ) {
    // cwd is a prefix or equal to path
    // just trim cwd off of path.
    if( path[i] == '/' ) i++;
    *path_out = qio_strdup( &path[i] );
    if( ! *path_out ) return QIO_ENOMEM;
    return 0;
  }

  // Otherwise, we have to add some '../'es
  // How many /'s are there in cwd after last_common_slash?
  later_slashes = 0;
  for( i = last_common_slash + 1; i < cwd_len; i++ ) {
    if( cwd[i] == '/' ) later_slashes++;
  }

  // Always consider cwd to end in a /
  later_slashes++;

  // OK, we need to add that many ../ to our relative path.
  after_len = strlen(&path[last_common_slash + 1]);
  tmp = (char*) qio_malloc(3*later_slashes+after_len+1);
  if( ! tmp ) {
    return QIO_ENOMEM;
  }

  j = 0;
  // First, put the ../
  for( i = 0; i < later_slashes; i++ ) {
    tmp[j++] = '.';
    tmp[j++] = '.';
    tmp[j++] = '/';
  }
  // Now, copy the path after last_common_slash, including trailing '\0'
  qio_memcpy(&tmp[j], &path[last_common_slash + 1], after_len+1);

  *path_out = tmp;
  return 0;
}

/* Try to find the shortest way to write the absolute path in path_in
 * relative to the current working directory.
 */
qioerr qio_shortest_path(qio_file_t* file, const char** path_out, const char* path_in)
{
  const char* cwd = NULL;
  const char* relpath = NULL;
  qioerr err;

  // TODO: Ensure that cwd is a malloc'd string.
  if (file->fsfns && file->fsfns->getcwd) {
    err = file->fsfns->getcwd(file, &cwd, file->fs_info);
  } else {
    err = qio_int_to_err(sys_getcwd(&cwd));
  }

  if( err ) return err;

  err = qio_relative_path(&relpath, cwd, path_in);

  //printf("cwd %s abs %s rel %s\n", cwd, path_in, relpath);

  qio_free((void*) cwd); cwd = NULL;

  if( ! err ) {
    // Use relpath or path_in, whichever is shorter.
    if( strlen(relpath) < strlen(path_in) ) {
      *path_out = relpath; relpath = NULL;
    } else {
      // Not returning relpath, so free it.
      qio_free((void*) relpath); relpath = NULL;
      *path_out = qio_strdup(path_in);
      if( ! *path_out ) err = QIO_ENOMEM;
    }
  }

  return err;
}

qioerr qio_channel_path_offset(const int threadsafe, qio_channel_t* ch, const char** string_out, int64_t* offset_out)
{
  qioerr err;
  const char* tmp = NULL;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *offset_out = -1;
      *string_out = NULL;
      return err;
    }
  }

  *offset_out = qio_channel_offset_unlocked(ch);

  err = qio_file_path(ch->file, &tmp);
  if( !err ) {
    err = qio_shortest_path(ch->file, string_out, tmp);
  }

  qio_free((void*) tmp);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr _qio_channel_final_flush_unlocked(qio_channel_t* ch)
{
  qioerr err = 0;
  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);
  qio_chtype_t type = (qio_chtype_t) (ch->hints & QIO_CHTYPEMASK);
  struct stat stats;

  if( type == QIO_CHTYPE_CLOSED ) return 0;
  if( ! ch->file ) return 0;

  err = _qio_channel_flush_unlocked(ch);
  if( ! err ) {
    // If we have a buffered writing MMAP channel, we need to truncate
    // the file under the right circumstances. See the comment
    // next to the declaration of qio_file->max_initial_position.
    if( (method == QIO_METHOD_MMAP || method == QIO_METHOD_MEMORY) &&
        (ch->flags & QIO_FDFLAG_WRITEABLE) ) {
      err = qio_lock(&ch->file->lock);
      if( !err ) {
        int64_t max_space_made = ch->av_end;
        int64_t max_written = ch->mark_stack[0];
        // Protect against advance past end of the channel.
        if( max_written > ch->end_pos ) max_written = ch->end_pos;

        if( method == QIO_METHOD_MMAP ) {
          stats.st_size = 0;
          // We got the lock. Update the space in the file.
          err = qio_int_to_err(sys_fstat(ch->file->fd, &stats));

          if( !err &&
              stats.st_size == max_space_made &&
              ch->file->max_initial_position == ch->start_pos ) {
            // Truncate the file.
            // NOTE -- this does not work with Cygwin
            // because (presuambly) Windows does not like to
            // truncate a file while it is still mapped.
            err = qio_int_to_err(sys_ftruncate(ch->file->fd, max_written ));
          }
        } else {
          // MEMORY
          // trim the file's buffer so that it's only max_written
          // long.
          int64_t cur_end = qbuffer_end_offset(ch->file->buf);
          if( cur_end > max_written ) {
            qbuffer_trim_back( ch->file->buf, cur_end - max_written );
          }
        }

        qio_unlock(&ch->file->lock);
      }
    }
  }

  // set end_pos to the current position.
  ch->end_pos = qio_channel_offset_unlocked(ch);

  if( qbuffer_is_initialized(&ch->buf) ) {
    // Destroy the buffer.
    err = qbuffer_destroy(&ch->buf);
  }

  ch->hints |= QIO_CHTYPE_CLOSED; // set to invalid type so funcs return EINVAL

  qio_file_release(ch->file);
  ch->file = NULL;

  return err;
}

void _qio_channel_destroy(qio_channel_t* ch)
{
  qioerr err;

  if( DEBUG_QIO ) printf("Destroying channel %p\n", ch);

  err = _qio_channel_final_flush_unlocked(ch);
  if( err ) {
    fprintf(stderr, "qio_channel_final_flush returned fatal error %i", qio_err_to_int(err));
    assert( !err );
    abort();
  }

  qio_lock_destroy(&ch->lock);

  qio_file_release(ch->file);
  ch->file = NULL;

  DO_DESTROY_REFCNT(ch);

  qio_free(ch);
}

static inline
qbuffer_iter_t _av_end_iter(qio_channel_t* ch)
{
  qbuffer_iter_t ret;
  ret = qbuffer_end(&ch->buf);
  // advance a negative amount.
  qbuffer_iter_advance(&ch->buf, &ret, ch->av_end - ret.offset);
  return ret;
}

static inline
qbuffer_iter_t _av_start_iter(qio_channel_t* ch)
{
  qbuffer_iter_t ret;
  ret = qbuffer_begin(&ch->buf);
  // advance a positive amount.
  qbuffer_iter_advance(&ch->buf, &ret, ch->mark_stack[0] - ret.offset);
  return ret;
}

static inline
int64_t _right_mark_start(qio_channel_t* ch)
{
  return ch->mark_stack[ch->mark_cur];
}

static inline
void _add_right_mark_start(qio_channel_t* ch, int64_t amt)
{
  ch->mark_stack[ch->mark_cur] += amt;
}

static inline
void _set_right_mark_start(qio_channel_t* ch, int64_t pos)
{
  ch->mark_stack[ch->mark_cur] = pos;
}

static
qbuffer_iter_t _right_mark_start_iter(qio_channel_t* ch)
{
  int64_t right_mark_start;
  qbuffer_iter_t ret;

  right_mark_start = _right_mark_start(ch);

  /*
  ret = qbuffer_begin(&ch->buf);
  // advance a positive amount.
  qbuffer_iter_advance(&ch->buf, &ret, right_mark_start - ret.offset);
  */
  ret = qbuffer_iter_at(&ch->buf, right_mark_start);
  return ret;
}


// allocate >= amt buffer space, and put it into the
// buffer, but don't advance any iterators.
static
qioerr _buffered_allocate_bufferspace(qio_channel_t* ch, int64_t amt, int64_t max_amt)
{
  int64_t left = amt;
  int64_t max_left = max_amt;
  int64_t uselen;
  qbytes_t* tmp;
  qioerr err;

  // allocate some space!
  while( left > 0 ) {
    err = qbytes_create_iobuf(&tmp);
    if( err ) goto error;
    uselen = tmp->len;
    if( uselen > max_left ) uselen = max_left;
    err = qbuffer_append(&ch->buf, tmp, 0, uselen);
    // qbuffer_append retains tmp, so we can release our local reference.
    // If there was an error, then it is not retained anywhere, so it is
    // reclaimed here.
    qbytes_release(tmp);

    if( err ) goto error;

    left -= uselen;
    max_left -= uselen;
  }

  err = 0;
error:
  return err;
}

// allocates space and advances the end iterator.
static
qioerr _buffered_makespace_atleast(qio_channel_t* ch, int64_t amt)
{
  qioerr err;
  int64_t max_amt;
  int return_eof = 0;

  // do not exceed end_pos.
  max_amt = INT64_MAX;
  if( ch->end_pos < INT64_MAX ) {
    max_amt = ch->end_pos - ch->av_end;
    if( max_amt < 0 ) max_amt = 0; // handle channel position beyond end
  }

  if( amt > max_amt ) {
    amt = max_amt;
    return_eof = 1;
  }

  err = _buffered_allocate_bufferspace(ch, amt, max_amt);
  if( err ) return err;

  ch->av_end = qbuffer_end_offset(&ch->buf);

  if( return_eof ) return QIO_EEOF;
  else return 0;
}

static
qioerr _buffered_get_memory_file_lock_held(qio_channel_t* ch, int64_t amt, int writing)
{
  qioerr err;
  int eof = 0;
  int64_t start, end;
  qbuffer_iter_t fstart;
  qbuffer_iter_t fend;


  if( ch->av_end != qbuffer_end_offset(&ch->buf) ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "internal error");
  }


  start = qbuffer_end_offset(&ch->buf);
  end = start + amt;

  // do not exceed end_pos.
  if( end > ch->end_pos ) {
    end = ch->end_pos;
  }

  // Do we need to add to the buffer, if we're
  // writing?
  if( writing ) {
    if( end > qbuffer_end_offset(ch->file->buf) ) {
      // Try extending the last part in the buffer
      // if it can be extended.
      qbuffer_extend_back(ch->file->buf);
    }

    while( end > qbuffer_end_offset(ch->file->buf) ) {
      // add an IO chunk.
      qbytes_t* iobuf = NULL;
      err = qbytes_create_iobuf(&iobuf);
      if( err ) goto error;
      err = qbuffer_append(ch->file->buf, iobuf, 0, qbytes_len(iobuf));
      // qbuffer_append retains iobuf, so we can release our local reference.
      // If there was an error, then this releases the buffer entirely.
      qbytes_release(iobuf);

      if( err ) goto error;
    }
  } else {
    // When reading, do not go past end of buffer.
    if( end > qbuffer_end_offset(ch->file->buf) ) {
      end = qbuffer_end_offset(ch->file->buf);
    }
  }

  // Set fstart and fend to the position of start/end in the
  // file's buffer.
  fstart = qbuffer_iter_at(ch->file->buf, start);
  fend = fstart;
  qbuffer_iter_advance(ch->file->buf, &fend, end - start);

  // round 'end' forward to the end of a chunk,
  // so that we only read entire chunks into our buffer.
  qbuffer_iter_ceil_part(ch->file->buf, &fend);

  // Do not allow writing past end_pos.
  if( fend.offset > ch->end_pos ) {
    fend = fstart;
    qbuffer_iter_advance(ch->file->buf, &fend, end - start);
  }

  // If we're not going to get all the requested data, return EEOF.
  if( qbuffer_iter_num_bytes(fstart, fend) < amt ) eof = 1;

  // Now append the bytes objects in the file's buffer
  // from fstart to fend to our channel buffer.
  // This just copies pointers and increases reference counts.
  err = qbuffer_append_buffer(&ch->buf, ch->file->buf, fstart, fend);
  if( err ) goto error;

  // Update av_end to be pointing to the end of the buffer.
  ch->av_end += qbuffer_iter_num_bytes(fstart, fend);

  // OK!
  err = 0;
  // make sure to return EEOF if we ran out of room.
  if( eof ) err = QIO_EEOF;

error:
  return err;
}


static
qioerr _buffered_get_memory(qio_channel_t* ch, int64_t amt, int writing)
{
  qioerr err;

  err = _qio_channel_needbuffer_unlocked(ch);
  if( err ) return err;
  
  // lock the file's buffer, which protects
  // access to file->buf.
  err = qio_lock(&ch->file->lock);
  if( err ) return err;

  err = _buffered_get_memory_file_lock_held(ch, amt, writing);

  qio_unlock(&ch->file->lock);
  return err;
}

static
qioerr _buffered_get_mmap(qio_channel_t* ch, int64_t amt_in, int writing)
{
  qbuffer_iter_t start;
  qioerr err;
  size_t mmap_chunk = qio_mmap_chunk_iobufs * qbytes_iobuf_size;
  struct stat stats;
  int prot;
  void* data;
  int64_t len;
  int64_t skip;
  qbytes_t* bytes;
  int eof = 0;
  long pagesize = 0;
  int64_t pages_in;
  int64_t map_start;
  int64_t amt = amt_in;

  err = _qio_channel_needbuffer_unlocked(ch);
  if( err ) return err;

  if( ch->av_end != qbuffer_end_offset(&ch->buf) ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "internal error");
  }

  // round start down to page size.
  pagesize = sys_page_size();

  start = qbuffer_end(&ch->buf);

  pages_in = start.offset / pagesize;
  map_start = pages_in * pagesize;
  skip = start.offset - map_start;

  amt += skip;

  // Round amt up to mmap_chunk size, store in len.
  // We'll map len bytes starting at map_start.
  {
    int64_t nchunks = amt / mmap_chunk;
    int64_t rounded_down = nchunks * mmap_chunk;

    // never mmap 0 bytes..
    if( rounded_down == 0 ) rounded_down = mmap_chunk;

    if( rounded_down < amt ) len = rounded_down + mmap_chunk;
    else len = rounded_down;
  }

  // Seek to the end of the file.
  // Get the file's length.
  err = qio_int_to_err(sys_fstat(ch->file->fd, &stats));
  if( err ) return err;

  // do not exceed end_pos.
  if( ch->end_pos < INT64_MAX ) {
    if( map_start + len > ch->end_pos ) {
      len = ch->end_pos - map_start;
    }
  }

  if( map_start + len > stats.st_size ) {
    if( writing ) {
      // Extend the file by writing a 0 at the
      // last position of the mapping. Note that there is no
      // race condition problem with multiple writers when we
      // do it this way (vs ftruncate) since if somebody else
      // extends the file past this position after we called fstat,
      // we won't mess up their mmap.
      
     
      // Make sure that we have actual disk space for the mapping.
      // Note that posix_fallocate returns an error code.
#ifdef __linux__
      err = qio_int_to_err(sys_posix_fallocate(ch->file->fd, map_start, len));
      if( err ) return err;
#else
      {
        uint8_t zero = 0;
        ssize_t wrote = 0;
        // This code could be used on a system without posix_fallocate
        err = qio_int_to_err(sys_pwrite(ch->file->fd, &zero, 1, map_start + len - 1, &wrote));
        if( err ) return err;
      }
#endif
    } else {
      // Don't mmap all that -- just mmap to how much we're reading.
      len = stats.st_size - map_start;
    }
  }

  // If we're not going to get all the requested data, return EEOF.
  if( len < amt ) eof = 1;

  if( len > 0 ) {
    // OK, now mmap
    prot = PROT_READ;
    if( ch->flags & QIO_FDFLAG_WRITEABLE ) prot |= PROT_WRITE;

    if( len > SSIZE_MAX ) QIO_RETURN_CONSTANT_ERROR(EOVERFLOW, "overflow in mmap");

    err = qio_int_to_err(sys_mmap(NULL, len, prot, MAP_SHARED, ch->file->fd, map_start, &data));
    if( err ) return err;

    err = qbytes_create_generic(&bytes, data, len, qbytes_free_munmap);
    if( err ) {
      sys_munmap(data, len);
      return err;
    }

    err = qbuffer_append(&ch->buf, bytes, skip, len - skip);
    // qbuffer_append retains ch->buf, so we can release our local reference.
    // If there was an error, then we release bytes entirely.
    qbytes_release(bytes); // munmaps on error, decs ref count normally.
    ch->av_end = qbuffer_end_offset(&ch->buf);

    if( err ) return err;
  }

  if( eof ) return QIO_EEOF;
  else return 0;
}

// Runs read or pread, whichever is appropriate,
// to read into the buffer.
static
qioerr _buffered_read_atleast(qio_channel_t* ch, int64_t amt)
{
  qbuffer_iter_t read_start;
  qbuffer_iter_t read_end;
  ssize_t num_read;
  int64_t left = amt;
  int64_t max_amt;
  int return_eof = 0;
  qioerr err;
  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);

  err = _qio_channel_needbuffer_unlocked(ch);
  if( err ) return err;

  // do not exceed end_pos.
  max_amt = INT64_MAX;
  if( ch->end_pos < INT64_MAX ) {
    max_amt = ch->end_pos - ch->av_end;
    if( max_amt < 0 ) max_amt = 0; // handle channel position beyond end
  }

  if( amt > max_amt ) {
    amt = max_amt;
    return_eof = 1;
  }

  //printf("Allocating bufferspace %lli\n", (long long int) amt);
  err = _buffered_allocate_bufferspace(ch, amt, max_amt);
  if( err ) return err;

  read_start = _av_end_iter(ch);

  left = amt;
  while(left > 0) {
    read_end = read_start;
    qbuffer_iter_advance(&ch->buf, &read_end, left);

    // Round up to whole chunk.
    qbuffer_iter_ceil_part(&ch->buf, &read_end);

    QIO_GET_CONSTANT_ERROR(err, EINVAL, "read method not implemented");
    num_read = 0;
    switch (method) {
      case QIO_METHOD_READWRITE:
        err = qio_readv(ch->file, &ch->buf, read_start, read_end, &num_read);
        break;
      case QIO_METHOD_PREADPWRITE:
        err = qio_preadv(ch->file, &ch->buf, read_start, read_end, read_start.offset, &num_read);
        break;
      case QIO_METHOD_FREADFWRITE:
        err = qio_freadv(ch->file->fp, &ch->buf, read_start, read_end, &num_read);
        break;
      case QIO_METHOD_MMAP:
      case QIO_METHOD_MEMORY:
        // should've been handled outside this method!
        QIO_GET_CONSTANT_ERROR(err, EINVAL, "internal error");
        break;
      // no default to get warnings when new methods are added
    }

    left -= num_read;
    qbuffer_iter_advance(&ch->buf, &read_start, num_read);

    if( err ) break;
  }

  ch->av_end = read_start.offset;

  if( err ) return err;

  if( return_eof ) return QIO_EEOF;
  else return 0;
}

static
qioerr _qio_flush_bits_if_needed_unlocked(qio_channel_t* restrict ch)
{
  qioerr err = 0;
  int keep_bytes = 0;
  qio_bitbuffer_t part_one_bits_be;
  int writing = ch->flags & QIO_FDFLAG_WRITEABLE;
  int flush;


  if( writing ) flush = ch->bit_buffer_bits > 0;
  else flush = ch->bits_read_bytes > 0;

  if( flush ) {

    //printf("IN FLUSH BITS bit_buffer_bits=%i read_bytes=%i\n", ch->bit_buffer_bits, ch->bits_read_bytes);

    if( writing ) {
      //printf("FLUSHING TO BYTE %i\n", ch->bit_buffer_bits);
      keep_bytes = (ch->bit_buffer_bits + 7)/8;

      // Pad out the end of bit_buffer with zeros.
      err = qio_channel_write_bits(false, ch, 0, 8*sizeof(qio_bitbuffer_t) - ch->bit_buffer_bits);

      part_one_bits_be = qio_bitbuffer_tobe(ch->bit_buffer); // big endian now.
    } else {
      keep_bytes = (7 + 8*ch->bits_read_bytes - ch->bit_buffer_bits) / 8;
    }
  }

  if( ch->cached_end_bits ) {
    ch->cached_end = ch->cached_end_bits;
    ch->cached_end_bits = NULL;
  }

  ch->bit_buffer = 0;
  ch->bit_buffer_bits = 0;
  ch->bits_read_bytes = 0;

  if( flush ) {
    if( writing ) {
      //printf("IN FLUSH BITS WRITING %i BYTES %llx\n", keep_bytes, (long long int) qio_bitbuffer_unbe(part_one_bits_be) );
      // Now write the top keep_bytes of ch->bit_buffer.
      err = qio_channel_write_amt(false, ch, &part_one_bits_be, keep_bytes);
    } else {
      // advance past any bits we've already read..
      err = qio_channel_read_amt(false, ch, &part_one_bits_be, keep_bytes);
    }
  }

  return err;
}


/* This function updates our buffer ch->buf with the
 * current position from the fast-path pointer ch->cached_cur
 */
void _qio_buffered_advance_cached(qio_channel_t* ch)
{
  qioerr err;
  int64_t cur_pos_cached;
  int64_t cur_pos_buf;

  // flush any bits stored up.
  // Note that we only store up bits for
  // more than a byte if we've got a buffered
  // channel and cached->cur has room for
  // 2 8-byte words. So we can only lose
  // up to a byte from this particular
  // call failing.
  err = _qio_flush_bits_if_needed_unlocked(ch);

  // The cached data is from 
  //   ch->cached_start_pos to 
  //   ch->cached_start_pos + (ch->cached_end - ch->cached_start)
  // The current position, according to the cached data, is
  //   ch->cached_start_pos + (ch->cached_cur - ch->cached_start)

  if( ch->cached_end ) {
    cur_pos_cached = ch->cached_start_pos +
                       VOID_PTR_DIFF(ch->cached_cur, ch->cached_start);
    cur_pos_buf = _right_mark_start(ch);

    // We cannot go backwards with cached...
    assert(cur_pos_cached >= cur_pos_buf);

    //printf("advance cached %li %li\n", (long int) cur_pos_cached, (long int) cur_pos_buf);
    _add_right_mark_start(ch, cur_pos_cached - cur_pos_buf);

    // Clear out where we are.. If we're doing this
    // before a read or a write, we'll recompute it in a jiffy.
    ch->cached_cur = NULL;
    ch->cached_end = NULL;
  }

  _qio_channel_set_error_unlocked(ch, err);
}

void _qio_buffered_setup_cached(qio_channel_t* ch)
{
  int64_t av_bytes;

  ch->cached_cur = NULL;
  ch->cached_end = NULL;
  ch->cached_start = NULL;

  // disable the fast path if the right hint is set.
  if( ch->hints & QIO_HINT_NOFAST ) return;

  // Put the available data in read_cur and read_end.
  av_bytes = ch->av_end - _right_mark_start(ch);
  if( av_bytes > 0 ) {
    qbuffer_iter_t start;
    qbuffer_iter_t end;
    qbytes_t* bytes;
    int64_t skip;
    int64_t len;

    start = _right_mark_start_iter(ch);
    end = _av_end_iter(ch);
    qbuffer_iter_get(start, end, &bytes, &skip, &len);

    if( len > av_bytes ) len = av_bytes;

    ch->cached_cur = VOID_PTR_ADD(bytes->data, skip);
    ch->cached_start = ch->cached_cur;
    ch->cached_start_pos = start.offset;
    ch->cached_end = VOID_PTR_ADD(bytes->data, skip + len);
    //printf("setup has len=%li\n", (long int) len);
  }
  //printf("setup cached start %lx %p %p\n", (long) ch->cached_start_pos, ch->cached_cur, ch->cached_end);
}


// Writes chunks that are complete. If flushall is set,
// also writes an incomplete portion of a chunk.
//
// Calls qio_buffered_setup_cached if there were no errors
//
// This function returns an error code, but in some situations
// the error code is not checked. So it needs to return the
// same error code again (or succeed) when the action
// is tried again. This is mainly an issue for writing.
static
qioerr _qio_buffered_behind(qio_channel_t* ch, int flushall)
{
  // write any chunks before av_start
  // pop them from the buffer
  // recompute the iterators.
  qbuffer_iter_t start;
  qbuffer_iter_t write_start;
  qbuffer_iter_t write_end;
  qioerr err;
  ssize_t num_written;
  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);

  // If we are a FILE* type buffer, we want to automatically
  // flush after every write, so that C I/O can be intermixed
  // with QIO calls. This is (obviously) not the most perfomant way to do
  // it, but we expect this to be used with stdout/stderr mostly,
  // where timely updating (e.g. line-buffering) is more important
  // than total speed.
  if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_FREADFWRITE ) {
    flushall = 1;
  }

  write_start = qbuffer_begin(&ch->buf);
  write_end = _av_start_iter(ch);

  if( !flushall ) {
    // Move write_end back to the start of the chunk
    // we're working on.
    qbuffer_iter_floor_part(&ch->buf, &write_end);
  }

  // If there's nothing to write, just return. We don't even need
  // to update the iterators. This is the common case.
  if( qbuffer_iter_num_bytes(write_start, write_end) == 0 ) {
    err = 0;
    goto done;
  }

  //fprintf(stderr, "starting write\n");
  //debug_print_qbuffer(&ch->buf);

  if(ch->hints & QIO_HINT_DIRECT) {
    // Round out the data to write to the end of the current chunk.
    qbuffer_iter_ceil_part(&ch->buf, &write_end);
  }

  if(ch->flags & QIO_FDFLAG_WRITEABLE) {
    while( qbuffer_iter_num_bytes(write_start, write_end) > 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "write method not implemented");
      num_written = 0;
      switch (method) {
        case QIO_METHOD_READWRITE:
          err = qio_writev(ch->file, &ch->buf, write_start, write_end, &num_written);
          break;
        case QIO_METHOD_PREADPWRITE:
          err = qio_pwritev(ch->file, &ch->buf, write_start, write_end, write_start.offset, &num_written);
          break;
        case QIO_METHOD_FREADFWRITE:
          err = qio_fwritev(ch->file->fp, &ch->buf, write_start, write_end, &num_written);
          break;
        case QIO_METHOD_MMAP:
        case QIO_METHOD_MEMORY:
          // do nothing; mmap already puts data.
          // We could call msync here if we were feeling really pushy.
          err = 0;
          num_written = qbuffer_iter_num_bytes(write_start, write_end);
          break;
        // no default to get warnings when new methods are added
      }
      if( err ) goto error;
      qbuffer_iter_advance(&ch->buf, &write_start, num_written);
    }
  } else {
    // just pretend like we wrote it; in fact we just deallocate
    // the buffer space below.
    write_start = write_end;
  }

  err = 0;

error:
  //fprintf(stderr, "before trim\n");
  //debug_print_qbuffer(&ch->buf);

  start = qbuffer_begin(&ch->buf);
  // Now remove parts we wrote. This might invalidate iterators!
  qbuffer_trim_front(&ch->buf, qbuffer_iter_num_bytes(start, write_start));

  //fprintf(stderr, "after trim\n");
  //debug_print_qbuffer(&ch->buf);

done:
  if( !err ) {
    _qio_buffered_setup_cached(ch);
  }

  return err;
}

qioerr _qio_channel_require_unlocked(qio_channel_t* ch, int64_t amt, int writing)
{
  qioerr err;

  int64_t n_available;
  int64_t n_needed;

  err = _qio_channel_needbuffer_unlocked(ch);
  if( err ) return err;

  // Include whatever data we got in cached_cur/cached_end
  _qio_buffered_advance_cached(ch);

  n_available = ch->av_end - _right_mark_start(ch);
  // Is the data already available?
  if( amt <= n_available ) {
    // Great! Don't do anything.
    return 0;
  }

  // End < position means we are at EOF.
  if(n_available < 0) return QIO_EEOF;

  // Otherwise, we need some data.
  n_needed = amt - n_available;

  if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_MMAP ) {
    err = _buffered_get_mmap(ch, n_needed, writing);
  } else if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_MEMORY ) {
    err = _buffered_get_memory(ch, n_needed, writing);
  } else {
    if( ch->flags & QIO_FDFLAG_READABLE ) {
      // we're reading the data. So read some data!
      err = _buffered_read_atleast(ch, n_needed);
    } else {
      // Just writing. Add some space to the end of the channel.
      err = _buffered_makespace_atleast(ch, n_needed);
    }
  }
  return err;
}

static
qioerr _qio_buffered_read(qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* amt_read)
{
  qbuffer_iter_t start;
  qbuffer_iter_t end;
  int64_t gotlen = 0;
  qioerr err;
  int eof;

  // handle channel position beyond end.
  if( _right_mark_start(ch) > ch->end_pos ) return QIO_EEOF;

  // do the actual read. (require calls advance_cached)
  err = _qio_channel_require_unlocked(ch, len, 0);
  eof = 0;
  if( qio_err_to_int(err) == EEOF ) eof = 1;
  else if( err ) goto error;

  // figure out the end of the data to copy
  gotlen = ch->av_end - _right_mark_start(ch);
  start = _right_mark_start_iter(ch);
  if( len < gotlen ) {
    gotlen = len;
    end = start;
    qbuffer_iter_advance(&ch->buf, &end, len);
  } else {
    end = _av_end_iter(ch);
  }

  // Now copy out the data.
  err = qbuffer_copyout(&ch->buf, start, end, ptr, gotlen);
  if( err ) goto error;

  // now advance the start of the available buffer by the amount.
  _set_right_mark_start(ch, end.offset);

  // did we get to a different part? if so, we can release some
  // buffers.
  err = _qio_buffered_behind(ch, false);
  if( err ) goto error;

  err = 0;
  if( eof ) err = QIO_EEOF;

error:
  *amt_read = gotlen;
  return err;
}

/*
qioerr qio_buffered_read_until(qio_channel_t* ch, qbytes_t** ret, recognizer_t recognize, void* state)
{
  assert(0);
}
*/

static
qioerr _qio_buffered_write(qio_channel_t* ch, const void* ptr, ssize_t len, ssize_t *amt_written)
{
  qbuffer_iter_t start;
  qbuffer_iter_t end;
  int64_t gotlen = 0;
  qioerr err;
  int eof;

  // handle channel position beyond end.
  if( _right_mark_start(ch) > ch->end_pos ) return QIO_EEOF;

  // make sure we have buffer space. (require calls advance_cached)
  err = _qio_channel_require_unlocked(ch, len, 1);
  eof = 0;
  if( qio_err_to_int(err) == EEOF ) eof = 1;
  else if( err ) goto error;

  // figure out the end of the data to copy
  start = _right_mark_start_iter(ch);
  end = start;
  gotlen = qbuffer_iter_num_bytes_after(&ch->buf, end);
  if( len < gotlen ) gotlen = len;
  qbuffer_iter_advance(&ch->buf, &end, gotlen);

  // now copy the data in to the buffer.
  err = qbuffer_copyin(&ch->buf, start, end, ptr, gotlen);
  if( err ) goto error;

  // now move start forward.
  _set_right_mark_start(ch, end.offset);

  // did we get to a different part? If so, call write()
  err = _qio_buffered_behind(ch, false);
  if( err ) goto error;

  err = 0;
  if( eof ) err = QIO_EEOF;
error:
  *amt_written = gotlen;
  return err;
}

static
qioerr _qio_unbuffered_write(qio_channel_t* ch, const void* ptr, ssize_t len_in, ssize_t *amt_written)
{
  ssize_t num_written;
  size_t num_written_u;
  ssize_t len;
  qioerr err;
  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);
  int return_eof = 0;

  // handle channel position beyond end.
  if( _right_mark_start(ch) > ch->end_pos ) return QIO_EEOF;

  // do not exceed end_pos.
  if( ch->end_pos < INT64_MAX ) {
    if( _right_mark_start(ch) + len_in > ch->end_pos ) {
      len_in = ch->end_pos - _right_mark_start(ch);
      return_eof = 1;
    }
  }
  len = len_in;

  if( method == QIO_METHOD_MMAP &&
      ch->file->mmap && _right_mark_start(ch) + len <= ch->file->mmap->len) {
    // Copy the data to the mmap.
    qio_memcpy( VOID_PTR_ADD(ch->file->mmap->data,_right_mark_start(ch)), ptr, len);
    _add_right_mark_start(ch, len);
  } else {
    while( len > 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "write method not implemented");
      num_written = 0;
      switch (method) {
        case QIO_METHOD_READWRITE:
          err = qio_int_to_err(sys_write(ch->file->fd, ptr, len, &num_written));
          break;
        case QIO_METHOD_MMAP: // mmap uses pread/pwrite when we're 
                              // outside the mmap'd region.
        case QIO_METHOD_PREADPWRITE:
          err = qio_int_to_err(sys_pwrite(ch->file->fd, ptr, len, _right_mark_start(ch), &num_written));
          break;
        case QIO_METHOD_FREADFWRITE:
          if( ch->file->fp ) {
            num_written_u = fwrite(ptr, 1, len, ch->file->fp);
            err = 0;
            if( num_written_u == 0 ) {
              err = qio_int_to_err(ferror(ch->file->fp));
            }
            num_written = num_written_u;
          } else {
            QIO_GET_CONSTANT_ERROR(err, EINVAL, "missing file pointer");
            num_written = 0;
          }
          break;
        case QIO_METHOD_MEMORY:
          /* This could be supported, but for
           * now, only buffered memory-only channels will work.
           */
          /* what we would need to do:
          // lock the file lock
          //
          // make sure space is available in the file's buffer,
          // growing the buffer if necessary
          //
          // copy the data in to the file's buffer
          //
          // update something so we can remove extra space
          // from the end of the buffer when we're all done.
          //
          // unlock the file lock
          */
          QIO_GET_CONSTANT_ERROR(err, EINVAL, "not supported for memory file");
          break;
        // no default to get warnings when new methods are added
      }
      if( err ) {
        *amt_written = num_written + len_in - len;
        return err;
      }
      ptr = VOID_PTR_ADD(ptr, num_written);
      len -= num_written;
      _add_right_mark_start(ch, num_written);
    }
  }

  *amt_written = len_in;
  if( return_eof ) return QIO_EEOF;
  else return 0;
}

static
qioerr _qio_unbuffered_read(qio_channel_t* ch, void* ptr, ssize_t len_in, ssize_t *amt_read)
{
  ssize_t num_read;
  size_t num_read_u;
  ssize_t len;
  qioerr err;
  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);
  int return_eof = 0;

  // handle channel position beyond end.
  if( _right_mark_start(ch) > ch->end_pos ) return QIO_EEOF;

  // do not exceed end_pos.
  if( ch->end_pos < INT64_MAX ) {
    if( _right_mark_start(ch) + len_in > ch->end_pos ) {
      len_in = ch->end_pos - _right_mark_start(ch);
      return_eof = 1;
    }
  }
  len = len_in;

  if( ch->file->mmap &&
      (method == QIO_METHOD_PREADPWRITE || method == QIO_METHOD_MMAP) &&
      _right_mark_start(ch) + len <= ch->file->mmap->len) {
    // As long as we're using an I/O method that seeks on every read,
    // copy the data out of the mmap.
    qio_memcpy( ptr, VOID_PTR_ADD(ch->file->mmap->data,_right_mark_start(ch)), len);
    _add_right_mark_start(ch, len);
  } else {
    while( len > 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "read method not implemented");
      num_read = 0;
      switch (method) {
        case QIO_METHOD_READWRITE:
          err = qio_int_to_err(sys_read(ch->file->fd, ptr, len, &num_read));
          break;
        case QIO_METHOD_MMAP:
        case QIO_METHOD_PREADPWRITE:
          err = qio_int_to_err(sys_pread(ch->file->fd, ptr, len, _right_mark_start(ch), &num_read));
          break;
        case QIO_METHOD_FREADFWRITE:
          if( ch->file->fp ) {
            num_read_u = fread(ptr, 1, len, ch->file->fp);
            err = 0;
            if( num_read_u == 0 ) {
              if( feof(ch->file->fp) ) err = QIO_EEOF;
              else err = qio_int_to_err(ferror(ch->file->fp));
            }
            num_read = num_read_u;
          } else {
            QIO_GET_CONSTANT_ERROR(err, EINVAL, "missing file pointer");
            num_read = 0;
          }
          break;
        case QIO_METHOD_MEMORY:
          /* This could be supported, but for
           * now, only buffered memory-only channels will work.
           */
          /* what we would have to do:
          // lock the file lock
          //
          // make sure space is available in the file's buffer
          //
          // copy the data from the file's buffer
          qbuffer_copyout();
          //
          // unlock the file lock
          */
          QIO_GET_CONSTANT_ERROR(err, EINVAL, "not supported for memory file");
          break;
        // no default to get warnings when new methods are added
      }
      // Return early on an error or on EOF.
      if( err ) {
        *amt_read = num_read + len_in - len;
        return err;
      }
      ptr = VOID_PTR_ADD(ptr, num_read);
      len -= num_read;
      _add_right_mark_start(ch, num_read);
    }
  }

  *amt_read = len_in;
  if( return_eof ) return QIO_EEOF;
  else return 0;
}

qioerr _qio_channel_flush_qio_unlocked(qio_channel_t* ch)
{
  qioerr err, saved_err;
  err_t errcode;

  err = 0;

  // if writing, write anything in the bits...
  err = _qio_flush_bits_if_needed_unlocked(ch);

  // Handle flushing any buffers.

  // If we've got no buffer, we don't have anything to
  // write-behind. Note, though, that if we decide to call
  // msync on mmap writes, the cached mmap data in cached_cur
  // would need to get msync'd too when the qbuffer is not
  // initialized.
  if( qbuffer_is_initialized(&ch->buf) ) {
    _qio_buffered_advance_cached(ch);
    err = _qio_buffered_behind(ch, true);
    if( err ) return err;
  }

  // If there was an error saved earlier, report it now.
  // We don't report EILSEQ, EEOF, or EFORMAT on a flush.
  saved_err = qio_channel_error(ch);
  errcode = qio_err_to_int(saved_err);
  if( !err &&
      !(errcode == EILSEQ || errcode == EEOF || errcode == EFORMAT) ) {
    err = saved_err;
  }
  return err;
}

qioerr _qio_channel_flush_unlocked(qio_channel_t* ch)
{
  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);
  qioerr err;

  err = _qio_channel_flush_qio_unlocked(ch);

  // Also flush cstdio buffer if we're using fread/fwrite
  // and we're writing.
  if( method == QIO_METHOD_FREADFWRITE &&
      (ch->flags & QIO_FDFLAG_WRITEABLE) ) {
    int got = fflush(ch->file->fp);
    if( got && err == 0 ) err = qio_mkerror_errno();
  }

  return err;
}

static inline
int _use_buffered(qio_channel_t* ch, ssize_t len)
{
  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);
  qio_chtype_t type = (qio_chtype_t) (ch->hints & QIO_CHTYPEMASK);
  int64_t offset = qio_channel_offset_unlocked(ch);

  if( type == QIO_CH_ALWAYS_UNBUFFERED ) return 0;
  // There are cases in which a buffer is required
  else if( type == QIO_CH_ALWAYS_BUFFERED ) return 1;
  else if (qbuffer_is_initialized(&ch->buf)) return 1;
  else if (ch->cached_cur) return 1;
  else if (ch->mark_cur > 0) return 1;
  else if (method == QIO_METHOD_MEMORY) return 1;
  // Do not bother initializing the buffer if we are going
  // to read outside of the channel's region.
  else if (offset == ch->end_pos) return 0; 
  else if (offset > ch->start_pos ||
           offset + len < ch->end_pos) return 1;
  else return 0;
}

/* _qio_slow_write does the I/O passed itself, and also
 * sets ch->write_cur and ch->write_end appropriately (if possible)
 * so that future calls will go through that fast path.
 *
 * This function writes whatever is in the buffer up to ch->write_cur
 * and then writes the data in ptr, len.
 *
 * The lock must be held (if we're using the lock) before this is called.
 */
qioerr _qio_slow_write(qio_channel_t* ch, const void* ptr, ssize_t len, ssize_t* amt_written)
{
  *amt_written = 0;

  if( ! (ch->flags & QIO_FDFLAG_WRITEABLE ) ) {
    QIO_RETURN_CONSTANT_ERROR(EBADF, "not writeable");
  }

  if( _use_buffered(ch, len) ) {
    return _qio_buffered_write(ch, ptr, len, amt_written);
  } else {
    return _qio_unbuffered_write(ch, ptr, len, amt_written);
  }
}

qioerr _qio_slow_read(qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* amt_read)
{
  qioerr ret;

  *amt_read = 0;

  if( ! (ch->flags & QIO_FDFLAG_READABLE ) ) {
    QIO_RETURN_CONSTANT_ERROR(EBADF, "not readable");
  }

  ret = 0;

  if( _use_buffered(ch, len) ) {
    ret = _qio_buffered_read(ch, ptr, len, amt_read);
  } else {
    ret = _qio_unbuffered_read(ch, ptr, len, amt_read);
  }

  if( qio_err_to_int(ret) == EEOF ) {
    // Update the channel end position if we got EOF
    // (this makes the EOF sticky).
    ch->end_pos = ch->av_end; 
  }

  return ret;
}

// Only returns locking errors (ie when threadsafe=true).
qioerr qio_channel_offset(const int threadsafe, qio_channel_t* ch, int64_t* offset_out)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *offset_out = -1;
      return err;
    }
  }

  *offset_out = qio_channel_offset_unlocked(ch);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return 0;
}

qioerr qio_channel_end_offset(const int threadsafe, qio_channel_t* ch, int64_t* offset_out)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *offset_out = -1;
      return err;
    }
  }

  *offset_out = qio_channel_end_offset_unlocked(ch);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return 0;
}

static
qioerr _qio_channel_put_bytes_unlocked(qio_channel_t* ch, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes)
{
  qioerr err;
  int64_t use_len, use_skip, copylen;

  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);

  err = _qio_channel_needbuffer_unlocked(ch);
  if( err ) return err;

  // advance past any data put in cached_cur
  _qio_buffered_advance_cached(ch);

  // check that we don't write past the channel's region.
  {
    int64_t start, end;
    start = _right_mark_start(ch);
    end = start + len_bytes;
    if( end > ch->end_pos ) {
      end = ch->end_pos;
    }
    use_len = end - start;
    use_skip = skip_bytes;
  }

  // Now, if we're using MMAP, we need to copy the data
  // to the mmap'd region, extending it if necessary.
  // In general, this probably doesn't make sense (because
  // the system will read the data before writing it).
  // However, we default to pwrite when writing, and besides
  // that, when appending, the system will avoid the read
  // (since the file will have just had space allocated and
  // so the filesystem can know it's all zeros anyway).
  //
  // Meanwhile, if we're using MEMORY, and we
  // have some blocks in our buffer, we want to copy
  // from bytes to the blocks.
  //
  // Once we run out of blocks, we'll:
  //   for MEMORY, append to the file's buffer and channel's buffer
  //   for others, append to the channel's buffer

  // If we're using MEMORY, lock the file
  if( method == QIO_METHOD_MEMORY ) {
    err = qio_lock(&ch->file->lock);
    if( err ) return err;
  }

  // Adjust our channel buffer
  if( method == QIO_METHOD_MMAP ) {
    // extend the file and our buffer.
    err = _buffered_get_mmap(ch, use_len, true);
    if( err ) goto error; // can't have EOF because we're writing.
  } else if( method == QIO_METHOD_MEMORY ) {
    // get any bytes from the channel but without adding
    err = _buffered_get_memory_file_lock_held(ch, use_len, false);
    if( qio_err_to_int(err) == EEOF ) err = 0; // ignore EOF, we'll just append in a moment.
    if( err ) goto error;
  } else {
    // Remove everything after write-mark from the channel buffer
    // so we can zero-copy the bytes in here.
    ch->av_end = _right_mark_start(ch);
    qbuffer_trim_back(& ch->buf, qbuffer_end_offset(&ch->buf) - ch->av_end);
  }

  if( ch->av_end != qbuffer_end_offset(&ch->buf) ) {
   QIO_GET_CONSTANT_ERROR(err, EINVAL, "internal error");
   goto error;
  }

  // Now, copy data from bytes in as long as there's
  // buffer area to copy in to.
  copylen = ch->av_end - _right_mark_start( ch );
  if( copylen > 0 ) {
    err = qbuffer_copyin(& ch->buf,
                         _right_mark_start_iter( ch ),
                         qbuffer_end(&ch->buf),
                         VOID_PTR_ADD(bytes->data, use_skip), copylen);
    if( err ) goto error;

    use_skip += copylen;
    use_len -= copylen;

    // Update the channel position.
    _set_right_mark_start(ch, _right_mark_start(ch) + copylen);
  }

  // Now, append bytes until there's no more appending to do.
  if( use_len > 0 ) {
    if( method == QIO_METHOD_MMAP )
      QIO_RETURN_CONSTANT_ERROR(EINVAL, "internal error");

    // If we're a MEMORY buffer, append to the file->buf.
    // We take care of locking file above
    if( method == QIO_METHOD_MEMORY ) {
      err = qbuffer_append( ch->file->buf, bytes, use_skip, use_len);
      if( err ) goto error;
    }

    // Append to the channel.
    err = qbuffer_append(& ch->buf, bytes, use_skip, use_len);
    if( err ) goto error;

    // Update the channel position.
    ch->av_end = qbuffer_end_offset(&ch->buf);
    _set_right_mark_start(ch, _right_mark_start(ch) + use_len);
  }

  err = 0;

error:
  if( method == QIO_METHOD_MEMORY ) {
    qio_unlock(&ch->file->lock);
  }

  if( err ) return err;

  // if there was no error...
  // did we get to a different part? If so, call write()
  err = _qio_buffered_behind(ch, false);
  return err;
}

qioerr qio_channel_put_bytes(const int threadsafe, qio_channel_t* ch, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes)
{
  qioerr err;

  if( ! (ch->flags & QIO_FDFLAG_WRITEABLE) ) {
    QIO_RETURN_CONSTANT_ERROR(EBADF, "not writeable");
  }

  if( skip_bytes < 0 || len_bytes < 0 ||
     skip_bytes + len_bytes > bytes->len ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");
  }

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = _qio_channel_put_bytes_unlocked(ch, bytes, skip_bytes, len_bytes);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static
qioerr _qio_channel_put_buffer_unlocked(qio_channel_t* ch, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end)
{
  qioerr err;
  int64_t use_len, copylen;
  qbuffer_iter_t src_copy_end;

  qio_method_t method = (qio_method_t) (ch->hints & QIO_METHODMASK);

  err = _qio_channel_needbuffer_unlocked(ch);
  if( err ) return err;

  // advance past any data put in cached_cur
  _qio_buffered_advance_cached(ch);

  // check that we don't write past the channel's region.
  {
    int64_t start, end;

    start = _right_mark_start(ch);
    end = start + qbuffer_iter_num_bytes(src_start, src_end);
    if( end > ch->end_pos ) {
      end = ch->end_pos;
      src_end = src_start;
      qbuffer_iter_advance(src, &src_end, end - start);
    }
    use_len = qbuffer_iter_num_bytes(src_start, src_end);
  }

  // see comment in _qio_channel_put_bytes_unlocked for more info...

  // If we're using MEMORY, lock the file
  if( method == QIO_METHOD_MEMORY ) {
    err = qio_lock(&ch->file->lock);
    if( err ) return err;
  }

  // Adjust our channel buffer
  if( method == QIO_METHOD_MMAP ) {
    // extend the file and our buffer.
    err = _buffered_get_mmap(ch, use_len, true);
    if( err ) goto error; // can't have EOF because we're writing.
  } else if( method == QIO_METHOD_MEMORY ) {
    // get any bytes from the channel but without adding
    err = _buffered_get_memory_file_lock_held(ch, use_len, false);
    if( qio_err_to_int(err) == EEOF ) err = 0; // ignore EOF, we'll just append in a moment.
    if( err ) goto error;
  } else {
    // Remove everything after write-mark from the channel buffer
    // so we can zero-copy the bytes in here.
    ch->av_end = _right_mark_start(ch);
    qbuffer_trim_back(& ch->buf, qbuffer_end_offset(&ch->buf) - ch->av_end);
  }

  if( ch->av_end != qbuffer_end_offset(&ch->buf) ) {
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "internal error");
    goto error;
  }

  // Now, copy data from src_buffer in as long as there's
  // buffer area to copy in to.
  copylen = ch->av_end - _right_mark_start(ch);
  if( copylen > 0 ) {
    src_copy_end = src_start;
    qbuffer_iter_advance(src, &src_copy_end, copylen);

    err = qbuffer_copyin_buffer(& ch->buf,
                                _right_mark_start_iter(ch),
                                qbuffer_end(&ch->buf),
                                src,
                                src_start,
                                src_copy_end);
    if( err ) goto error;

    qbuffer_iter_advance(src, &src_start, copylen);
    use_len -= copylen;

    // Update the channel position.
    _set_right_mark_start(ch, _right_mark_start(ch) + copylen);
  }

  // Now, append bytes until there's no more appending to do.
  if( use_len > 0 ) {
    if( method == QIO_METHOD_MMAP )
      QIO_RETURN_CONSTANT_ERROR(EINVAL, "internal error");

    // If we're a MEMORY buffer, append to the file->buf.
    // We take care of locking file above
    if( method == QIO_METHOD_MEMORY ) {
      err = qbuffer_append_buffer( ch->file->buf, src, src_start, src_end);
      if( err ) goto error;
    }

    // Append to the channel.
    err = qbuffer_append_buffer(& ch->buf, src, src_start, src_end);
    if( err ) goto error;

    // Update the channel position.
    ch->av_end = qbuffer_end_offset(&ch->buf);
    _set_right_mark_start(ch, _right_mark_start(ch) + use_len);
  }

  err = 0;

error:
  if( method == QIO_METHOD_MEMORY ) {
    qio_unlock(&ch->file->lock);
  }

  if( err ) return err;

  // if there was no error...
  // did we get to a different part? If so, call write()
  err = _qio_buffered_behind(ch, false);
  return err;
}

qioerr qio_channel_put_buffer(const int threadsafe, qio_channel_t* ch, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end)
{
  qioerr err;

  if( ! (ch->flags & QIO_FDFLAG_WRITEABLE) )
    QIO_RETURN_CONSTANT_ERROR(EBADF, "not writeable");

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = _qio_channel_put_buffer_unlocked(ch, src, src_start, src_end);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// you don't have to call end_peek_buffer if this returns an error
qioerr qio_channel_begin_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t require, int writing, qbuffer_t** buf_out, qbuffer_iter_t* start_out, qbuffer_iter_t* end_out)
{
  qioerr err;

  *buf_out = NULL;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  // require calls needbuffer_unlocked and advance_cached.
  err = _qio_channel_require_unlocked(ch, require, writing);
  if( err ) {
    _qio_channel_set_error_unlocked(ch, err);
    qio_unlock(&ch->lock);
    return err;
  }

  *buf_out = &ch->buf;
  *start_out = _right_mark_start_iter(ch);
  *end_out = _av_end_iter(ch);

  err = 0;

  return err;
}


qioerr _qio_buffered_behind(qio_channel_t* ch, int flushall);

qioerr qio_channel_end_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t advance)
{
  qioerr err;

  if( ! qbuffer_is_initialized(&ch->buf) ) {
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "internal error");
    goto error;
  }

  _add_right_mark_start(ch, advance);

  err = _qio_buffered_behind(ch, false);

error:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_mark_maybe_flush_bits(const int threadsafe, qio_channel_t* ch, int flushbits)
{
  qioerr err;
  int64_t pos = -1;
  ssize_t new_size;
  ssize_t i;
  int64_t* new_buf;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }
 
  if( flushbits ) {
    err = _qio_flush_bits_if_needed_unlocked(ch);
    if( err ) goto error;
  }

  // includes the amount we've got in cached in the channel.
  pos = qio_channel_offset_unlocked(ch);
  
  if( ch->mark_cur + 1 >= ch->mark_stack_size ) {
    new_size = 2 * (ch->mark_cur + 1);

    // Reallocate the mark buffer.
    if( ch->mark_stack == ch->mark_space ) {
      // mark_space is the preallocated buffer, so we have to do an explicit
      // copy here.  (Calling qio_realloc would try to free stack-allocated
      // space :-O)
      new_buf = (int64_t*) qio_malloc(new_size*sizeof(int64_t));
      if( ! new_buf ) {
        err = QIO_ENOMEM;
        goto error;
      }
      // Copy the values from our old stack.
      for( i = 0; i <= ch->mark_cur; i++ ) new_buf[i] = ch->mark_stack[i];
    } else {
      new_buf = (int64_t*)qio_realloc(ch->mark_stack, new_size*sizeof(int64_t));
      if( ! new_buf ) {
        err = QIO_ENOMEM;
        goto error;
      }
      // Realloc already copies the values if necessary.
    }
    // Now clear out the new elements.
    for( i = ch->mark_cur + 2; i < new_size; i++ ) {
      new_buf[i] = -1;
    }

    ch->mark_stack = new_buf;
    ch->mark_stack_size = new_size;
  }

  // Set the current mark stack value to the current offset
  // (since it could have advanced with the cached pointers)
  // That way, if we revert to it, we go back to
  // this point.
  ch->mark_stack[ch->mark_cur] = pos;
  // Now create the new position on the mark stack
  // with the same position.
  ch->mark_stack[ch->mark_cur+1] = pos;
  ch->mark_cur++;

  // Make sure that we have a buffer!
  //  (otherwise we couldn't possibly revert)
  err = _qio_channel_needbuffer_unlocked(ch);

error:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_mark(const int threadsafe, qio_channel_t* ch)
{
  return qio_channel_mark_maybe_flush_bits(threadsafe, ch, 1);
}

/* Always advances, may call qio_buffered_behind and
 * then returns an error code. This error code may be ignored
 * because it presumably will come up again in a call
 * to read/write/flush.
 *
 * Returns EEOF if we got to EOF before advancing that many bytes.
 */
qioerr qio_channel_advance_unlocked(qio_channel_t* ch, int64_t nbytes)
{
  int use_buffered;
  qioerr err = 0;

  // clear out any bits.
  ch->bit_buffer = 0;
  ch->bit_buffer_bits = 0;

  if( nbytes < 0 ) nbytes = 0;

  // Fast path: all data is available in the cached area.
  if( nbytes <= VOID_PTR_DIFF(ch->cached_end, ch->cached_cur) ) {
    ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, nbytes);
    return 0;
  }

  // Slow path: not all data is available in the cached area.
  use_buffered = _use_buffered(ch, nbytes);
  if( use_buffered ) {
    err = _qio_channel_needbuffer_unlocked(ch);
    if( err ) return err;

    _qio_buffered_advance_cached(ch);
  }

  _add_right_mark_start(ch, nbytes);

  // If qio_buffered_behind fails, it will presumably
  // fail again on flush/close. So it is OK to
  // ignore the error code.
  //
  // _qio_buffered_behind calls _qio_buffered_setup_cached
  if( use_buffered ) {
    err = _qio_buffered_behind(ch, false);
  }
  return err;
}

void qio_channel_revert_unlocked(qio_channel_t* restrict ch)
{
  int64_t target;

  assert(ch->mark_cur >= 1);

  // clear out any bits.
  ch->bit_buffer = 0;
  ch->bit_buffer_bits = 0;

  // seek back to ch->mark_stack[ch->mark_cur].
  target = ch->mark_stack[ch->mark_cur-1];
  
  // Is that within the cached area?
  if( ch->cached_start && target >= ch->cached_start_pos ) {
    // OK, great, just move the cached pointer.
    ch->cached_cur = VOID_PTR_ADD(ch->cached_start, target - ch->cached_start_pos);
    // We should not be past the end!
    assert( VOID_PTR_DIFF(ch->cached_end, ch->cached_cur) >= 0 );

    // OK to change mark stack value because it remains
    // within the currently cached region.
    ch->mark_stack[ch->mark_cur] = -1;
    ch->mark_cur--;
  } else {
    if( target != ch->mark_stack[ch->mark_cur] ) {
      // We need to go further backwards than the cached area
      // so we need to re-compute the cached pointers.
      assert( qbuffer_is_initialized(&ch->buf) );
    }

    // Advance (ie take information from cached and
    // put it into buffer pointers) BEFORE we change
    // the current mark stack value.
    _qio_buffered_advance_cached(ch);

    // Now that cached are NULLed, OK to change mark stack value.
    ch->mark_stack[ch->mark_cur] = -1;
    ch->mark_cur--;

    // Now we just have lots of extra buffer space. No need
    // to call write-behind.

    // Set up the buffered pointers appropriately.
    _qio_buffered_setup_cached(ch);
  }
}

void qio_channel_commit_unlocked(qio_channel_t* ch)
{
  int64_t pos = -1;

  // OK to leave any bits alone.

  assert(ch->mark_cur >= 1);

  pos = ch->mark_stack[ch->mark_cur];
  ch->mark_stack[ch->mark_cur] = -1;
  ch->mark_cur--;
  ch->mark_stack[ch->mark_cur] = pos;

  // We wrote it to our buffer.. in some way
  // the write succeeded. 
  // So we ignore an error code from 
  // post cached write.
  // (that way, functions that return an error
  //  indicate nothing was written)
  _qio_channel_post_cached_write(ch);
}

qioerr qio_channel_advance(const int threadsafe, qio_channel_t* ch, int64_t nbytes)
{
  qioerr err;

  if( nbytes < 0 )
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative count");

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = qio_channel_advance_unlocked(ch, nbytes);
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

/* Handle I/O of bits at a time */
void _qio_channel_write_bits_cached_realign(qio_channel_t* restrict ch, uint64_t v, int8_t nbits)
{
  qio_bitbuffer_t part_one_bits;
  qio_bitbuffer_t part_one_bits_be;
  qio_bitbuffer_t tmp_bits;
  qio_bitbuffer_t mask;
  int tmp_live;
  int part_one;
  int part_two;
  size_t to_copy;

  tmp_live = ch->bit_buffer_bits;
  tmp_bits = ch->bit_buffer;

  // We've got > 64 bits to write.
  part_one = 8*sizeof(qio_bitbuffer_t) - tmp_live;
  part_two = nbits - part_one;
  part_one_bits = (tmp_bits << part_one) | ( v >> part_two );
  part_one_bits_be = qio_bitbuffer_tobe(part_one_bits); // big endian now.
  tmp_bits = v;
  tmp_live = part_two;

  // How many bytes to write?
  to_copy = sizeof(qio_bitbuffer_t) - VOID_PTR_ALIGN(ch->cached_cur, sizeof(qio_bitbuffer_t));
  
  //printf("WRITE BITS REALIGNALIGNED WRITING %llx %i\n", (long long int) part_one_bits, (int) (8*to_copy));
  // memcpy will work because part_one_bits is big endian now.
  qio_memcpy(ch->cached_cur, &part_one_bits_be, to_copy);
  ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, to_copy);

  // Remove junk from the top of tmp_bits, so only bottom tmp_live bits are set.
  if( 0 < tmp_live && tmp_live < (int) (8*sizeof(qio_bitbuffer_t)) ) {
    mask = (qio_bitbuffer_t) -1;
    mask >>= (8*sizeof(qio_bitbuffer_t) - tmp_live);
  } else if( tmp_live == 0 ) {
    // no live bits.
    mask = 0;
  } else {
    // tmp_live == bit size of bitbuffer.
    mask = (qio_bitbuffer_t) -1;
  }
  tmp_bits &= mask;

  // But now what do we need to put into tmp_bits?
  if( tmp_live <= (int) (8*to_copy) ) {
    // We can just put the remaining bits into tmp_bits
    // and that's great.
    // Clear out what we copied.
    part_one_bits <<= 8*to_copy;
    // now relevant bits of part_one_bits are in the hi bits. Move them to bottom.
    part_one_bits >>= 8*to_copy;
    tmp_bits |= (part_one_bits << tmp_live);
    tmp_live = 8*sizeof(qio_bitbuffer_t) - 8*to_copy + tmp_live;
  } else {
    // Otherwise... we still have to write another word.
    part_one_bits <<= 8*to_copy;
    part_one_bits |= tmp_bits >> (tmp_live - 8*to_copy);
    part_one_bits_be = qio_bitbuffer_tobe(part_one_bits); // big endian now.
    // We have 8-byte alignment
    *(qio_bitbuffer_t*)ch->cached_cur = part_one_bits_be;
    ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, sizeof(qio_bitbuffer_t));

    // tmp_bits stays what it is.
    tmp_live = tmp_live - 8*to_copy;
  }

  //printf("WRITE BITS REALIGNALIGNED LEAVING %llx %i\n", (long long int) tmp_bits, tmp_live);
  ch->bit_buffer = tmp_bits;
  ch->bit_buffer_bits = tmp_live;
}

qioerr _qio_channel_write_bits_slow(qio_channel_t* restrict ch, uint64_t v, int8_t nbits)
{
  qioerr err = 0;
  qio_bitbuffer_t part_one, part_two;
  qio_bitbuffer_t parts_be[2];
  qio_bitbuffer_t tmp_bits;
  uint64_t tmpv;
  int tmp_live;
  int tmp_leftshift;
  int part_bits;
  int v_leftshift;
  int v_rightshift;
  int writebytes;

  tmp_live = ch->bit_buffer_bits;
  tmp_bits = ch->bit_buffer;
  if( tmp_live == 0 ) tmp_bits = 0;

  //printf("In write bits slow\n");

  // Write any number of bytes based on what is in ch->bit_buffer
  // and based upon what we're writing in nbits.

  tmp_leftshift = 8*sizeof(qio_bitbuffer_t) - tmp_live;
  part_bits = tmp_live + nbits;
  if( nbits > tmp_leftshift ) {
    // we will need more than one word...
    v_rightshift = nbits - tmp_leftshift;
    tmpv = (v_rightshift < 64) ? v : 0;
    part_one = (tmp_bits << tmp_leftshift) | (tmpv >> v_rightshift);
    tmpv = (v_rightshift > 0) ? v : 0;
    part_two = tmpv << (8*sizeof(qio_bitbuffer_t) - v_rightshift);
  } else {
    // otherwise, we will not spill over..
    v_leftshift = tmp_leftshift - nbits;
    tmpv = (v_leftshift < 64) ? v : 0;
    part_one = (tmp_bits << tmp_leftshift) | (tmpv << v_leftshift);
    part_two = 0;
  }

  // Convert them to big-endian.
  parts_be[0] = qio_bitbuffer_tobe(part_one);
  parts_be[1] = qio_bitbuffer_tobe(part_two);

  writebytes = part_bits / 8; // round down.

  // Now we have parts[0,1] and part_bits.
  // Write out the full bytes.
  
  // avoid flushing bit-buffer in this write
  ch->bit_buffer_bits = 0;
  ch->bits_read_bytes = 0;

  // setup buffer pointers for normal write.
  if( ch->cached_end_bits ) {
    ch->cached_end = ch->cached_end_bits;
    ch->cached_end_bits = NULL;
  }

  //printf("SLOW WRITING %i bytes %llx %llx\n", writebytes, (long long int) part_one, (long long int) part_two);

  if( writebytes > 0 ) {
    err = qio_channel_write_amt(false, ch, parts_be, writebytes);
  }

  // Now put any partial byte back in ch->bit_buffer.

  tmp_live = part_bits - 8*writebytes;

  // Which word is the remainder in?
  if( writebytes < (int) sizeof(qio_bitbuffer_t) ) {
    // remainder in part_one
    // put the byte in question to the hi byte
    tmp_bits = part_one << (8*writebytes);
  } else {
    // put the byte in question to the hi byte
    tmp_bits = part_two << (8*(writebytes-sizeof(qio_bitbuffer_t)));
  }
  // put the byte in question to the lo byte
  tmp_bits = tmp_bits >> (8*sizeof(qio_bitbuffer_t) - 8);
  tmp_bits &= 0xff; // just one byte...
  // set the bits right
  tmp_bits = tmp_bits >> (8 - tmp_live); // move from hi to lo

  if( ! err ) {
    ch->bit_buffer = tmp_bits;
    ch->bit_buffer_bits = tmp_live;

    //printf("AFTER SLOW WRITING %llx %i\n", (long long int) tmp_bits, tmp_live);
    // set up caching for bit-buffer fast writes
    // and cause automatic flush of remaining bits
    // upon a byte-driven write.
    if( ch->cached_end ) {
      ch->cached_end_bits = ch->cached_end;
      ch->cached_end = NULL;
    }
  }

//unlock:
  _qio_channel_set_error_unlocked(ch, err);

  return err;
}

qioerr qio_channel_flush_bits(const int threadsafe, qio_channel_t* restrict ch)
{
  qioerr err = 0;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = _qio_flush_bits_if_needed_unlocked(ch);

//unlock:
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

void _qio_channel_read_bits_cached_realign(qio_channel_t* restrict ch, uint64_t* restrict v, int8_t nbits)
{
  qio_bitbuffer_t tmp_bits;
  qio_bitbuffer_t buf;
  size_t to_copy;
  uint64_t value;
  int vbits;
  int tmp_live;
  int tmp_read;
  int value_part;
  int part_one;
  int part_two;

  tmp_bits = ch->bit_buffer;
  tmp_live = ch->bit_buffer_bits;
  tmp_read = 0;

  assert( tmp_live < nbits ); // shouldn't get here otherwise.

  // first, take any bits we have in tmp_bits and put them into value.
  value = qio_bitbuffer_topn(tmp_bits, tmp_live);
  vbits = tmp_live;

  // Next, read as many bytes as we need to in order to
  // get the rest of the data we have to put in value.
  value_part = nbits - vbits;
  to_copy = (7 + value_part) / 8;

  buf = 0;
  qio_memcpy(&buf, ch->cached_cur, to_copy);
  ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, to_copy);

  // now we've set the top several bytes of buf.
  buf = qio_bitbuffer_unbe(buf);

  // Extract the part that applies to our value.
  value <<= value_part;
  value |= qio_bitbuffer_topn(buf, value_part);

  *v = value;

  // Now what's left in buf (<8bits) needs to go into
  // tmp_bits.
  part_one = 8*to_copy - value_part;
  buf <<= value_part;
  tmp_bits = qio_bitbuffer_topn(buf, part_one); // currently storing at bottom.
  tmp_live = part_one;

  // now we need to read some amount.
  // We've got < 1 byte in tmp currently,
  // and to get to alignment we'll need to read
  // <= 7 bytes, so that will fit into 8 bytes.
  to_copy = sizeof(qio_bitbuffer_t) - VOID_PTR_ALIGN(ch->cached_cur, sizeof(qio_bitbuffer_t));
  if( to_copy == sizeof(qio_bitbuffer_t) ) to_copy = 0;

  buf = 0;
  if( to_copy )
    qio_memcpy(&buf, ch->cached_cur, to_copy);
  tmp_read = to_copy;
  part_two = 8*to_copy;

  // now we've set the top several bytes of buf.
  buf = qio_bitbuffer_unbe(buf);

  // OK, now extract the part of buf that we care about.
  tmp_bits <<= part_two;
  tmp_bits |= qio_bitbuffer_topn(buf, part_two);
  tmp_live += part_two;

  // Now move tmp_bits to the higher order bits
  // like we want for reading.
  tmp_bits <<= (8*sizeof(qio_bitbuffer_t) - tmp_live);

  ch->bit_buffer = tmp_bits;
  ch->bit_buffer_bits = tmp_live;
  ch->bits_read_bytes = tmp_read;
}

qioerr _qio_channel_read_bits_slow(qio_channel_t* restrict ch, uint64_t* restrict v, int8_t nbits)
{
  qio_bitbuffer_t tmp_bits;
  qio_bitbuffer_t buf;
  int tmp_live;
  int tmp_read;
  int part_two;
  uint64_t value;
  qioerr err = 0;

  tmp_bits = ch->bit_buffer;
  tmp_live = ch->bit_buffer_bits;
  tmp_read = ch->bits_read_bytes;

  if( nbits <= tmp_live ) {
    // we're going to get everything we need from tmp_bits.
    *v = tmp_bits >> (8*sizeof(qio_bitbuffer_t) - nbits);
    tmp_bits <<= nbits;
    tmp_live -= nbits;
  } else {
    part_two = nbits - tmp_live;

    // Do not cause flushing of bit-I/O on these reads.
    ch->bits_read_bytes = 0; // avoid flushing.
    ch->bit_buffer_bits = 0;

    // setup buffer pointers for normal read.
    if( ch->cached_end_bits ) {
      ch->cached_end = ch->cached_end_bits;
      ch->cached_end_bits = NULL;
    }

    // skip any bytes we've already processed.
    buf = 0;
    if( tmp_read > 0 ) {
      err = qio_channel_read_amt(false, ch, &buf, tmp_read);
      if( err ) goto error;
    }

    // we're going to have to read something.
    // how many bytes will we read?
    tmp_read = (7 + nbits - tmp_live) / 8; // round up.

    buf = 0;
    err = qio_channel_read_amt(false, ch, &buf, tmp_read);
    if( err ) goto error;
    // move it to host endian.
    buf = qio_bitbuffer_unbe(buf);

    value = qio_bitbuffer_topn(tmp_bits, tmp_live);
    value <<= part_two;

    value |= qio_bitbuffer_topn(buf, part_two);
    tmp_bits = buf << part_two;
    tmp_live = 8*tmp_read - part_two;

    // Now we havn't read ahead any bytes beyond
    // where we'd expect byte-I/O to truncate us
    tmp_read = 0;

    *v = value;
  }


  if( ! err ) {
    // set up caching for bit-buffer fast reads
    // and cause automatic flush of remaining bits
    // upon a byte-driven read.
    if( ch->cached_end ) {
      ch->cached_end_bits = ch->cached_end;
      ch->cached_end = NULL;
    }

    ch->bit_buffer = tmp_bits;
    ch->bit_buffer_bits = tmp_live;
    ch->bits_read_bytes = tmp_read;
  }

error:
  _qio_channel_set_error_unlocked(ch, err);
  
  return err;
}

int64_t qio_channel_style_element(qio_channel_t* ch, int64_t element)
{
  if( element == QIO_STYLE_ELEMENT_STRING ) return ch->style.str_style;
  if( element == QIO_STYLE_ELEMENT_COMPLEX ) return ch->style.complex_style;
  if( element == QIO_STYLE_ELEMENT_ARRAY ) return ch->style.array_style;
  if( element == QIO_STYLE_ELEMENT_AGGREGATE ) return ch->style.aggregate_style;
  if( element == QIO_STYLE_ELEMENT_TUPLE ) return ch->style.tuple_style;
  if( element == QIO_STYLE_ELEMENT_BYTE_ORDER ) return ch->style.byteorder;
# if __BYTE_ORDER == __LITTLE_ENDIAN
  if( element == QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER ) {
    return (ch->style.byteorder == QIO_LITTLE ||
            ch->style.byteorder == QIO_NATIVE) ? 1 : 0;
  }
# else // __BYTE_ORDER == __BIG_ENDIAN
  if( element == QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER ) {
    return (ch->style.byteorder == QIO_BIG ||
            ch->style.byteorder == QIO_NATIVE) ? 1 : 0;
  }
#endif // __BYTE_ORDER
  return 0;
}

qioerr qio_get_fs_type(qio_file_t* fl, int* out)
{
  sys_statfs_t s;
  int rc = 1;

  if (fl->fsfns && fl->fsfns->get_fs_type) {
    *out = fl->fsfns->get_fs_type(fl->file_info, fl->fs_info);
    return 0;
  } 

  // else
  if (fl->fp)
    rc = sys_fstatfs(fileno(fl->fp), &s);
  else if (fl->fd != -1)
    rc = sys_fstatfs(fl->fd, &s);

  // can't stat, and we don't have a foreign FS
  if (rc != 0)
    QIO_RETURN_CONSTANT_ERROR(ENOTSUP, "Unable to find file system type");

  if (s.f_type == LUSTRE_SUPER_MAGIC) {
    *out = FTYPE_LUSTRE;
    return 0;
  }

  // else
  *out = FTYPE_NONE;
  return 0;
}


qioerr qio_get_chunk(qio_file_t* fl, int64_t* len_out)
{
  // In the case where we do not have a Lustre or block type fs, we set the chunk
  // size to be the optimal transfer block size
  qioerr err = 0;
  int fd = 0;
  sys_statfs_t s;

  if (fl->fsfns && fl->fsfns->get_chunk) {
    err = fl->fsfns->get_chunk(fl->file_info, len_out, fl->fs_info);
  } else {
    fd = fl->fd;
    if (fl->fp) fd = fileno(fl->fp);

#ifdef SYS_HAS_LLAPI 
    {
      int ftype = 0;
      // This will be set in the lustre plugin if we have Lustre support available
      err = qio_get_fs_type(fl, &ftype);
      if (ftype == FTYPE_LUSTRE) {
        // lustre FS
        err = qio_int_to_err(sys_lustre_get_stripe_size(fd, len_out));
      } else {
        // non-lustre FS
        err = qio_int_to_err(sys_fstatfs(fd, &s));
        *len_out = s.f_bsize;
      }
    }
#else
    err = qio_int_to_err(sys_fstatfs(fd, &s));
    *len_out = s.f_bsize;
#endif
  }

  return err;
}

qioerr qio_locales_for_region(qio_file_t* fl, off_t start, off_t end, const char*** loc_names_out, int* num_locs_out)
{ 
  qioerr err = 0;
  if (fl->fsfns && fl->fsfns->get_locales_for_region) {
    err = fl->fsfns->get_locales_for_region(fl->file_info, start, end, loc_names_out, num_locs_out, fl->fs_info);
    return err;
  } else {
    *num_locs_out = 0;
    QIO_RETURN_CONSTANT_ERROR(ENOSYS, "Unable to get locale for specified region of file");
  }
}

