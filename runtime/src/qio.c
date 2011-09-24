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

#ifndef SIMPLE_TEST
#include "chplrt.h"
#endif

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
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <assert.h>

#include "qio.h"
#include "qbuffer.h"

ssize_t qio_too_small_for_default_mmap = 16*1024;
ssize_t qio_too_large_for_default_mmap = 64*1024*((size_t)1024*1024);
ssize_t qio_mmap_chunk_iobufs = 1024; // we mmap 1024 iobufs at a time (64M)

/*
#define INIT_LOCK(v) (qthread_syncvar_empty(NULL, v))
#define TAKE_LOCK(v) (qthread_syncvar_writeEF_const(NULL, v, 1))

static inline int do_unlock(syncvar_t* x)
{
  if( qthread_syncvar_status(x) ) {
    return qthread_syncvar_empty(NULL, x);
  } else {
    // nobody has the lock!
    return EPERM;
  }
}
#define RELEASE_LOCK(v) (do_unlock(v))
#define DESTROY_LOCK(v) (TAKE_LOCK(v))
*/

#ifdef _chplrt_H_
err_t qio_lock(qio_lock_t* x) {
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

err_t qio_readv(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_read)
{
  int64_t nread = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  int iov_onstack;
  err_t err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    return EINVAL;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // read into our buffer.
  err = sys_readv(fd, iov, iovcnt, &nread);

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_read = nread;

  DONE_SLOW_SYSCALL;

  return err;
}

err_t qio_writev(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_written)
{
  int64_t nwritten = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  int iov_onstack;
  err_t err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    return EINVAL;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // write from our buffer
  err = sys_writev(fd, iov, iovcnt, &nwritten);

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_written = nwritten;

  DONE_SLOW_SYSCALL;

  return err;
}

err_t qio_preadv(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, int64_t* num_read)
{
  ssize_t nread = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  int iov_onstack;
  err_t err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    return EINVAL;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // read into our buffer.
  err = sys_preadv(fd, iov, iovcnt, seek_to_offset, &nread);

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_read = nread;
  DONE_SLOW_SYSCALL;

  return err;

}

err_t qio_freadv(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_read)
{
  int64_t total_read = 0;
  size_t nread;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i;
  int iov_onstack;
  err_t err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    return EINVAL;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  for( i = 0; i < iovcnt; i++ ) {
    nread = fread(iov[i].iov_base, 1, iov[i].iov_len, fp);
    err = 0;
    if( nread == 0 ) {
      if( feof(fp) ) err = EEOF;
      else err = ferror(fp);
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

err_t qio_fwritev(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_written)
{
  int64_t total_written = 0;
  size_t nwritten;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i;
  int iov_onstack;
  err_t err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    return EINVAL;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  for( i = 0; i < iovcnt; i++ ) {
    nwritten = fwrite(iov[i].iov_base, 1, iov[i].iov_len, fp);
    err = 0;
    if( nwritten == 0 ) {
      err = ferror(fp);
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



err_t qio_pwritev(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, int64_t* num_written)
{
  int64_t nwritten = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  int iov_onstack;
  err_t err;
 
  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    return EINVAL;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  // write from our buffer
  err = sys_pwritev(fd, iov, iovcnt, seek_to_offset, &nwritten);

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_written = nwritten;

  DONE_SLOW_SYSCALL;

  return err;
}

err_t qio_recv(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int flags,
              sys_sockaddr_t* src_addr_out, /* can be NULL */
              void* ancillary_out, socklen_t* ancillary_len_inout, /* can be NULL */
              int64_t* num_recvd_out)
{
  struct msghdr msg;
  int64_t nrecvd = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  int iov_onstack;
  err_t err;

  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    return EINVAL;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  memset(&msg, 0, sizeof(struct msghdr));
  if( src_addr_out ) {
    msg.msg_name = &src_addr_out->addr;
    msg.msg_namelen = src_addr_out->len;
  }
  msg.msg_iov = iov;
  msg.msg_iovlen = num_parts;
  if( ancillary_out && ancillary_len_inout ) {
    msg.msg_control = ancillary_out;
    msg.msg_controllen = *ancillary_len_inout;
  }

  err = sys_recvmsg(sockfd, &msg, flags, &nrecvd);
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

err_t qio_send(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int flags,
              const sys_sockaddr_t* dst_addr, /* can be NULL */
              const void* ancillary, /* can be NULL */
              socklen_t ancillary_len,
              int64_t* num_sent_out)
{
  struct msghdr msg;
  int64_t nsent = 0;
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  int iov_onstack;
  err_t err;

  if( num_bytes < 0 || num_parts < 0 || num_parts > INT_MAX ) {
    err = EINVAL;
    goto error;
  }

  STARTING_SLOW_SYSCALL;

  MAYBE_STACK_ALLOC(num_parts*sizeof(struct iovec), iov, iov_onstack);
  if( ! iov ) {
    err = ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  memset(&msg, 0, sizeof(struct msghdr));
  if( dst_addr ) {
    msg.msg_name = (struct sockaddr*) &dst_addr->addr;
    msg.msg_namelen = dst_addr->len;
  }
  msg.msg_iov = iov;
  msg.msg_iovlen = num_parts;
  if( ancillary ) {
    msg.msg_control = (void*) ancillary;
    msg.msg_controllen = ancillary_len;
  }
  err = sys_sendmsg(sockfd, &msg, flags, &nsent);

error:
  MAYBE_STACK_FREE(iov, iov_onstack);

  *num_sent_out = nsent;

  DONE_SLOW_SYSCALL;

  return err;
}

/*
ssize_t _qio_cookie_read(void* cookie, char* buf, size_t size)
{
  qio_channel_t* ch = (qio_channel_t*) cookie;
  err_t err;
  ssize_t amt_read;

  if( size > SSIZE_MAX ) size = SSIZE_MAX;

  //STARTING_SLOW_SYSCALL;
  amt_read = 0;
  err = qio_channel_read(1, ch, buf, size, &amt_read);
  if( amt_read == 0 && err && err != EEOF ) errno = err;
  //DONE_SLOW_SYSCALL;

  // If we read any, return what we read.
  if( amt_read > 0 ) return amt_read;
  // otherwise, there was maybe an error, or maybe EOF.
  if( err == 0 || err == EEOF ) return 0; // 0 means EOF.
  else return -1;
}


ssize_t _qio_cookie_write(void* cookie, const char* buf, size_t size)
{
  qio_channel_t* ch = (qio_channel_t*) cookie;
  err_t err;
  ssize_t amt_written;

  if( size > SSIZE_MAX ) size = SSIZE_MAX;

  //STARTING_SLOW_SYSCALL;
  amt_written = 0;
  err = qio_channel_write(1, ch, buf, size, &amt_written);
  if( amt_written == 0 && err ) errno = err;
  //DONE_SLOW_SYSCALL;

  // If we wrote any, return what we wrote.
  if( amt_written > 0 ) return amt_written;
  // otherwise, there was maybe an error.
  if( err == 0 ) return 0;
  else return -1;
}

int _qio_cookie_close(void* cookie)
{
  qio_channel_t* ch = (qio_channel_t*) cookie;
  err_t err;

  //STARTING_SLOW_SYSCALL;
  ch->stdio_wrapper = NULL;
  err = _qio_channel_destroy(ch);
  if( err ) errno = err;
  //DONE_SLOW_SYSCALL;
  
  if( err == 0 ) return 0;
  else return -1; // fopencookie says EOF, but that should be -1 too.
}

err_t qio_channel_fopen(const int threadsafe, qio_channel_t* ch, int reading, int writing, FILE** out)
{
  err_t err;
  err_t newerr;
  FILE* fp = NULL;

  err = 0;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  if( ch->stdio_wrapper ) fp = ch->stdio_wrapper;
  else {
#ifdef __GLIBC__
    const char* mode;
    cookie_io_functions_t io_funcs = {_qio_cookie_read,
                                      _qio_cookie_write,
                                      NULL,
                                      _qio_cookie_close};


    if( writing ) mode = "w";
    if( reading ) mode = "r";

    fp = fopencookie(ch, mode, io_funcs);
#else
    fp = funopen(ch, _qio_cookie_read, _qio_cookie_write, NULL, _qio_cookie_close);
#endif

    ch->stdio_wrapper = fp;
  }

  if( threadsafe ) {
    newerr = qio_unlock(&ch->lock);
    if( ! err ) err = newerr;
  }

  *out = fp;
  return err;
}*/

/*
err_t qio_vprintf(const int threadsafe, qio_channel_t* ch,
                  const char* fmt, va_list ap)
{
  err_t err;
  err_t newerr;
  int got;
  FILE* fp;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = qio_channel_fopen(0, ch, 0, 1, &fp);
  if( err ) goto error;

  //STARTING_SLOW_SYSCALL;
  got = vfprintf(fp, fmt, ap);
  if( got < 0 ) err = errno;
  //DONE_SLOW_SYSCALL;

error:
  if( threadsafe ) {
    newerr = qio_unlock(&ch->lock);
    if( ! err ) err = newerr;
  }

  return err;
}

err_t qio_printf(const int threadsafe, qio_channel_t* ch,
                 const char* fmt, ...)
{
  err_t err;
  va_list args;
  va_start( args, fmt );
  err = qio_vprintf(threadsafe, ch, fmt, args);
  va_end( args );
  return err;
}

err_t qio_vscanf(const int threadsafe, qio_channel_t* ch,
                 int* nmatched, const char* fmt, va_list ap)
{
  err_t err;
  err_t newerr;
  int got;
  FILE* fp;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = qio_channel_fopen(0, ch, 1, 0, &fp);
  if( err ) goto error;

  //STARTING_SLOW_SYSCALL;
  got = vfscanf(fp, fmt, ap);
  if( got >= 0 ) {
    *nmatched = got;
  }
  if( got == EOF ) {
    *nmatched = 0;
    if( feof(fp) ) err = EEOF;
    else err = errno;
  }
  //DONE_SLOW_SYSCALL;

error:
  if( threadsafe ) {
    newerr = qio_unlock(&ch->lock);
    if( ! err ) err = newerr;
  }

  return err;
}

err_t qio_scanf(const int threadsafe, qio_channel_t* ch,
                 int* nmatched, const char* fmt, ...)
{
  err_t err;
  va_list args;
  va_start( args, fmt );
  err = qio_vscanf(threadsafe, ch, nmatched, fmt, args);
  va_end( args );
  return err;
}

err_t qio_scanf1(const int threadsafe, qio_channel_t* ch,
                 const char* fmt, void* x)
{
  int nmatched = 0;
  err_t err;
  err = qio_scanf(threadsafe, ch, &nmatched, fmt, x);
  if( err ) return err;
  if( nmatched != 1 ) return EFORMAT;
  return 0;
}
qio_chtype_t qio_choose_io_type(qio_hint_t hints)
{
  qio_chtype_t type = QIO_CH_BUFFERED;
  if( (hints & QIO_HINT_LATENCY) && !(hints & QIO_HINT_BANDWIDTH) ) {
    type = QIO_CH_UNBUFFERED;
  }
  return type;
}
*/

static
qio_hint_t choose_io_method(qio_fdflag_t fdflags, qio_hint_t hints, qio_hint_t default_hints, int64_t file_size, int reading, int writing, int isfilestar)
{
  qio_hint_t method = hints & QIO_METHODMASK;
  qio_chtype_t type = hints & QIO_CHTYPEMASK;
  qio_hint_t ret = hints;
 
  ret &= ~(QIO_METHODMASK|QIO_CHTYPEMASK); // clear method number, channel type

  // 'or' in hints from default_hints.
  ret |= (default_hints & ~(QIO_METHODMASK|QIO_CHTYPEMASK));

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
          if( !writing &&
              qio_too_small_for_default_mmap < file_size &&
              file_size < qio_too_large_for_default_mmap ) {
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

  // Always use fread/fwrite with FILE*
  //if( file->fp ) method = QIO_METHOD_FREADFWRITE;
  // we get FILE* from tmpfile() and want to be able to mmap...

  if( type < QIO_CH_MIN_TYPE || type > QIO_CH_MAX_TYPE ) {
    // bad type number. Use default, or choose one.
    if( default_hints & QIO_CHTYPEMASK ) {
      type = default_hints & QIO_CHTYPEMASK;
    } else {
      type = QIO_CH_BUFFERED;
    }
  } else {
    // type is O.K.
  }

  return ret | method | type;
}

static
err_t qio_fadvise_for_hints(qio_file_t* file)
{
  int64_t len = file->initial_length;
  int advice = 0;
  err_t err;

  if( file->hints & QIO_HINT_DIRECT ) {
    int rc;
    long arg;
    // Set O_DIRECT on our fd.
    err = sys_fcntl(file->fd, F_GETFL, &rc);
    if( err ) return err;

    arg = rc | O_DIRECT;
    err = sys_fcntl_long(file->fd, F_SETFL, arg, &rc);
    if( err ) return err;
  } else {
    if( file->hints & QIO_HINT_RANDOM ) advice |= POSIX_FADV_RANDOM;
    if( file->hints & QIO_HINT_SEQUENTIAL ) advice |= POSIX_FADV_SEQUENTIAL;
    if( file->hints & QIO_HINT_NOREUSE ) advice |= POSIX_FADV_NOREUSE;

    if( advice == 0 ) err = 0; // do nothing.
    else err = sys_posix_fadvise(file->fd, 0, len, advice);

    if( err ) return err;
  }

  // now, if we're using mmap, go ahead and map the file.
  if( (file->hints & QIO_METHODMASK) == QIO_METHOD_MMAP &&
      len > 0 ) {
    void* data;
    int prot = PROT_READ;

    if( file->fdflags & QIO_FDFLAG_WRITEABLE ) prot |= PROT_WRITE;

    if( len > SSIZE_MAX ) return ENOMEM;

    // mmap the initial length of the file.
    err = sys_mmap(NULL, len, prot, MAP_SHARED, file->fd, 0, &data);
    if( err ) return err;

    err = qbytes_create_generic(&file->mmap, data, len, qbytes_free_munmap);
    if( err ) {
      sys_munmap(data, len);
      return err;
    }

  }

  return 0;
}


err_t qio_file_init(qio_file_t** file_out, FILE* fp, fd_t fd, qio_hint_t iohints, qio_style_t* style, int usefilestar)
{
  off_t initial_pos = 0;
  off_t initial_length = 0;
  int rc;
  err_t err;
  qio_fdflag_t fdflags = 0;
  qio_file_t* file = NULL;
  struct stat stats;
  off_t seek_ret;

  if( fp ) {
    fd = fileno(fp);
    if( fd == -1 ) return errno; 
  }

  // try to seek.
  err = sys_lseek(fd, 0, SEEK_CUR, &seek_ret);
  if( err == ESPIPE ) {
    // not seekable. Don't worry about it.
    initial_pos = 0;
  } else if( err ) {
    return err;
  } else {
    // seekable.
    fdflags |= QIO_FDFLAG_SEEKABLE;

    initial_pos = seek_ret;

    // get the file length, using stat (not seek)
    // so that this is thread-safe.
    err = sys_fstat(fd, &stats);
    if( err ) return err;

    initial_length = stats.st_size;
  }

  // try to fcntl
  err = sys_fcntl(fd, F_GETFL, &rc);
  if( err ) {
    return err;
  } else {
    rc &= O_ACCMODE;
    if( rc == O_RDONLY ) {
      fdflags |= QIO_FDFLAG_READABLE;
    } else if( rc == O_WRONLY ) {
      fdflags |= QIO_FDFLAG_WRITEABLE;
    } else if( rc == O_RDWR ) {
      fdflags |= QIO_FDFLAG_READABLE;
      fdflags |= QIO_FDFLAG_WRITEABLE;
    }
    // could get other access flags to know if e.g. append only
  }
  
  
  file = qio_calloc(sizeof(qio_file_t), 1);
  if( ! file ) {
    return ENOMEM;
  }

  DO_INIT_REFCNT(file);
  file->fp = fp;
  file->fd = fd;
  file->use_fp = usefilestar;
  file->buf = NULL;
  file->fdflags = fdflags;
  file->initial_length = initial_length;
  file->initial_pos = initial_pos;
  file->hints = choose_io_method(fdflags, iohints, 0, initial_length,
                                 (fdflags & QIO_FDFLAG_READABLE) > 0,
                                 (fdflags & QIO_FDFLAG_WRITEABLE) > 0,
                                 file->fp != 0 && file->use_fp );

  file->mmap = NULL;
  err = qio_lock_init(&file->lock);
  if( err ) goto error;
  file->max_initial_position = -1;

  err = qio_fadvise_for_hints(file);
  if( err ) goto error;

  // put file->hints back to DEFAULT if that's what we started with.
  if( (iohints & QIO_METHODMASK) == QIO_METHOD_DEFAULT ) {
    file->hints &= ~QIO_METHODMASK; // clear the method.
    file->hints |= QIO_METHOD_DEFAULT;
  }

  if( style ) qio_style_copy(&file->style, style);
  else qio_style_init_default(&file->style);

  *file_out = file;
  return 0;

error:
  qio_free(file);
  return err;
}

static
err_t _qio_file_do_close(qio_file_t* f)
{
  err_t err = 0;
  err_t newerr;
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
    rc = fclose(f->fp);
    if( rc ) err = errno;
    f->fp = NULL;
    f->fd = -1;
  }
  
  if( f->fd >= 0 ) {
    err = sys_close(f->fd);
    if( err ) {
      newerr = qio_file_path(f, &path);
      if( newerr ) {
        fprintf(stderr, "Warning: error %i on file close; additionally, error getting name of file\n", err);
      } else {
        fprintf(stderr, "Warning: error %i on file close for file %s\n",
                err, path);
      }
    }
    f->fp = NULL;
    f->fd = -1;
  }

  qio_unlock(& f->lock);

  return err;
}

err_t qio_file_close(qio_file_t* f)
{
  // TODO - we could check to see if
  // there are references to the file
  // but we'd have to do it with gasnet
  // atomics...
  //
  //if( f->ref_cnt > 1 ) return EINVAL;

  return _qio_file_do_close(f);
}

err_t qio_file_sync(qio_file_t* f)
{
  err_t err = 0;
  err_t newerr;

  if( f->fp ) {
    newerr = fflush(f->fp);
    if( ! err ) err = newerr;
    newerr = sys_fsync(fileno(f->fp));
    if( ! err ) err = newerr;
  } else if( f->fd >= 0 ) {
    newerr = sys_fsync(f->fd);
    if( ! err ) err = newerr;
  }

  return err;
}

void _qio_file_destroy(qio_file_t* f)
{
  err_t err;

  if( DEBUG_QIO ) printf("Destroying file %p\n", f);

  err = qio_file_close(f);
  if( err ) {
    printf("Encountered error %i when automatically closing file.\n", err);
    assert( !err );
    abort();
  }

  qio_lock_destroy(&f->lock);

  qio_free(f);
}

// mode should default to S_IRUSR | S_IWUSR | S_IRGRP |  S_IWGRP |  S_IROTH  |  S_IWOTH
// iohints should default to 0
err_t qio_file_open(qio_file_t** file_out, const char* pathname, int flags, mode_t mode, qio_hint_t iohints, qio_style_t* style)
{
  fd_t fd;
  err_t err;

  err = sys_open(pathname, flags, mode, &fd);
  if( err ) {
    *file_out = NULL;
    return err;
  }

  return qio_file_init(file_out, NULL, fd, iohints, style, 0);
}

// If buf is NULL, we create a new buffer. flags indicates readable/writeable/seekable.
// (default fdflags should be QIO_FDFLAG_READABLE|QIO_FDFLAG_WRITEABLE|QIO_FDFLAG_SEEKABLE
err_t qio_file_open_mem_ext(qio_file_t** file_out, qbuffer_t* buf, qio_fdflag_t fdflags, qio_hint_t iohints, qio_style_t* style)
{
  qio_file_t* file = NULL;
  err_t err;

  file = qio_calloc(sizeof(qio_file_t), 1);
  if( ! file ) {
    return ENOMEM;
  }

  if( !buf ) {
    // create with ref count = 1
    err = qbuffer_create(&file->buf);
    if( err ) goto error;
  } else {
    // retain..
    file->buf = buf;
    qbuffer_retain(file->buf);
  }

  DO_INIT_REFCNT(file);
  file->fp = NULL;
  file->fd = -1;
  file->fdflags = fdflags;
  file->hints = choose_io_method(fdflags, iohints, 0, qbuffer_len(file->buf),
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
  
  err = qio_lock_init(&file->lock);
  if( err ) goto error;

  file->max_initial_position = -1;

  *file_out = file;
  return 0;

error:
  qbuffer_release(file->buf);
  qio_free(file);

  return err;
}

err_t qio_file_open_mem(qio_file_t** file_out, qbuffer_t* buf, qio_style_t* style)
{
  return qio_file_open_mem_ext(file_out, buf, QIO_FDFLAG_READABLE|QIO_FDFLAG_WRITEABLE|QIO_FDFLAG_SEEKABLE, 0, style);
}

static
err_t open_flags_for_string(const char* s, int *flags_out)
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
    return EINVAL;
  }
  return 0;
}

err_t qio_file_open_access(qio_file_t** file_out, const char* pathname, const char* access, qio_hint_t iohints, qio_style_t* style)
{
  err_t err = 0;
  int flags = 0;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP |  S_IWGRP |  S_IROTH  |  S_IWOTH;

  err = open_flags_for_string(access, &flags);
  if( err ) return err;

  return qio_file_open(file_out, pathname, flags, mode, iohints, style);
}

err_t qio_file_open_tmp(qio_file_t** file_out, qio_hint_t iohints, qio_style_t* style)
{
  /*char* tmp;
  char* fname = NULL;
  char* prefix[] = {P_tmpdir, "/tmp", NULL};
  size_t plen;
  int i;
  fd_t fd = -1;
  err_t err = EINVAL;

  for( i = 0; prefix[i]; i++ ) {
    plen = strlen(prefix[i]);
    tmp = realloc(fname, plen + 1 + 6 + 1); // room for /,XXXXXX,\0
    if( !tmp ) {
      qio_free(fname);
      return ENOMEM;
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
  err_t err;

  fp = tmpfile();

  if( ! fp ) {
    return errno;
  }

  err = qio_file_init(file_out, fp, -1, iohints, style, 0);
  return err;
}

// string_out must be freed by the caller.
err_t qio_file_path_for_fd(fd_t fd, const char** string_out)
{
#ifdef __linux__
  char pathbuf[500];
  err_t err;
  sprintf(pathbuf, "/proc/self/fd/%i", fd);
  err = sys_readlink(pathbuf, string_out);
  return err;
#else
#ifdef __APPLE__
  char pathbuf[4*MAXPATHLEN];
  err_t err;
  int rc;
  err = sys_fcntl_ptr(fd, F_GETPATH, pathbuf, &rc);
  if( !err ) {
    // pathbuf now contains *a* path to the open file descriptor
    *string_out = qio_strdup(pathbuf);
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

err_t qio_file_path_for_fp(FILE* fp, const char** string_out)
{
  if( !fp ) return EINVAL;
  return qio_file_path_for_fd(fileno(fp), string_out);
}

// string_out must be freed by the caller.
err_t qio_file_path(qio_file_t* f, const char** string_out)
{
  return qio_file_path_for_fd(f->fd, string_out);
}


err_t qio_file_length(qio_file_t* f, int64_t *len_out)
{
  struct stat stats;
  err_t err;

  // Lock necessary for MEMORY buffers
  // but not necessary for file descriptors
  err = qio_lock(& f->lock);
  if( err ) return err;

  if( f->buf ) {
    err = 0;
    *len_out = qbuffer_len(f->buf);
  } else {
    stats.st_size = 0;
    err = sys_fstat(f->fd, &stats);
    *len_out = stats.st_size;
  }

  qio_unlock(& f->lock);

  return err;
}



/* CHANNELS ----------------------------- */
static
err_t _qio_channel_init(qio_channel_t* ch, qio_chtype_t type)
{
  memset(ch, 0, sizeof(qio_channel_t));

  // for emphasis...
  ch->cached_cur = NULL;
  ch->cached_end = NULL;

  return qio_lock_init(& ch->lock);
}

static
err_t _qio_channel_init_file_internal(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  err_t err;
  err_t newerr;

  qio_hint_t use_hints;
  //qio_method_t method;
  qio_chtype_t type;
  qio_style_t* use_style;

  // now normalize it!
  use_hints = choose_io_method(file->fdflags, hints, file->hints,
                               file->initial_length, readable, writeable,
                               file->fp != NULL && file->use_fp);
  //method = use_hints & QIO_METHODMASK;
  type = use_hints & QIO_CHTYPEMASK;

  err = _qio_channel_init(ch, type);
  if( err ) return err;

  ch->hints = use_hints;
  ch->flags = file->fdflags;
  if( ! readable ) {
    // channel is not readable... 
    ch->flags &= ~QIO_FDFLAG_READABLE;
  }
  if( ! writeable ) {
    // channel is not writeable... 
    ch->flags &= ~QIO_FDFLAG_WRITEABLE;
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

err_t _qio_channel_init_unbuffered(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  err_t err;

  err = _qio_channel_init_file_internal(ch, file, (hints & ~QIO_CHTYPEMASK)|QIO_CH_UNBUFFERED, readable, writeable, start, end, style);
  if( err ) return err;

  ch->u.unbuffered.pos = start + file->initial_pos;

  return 0;
}

err_t _qio_channel_init_buffered(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  err_t err;
  qio_buffered_channel_t* heavy = &ch->u.buffered;
  size_t i;

  err = _qio_channel_init_file_internal(ch, file, (hints & ~QIO_CHTYPEMASK)|QIO_CH_BUFFERED, readable, writeable, start, end, style);
  if( err ) return err;

  start += file->initial_pos;
  end += file->initial_pos;

  heavy->mark_next = 0;
  heavy->mark_stack_size = MARK_INITIAL_STACK_SZ;
  heavy->mark_stack = heavy->mark_space;
  for( i = 0; i < MARK_INITIAL_STACK_SZ; i++ ) heavy->mark_space[i] = -1;

  err = qbuffer_init(&heavy->buf);
  if( err ) {
    return err;
  }
  qbuffer_reposition(&heavy->buf, start);

  if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_MMAP ) {
    int64_t uselen;

    if( file->mmap ) {

      uselen = file->mmap->len - start;
      if( uselen > 0 ) {
        // Put the mmap bytes object into the buffer.
        err = qbuffer_append(&heavy->buf, file->mmap, start, uselen);
        if( err ) {
          qbuffer_destroy(&heavy->buf);
          return err;
        }
      }
    }
  }

  heavy->av_start = qbuffer_start_offset(&heavy->buf);
  heavy->av_end = qbuffer_end_offset(&heavy->buf);
  return 0;
}


err_t _qio_channel_init_file(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  qio_chtype_t type = hints & QIO_CHTYPEMASK;
  if( type == QIO_CH_DEFAULT ) type = QIO_CH_BUFFERED;

  switch( type ) {
    case QIO_CH_UNBUFFERED:
      return _qio_channel_init_unbuffered(ch, file, hints, readable, writeable, start, end, style);
    case QIO_CH_BUFFERED:
      return _qio_channel_init_buffered(ch, file, hints, readable, writeable, start, end, style);
    // no default to warn if any added
  }
  return EINVAL;
}

err_t qio_channel_create(qio_channel_t** ch_out, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style)
{
  qio_channel_t* ret;
  err_t err;

  ret = qio_calloc(1, sizeof(qio_channel_t));
  if( !ret ) return ENOMEM;

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

err_t _qio_channel_final_flush_unlocked(qio_channel_t* ch)
{
  err_t err = 0;
  qio_method_t method = ch->hints & QIO_METHODMASK;
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;
  struct stat stats;

  if( type == QIO_CHTYPE_CLOSED ) return 0;
  if( ! ch->file ) return 0;

  err = _qio_channel_flush_unlocked(ch);
  if( err ) {
    return err;
  }

  // If we have a buffered writing MMAP channel, we need to truncate
  // the file under the right circumstances. See the comment
  // next to the declaration of qio_file->max_initial_position.
  if( type == QIO_CH_BUFFERED &&
      (method == QIO_METHOD_MMAP || method == QIO_METHOD_MEMORY) &&
      (ch->flags & QIO_FDFLAG_WRITEABLE) ) {
    err = qio_lock(&ch->file->lock);
    if( !err ) {
      int64_t max_space_made = ch->u.buffered.av_end;
      int64_t max_written = ch->u.buffered.av_start;

      if( method == QIO_METHOD_MMAP ) {
        stats.st_size = 0;
        // We got the lock. Update the space in the file.
        err = sys_fstat(ch->file->fd, &stats);

        if( !err &&
            stats.st_size == max_space_made &&
            ch->file->max_initial_position == ch->start_pos ) {
          // Truncate the file.
          err = sys_ftruncate(ch->file->fd, max_written );
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

    if( err ) {
      return err;
    }
  }

  // set end_pos to the current position.
  ch->end_pos = qio_channel_offset_unlocked(ch);

  // Destroy the buffer.
  err = EINVAL;
  switch (type) {
    case QIO_CH_UNBUFFERED:
      err = 0; // no user buffer, nothing to destroy
      break;
    case QIO_CH_BUFFERED:
      err = qbuffer_destroy(&ch->u.buffered.buf);
      break;
    // No default; hope for useful errors if any added
  }

  if( err ) return err;


  ch->hints |= QIO_CHTYPE_CLOSED; // set to invalid type so funcs return EINVAL

  qio_file_release(ch->file);
  ch->file = NULL;

  return 0;
}

void _qio_channel_destroy(qio_channel_t* ch)
{
  err_t err;

  if( DEBUG_QIO ) printf("Destroying channel %p\n", ch);

  err = _qio_channel_final_flush_unlocked(ch);
  if( err ) {
    fprintf(stderr, "qio_channel_final_flush returned fatal error %i", err);
    assert( !err );
    abort();
  }

  qio_file_release(ch->file);
  ch->file = NULL;

  qio_lock_destroy(&ch->lock);

  qio_free(ch);
}

static inline
qbuffer_iter_t _av_end_iter(qio_buffered_channel_t* heavy)
{
  qbuffer_iter_t ret;
  ret = qbuffer_end(&heavy->buf);
  // advance a negative amount.
  qbuffer_iter_advance(&heavy->buf, &ret, heavy->av_end - ret.offset);
  return ret;
}

static inline
qbuffer_iter_t _av_start_iter(qio_buffered_channel_t* heavy)
{
  qbuffer_iter_t ret;
  ret = qbuffer_begin(&heavy->buf);
  // advance a positive amount.
  qbuffer_iter_advance(&heavy->buf, &ret, heavy->av_start - ret.offset);
  return ret;
}

static inline
int64_t _right_mark_start(qio_buffered_channel_t* heavy)
{
  int64_t right_mark_start;

  if( heavy->mark_next == 0 ) right_mark_start = heavy->av_start;
  else right_mark_start = heavy->mark_stack[heavy->mark_next-1];

  return right_mark_start;
}

static inline
void _add_right_mark_start(qio_buffered_channel_t* heavy, int64_t amt)
{
  if( heavy->mark_next == 0 ) heavy->av_start += amt;
  else heavy->mark_stack[heavy->mark_next-1] += amt;
}

static inline
void _set_right_mark_start(qio_buffered_channel_t* heavy, int64_t pos)
{
  if( heavy->mark_next == 0 ) heavy->av_start = pos;
  else heavy->mark_stack[heavy->mark_next-1] = pos;
}

static
qbuffer_iter_t _right_mark_start_iter(qio_buffered_channel_t* heavy)
{
  int64_t right_mark_start;
  qbuffer_iter_t ret;

  right_mark_start = _right_mark_start(heavy);

  /*
  ret = qbuffer_begin(&heavy->buf);
  // advance a positive amount.
  qbuffer_iter_advance(&heavy->buf, &ret, right_mark_start - ret.offset);
  */
  ret = qbuffer_iter_at(&heavy->buf, right_mark_start);
  return ret;
}


// allocate >= amt buffer space, and put it into the
// buffer, but don't advance any iterators.
static
err_t _buffered_allocate_bufferspace(qio_channel_t* ch, int64_t amt, int64_t max_amt)
{
  int64_t left = amt;
  int64_t max_left = max_amt;
  int64_t uselen;
  qbytes_t* tmp;
  qio_buffered_channel_t* heavy = &ch->u.buffered;
  err_t err;

  // allocate some space!
  while( left > 0 ) {
    err = qbytes_create_iobuf(&tmp);
    if( err ) goto error;
    uselen = tmp->len;
    if( uselen > max_left ) uselen = max_left;
    err = qbuffer_append(&heavy->buf, tmp, 0, uselen);
    qbytes_release(tmp); // free it when buffer is done with it.
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
err_t _buffered_makespace_atleast(qio_channel_t* ch, int64_t amt)
{
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  err_t err;
  int64_t max_amt;
  int return_eof = 0;

  // do not exceed end_pos.
  max_amt = ch->end_pos - heavy->av_end;
  if( amt > max_amt ) {
    amt = max_amt;
    return_eof = 1;
  }

  err = _buffered_allocate_bufferspace(ch, amt, max_amt);
  if( err ) return err;

  heavy->av_end = qbuffer_end_offset(&heavy->buf);

  if( return_eof ) return EEOF;
  else return 0;
}

static
err_t _buffered_get_memory_file_lock_held(qio_channel_t* ch, int64_t amt, int writing)
{
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  err_t err;
  int eof = 0;
  int64_t start, end;
  qbuffer_iter_t fstart;
  qbuffer_iter_t fend;


  if( heavy->av_end != qbuffer_end_offset(&heavy->buf) ) {
    return EINVAL;
  }


  start = qbuffer_end_offset(&heavy->buf);
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
  err = qbuffer_append_buffer(&heavy->buf, ch->file->buf, fstart, fend);
  if( err ) goto error;

  // Update av_end to be pointing to the end of the buffer.
  heavy->av_end += qbuffer_iter_num_bytes(fstart, fend);

  // OK!
  err = 0;
  // make sure to return EEOF if we ran out of room.
  if( eof ) err = EEOF;

error:
  return err;
}


static
err_t _buffered_get_memory(qio_channel_t* ch, int64_t amt, int writing)
{
  err_t err;
  // lock the file's buffer, which protects
  // access to file->buf.
  err = qio_lock(&ch->file->lock);
  if( err ) return err;

  err = _buffered_get_memory_file_lock_held(ch, amt, writing);

  qio_unlock(&ch->file->lock);
  return err;
}

static
err_t _buffered_get_mmap(qio_channel_t* ch, int64_t amt_in, int writing)
{
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  qbuffer_iter_t start;
  err_t err;
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

  if( heavy->av_end != qbuffer_end_offset(&heavy->buf) ) {
    return EINVAL;
  }

  // round start down to page size.
  err = sys_sysconf(_SC_PAGESIZE, &pagesize);
  if( err ) return err;

  start = qbuffer_end(&heavy->buf);

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
  err = sys_fstat(ch->file->fd, &stats);
  if( err ) return err;

  // do not exceed end_pos.
  if( map_start + len > ch->end_pos ) {
    len = ch->end_pos - map_start;
  }

  if( map_start + len > stats.st_size ) {
    if( writing ) {
      // Extend the file by writing a 0 at the
      // last position of the mapping. Note that there is no
      // race condition problem with multiple writers when we
      // do it this way (vs ftruncate) since if somebody else
      // extends the file past this position after we called fstat,
      // we won't mess up their mmap.
      
      // This code could be used on a system without posix_fallocate
      //err = sys_pwrite(ch->file->fd, &zero, 1, map_start + len - 1, &wrote);
      //if( err ) return err;
      
      // Make sure that we have actual disk space for the mapping.
      // Note that posix_fallocate returns an error code.
      err = sys_posix_fallocate(ch->file->fd, map_start, len);
      if( err ) return err;
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

    if( len > SSIZE_MAX ) return EOVERFLOW;

    err = sys_mmap(NULL, len, prot, MAP_SHARED, ch->file->fd, map_start, &data);
    if( err ) return err;

    err = qbytes_create_generic(&bytes, data, len, qbytes_free_munmap);
    if( err ) {
      sys_munmap(data, len);
      return err;
    }

    err = qbuffer_append(&heavy->buf, bytes, skip, len - skip);
    qbytes_release(bytes); // munmaps on error, decs ref count normally.
    heavy->av_end += len - skip;

    if( err ) return err;
  }

  if( eof ) return EEOF;
  else return 0;
}

// Runs read or pread, whichever is appropriate,
// to read into the buffer.
static
err_t _buffered_read_atleast(qio_channel_t* ch, int64_t amt)
{
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  qbuffer_iter_t read_start;
  qbuffer_iter_t read_end;
  int64_t num_read;
  int64_t left = amt;
  int64_t max_amt;
  int return_eof = 0;
  err_t err;
  qio_method_t method = ch->hints & QIO_METHODMASK;

  // do not exceed end_pos.
  max_amt = ch->end_pos - heavy->av_end;
  if( amt > max_amt ) {
    amt = max_amt;
    return_eof = 1;
  }

  //printf("Allocating bufferspace %lli\n", (long long int) amt);
  err = _buffered_allocate_bufferspace(ch, amt, max_amt);
  if( err ) return err;

  read_start = _av_end_iter(heavy);

  left = amt;
  while(left > 0) {
    read_end = read_start;
    qbuffer_iter_advance(&heavy->buf, &read_end, left);

    // Round up to whole chunk.
    qbuffer_iter_ceil_part(&heavy->buf, &read_end);

    err = EINVAL;
    num_read = 0;
    switch (method) {
      case QIO_METHOD_READWRITE:
        err = qio_readv(ch->file->fd, &heavy->buf, read_start, read_end, &num_read);
        break;
      case QIO_METHOD_PREADPWRITE:
        err = qio_preadv(ch->file->fd, &heavy->buf, read_start, read_end, read_start.offset, &num_read);
        break;
      case QIO_METHOD_FREADFWRITE:
        err = qio_freadv(ch->file->fp, &heavy->buf, read_start, read_end, &num_read);
        break;
      case QIO_METHOD_MMAP:
      case QIO_METHOD_MEMORY:
        err = EINVAL; // should've been handled outside this method!
        break;
      // no default to get warnings when new methods are added
    }

    left -= num_read;
    qbuffer_iter_advance(&heavy->buf, &read_start, num_read);

    if( err ) break;
  }

  heavy->av_end = read_start.offset;

  if( err ) return err;

  if( return_eof ) return EEOF;
  else return 0;
}

void _qio_buffered_advance_cached(qio_channel_t* ch)
{
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  int64_t av_bytes;

  // Update! Find the place the cached data is from.

  av_bytes = heavy->av_end - _right_mark_start(heavy);
  if( av_bytes > 0 && ch->cached_end ) {
    qbuffer_iter_t start;
    qbuffer_iter_t end;
    qbytes_t* bytes;
    int64_t skip;
    int64_t len;
    void* start_ptr;
    void* end_ptr;
    int64_t amt;

    start = _right_mark_start_iter(heavy);
    end = _av_end_iter(heavy);
    qbuffer_iter_get(start, end, &bytes, &skip, &len);

    if( len > av_bytes ) len = av_bytes;

    start_ptr = VOID_PTR_ADD(bytes->data, skip);
    end_ptr = VOID_PTR_ADD(bytes->data, skip + len);

    //printf("expected %p %p, got %p %p\n", start_ptr, end_ptr, ch->cached_cur, ch->cached_end);
    // OK, we should have cached_end == end
    assert( end_ptr == ch->cached_end );
    assert( VOID_PTR_DIFF(end_ptr,ch->cached_cur) >= 0 );
    assert( VOID_PTR_DIFF(ch->cached_cur,start_ptr) >= 0 );

    amt = VOID_PTR_DIFF(ch->cached_cur, start_ptr);

    _add_right_mark_start(heavy, amt);

    // Clear out where we are.. If we're doing this
    // before a read or a write, we'll recompute it in a jiffy.
    ch->cached_cur = NULL;
    ch->cached_end = NULL;
  } else {
    if( ch->cached_end ) assert( av_bytes > 0 );
  }
}

void _qio_buffered_setup_cached(qio_channel_t* ch)
{
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  int64_t av_bytes;

  ch->cached_cur = NULL;
  ch->cached_end = NULL;

  // disable the fast path if the right hint is set.
  if( ch->hints & QIO_HINT_NOFAST ) return;

  // Put the available data in read_cur and read_end.
  av_bytes = heavy->av_end - _right_mark_start(heavy);
  if( av_bytes > 0 ) {
    qbuffer_iter_t start;
    qbuffer_iter_t end;
    qbytes_t* bytes;
    int64_t skip;
    int64_t len;

    start = _right_mark_start_iter(heavy);
    end = _av_end_iter(heavy);
    qbuffer_iter_get(start, end, &bytes, &skip, &len);

    ch->cached_cur = VOID_PTR_ADD(bytes->data, skip);
    ch->cached_end = VOID_PTR_ADD(bytes->data, skip + len);
    //printf("setup has len=%li\n", (long int) len);
  }
  //printf("setup cached %p %p\n", ch->cached_cur, ch->cached_end);
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
err_t _qio_buffered_behind(qio_channel_t* ch, int flushall)
{
  // write any chunks before av_start
  // pop them from the buffer
  // recompute the iterators.
  qbuffer_iter_t start;
  qbuffer_iter_t write_start;
  qbuffer_iter_t write_end;
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  err_t err;
  int64_t num_written;
  qio_method_t method = ch->hints & QIO_METHODMASK;

  // If we are a FILE* type buffer, we want to automatically
  // flush after every write, so that C I/O can be intermixed
  // with QIO calls. This is (obviously) not the most perfomant way to do
  // it, but we expect this to be used with stdout/stderr mostly,
  // where timely updating (e.g. line-buffering) is more important
  // than total speed.
  if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_FREADFWRITE ) {
    flushall = 1;
  }

  write_start = qbuffer_begin(&heavy->buf);
  write_end = _av_start_iter(heavy);

  if( !flushall ) {
    // Move write_end back to the start of the chunk
    // we're working on.
    qbuffer_iter_floor_part(&heavy->buf, &write_end);
  } 

  // If there's nothing to write, just return. We don't even need
  // to update the iterators. This is the common case.
  if( qbuffer_iter_num_bytes(write_start, write_end) == 0 ) {
    err = 0;
    goto done;
  }

  //fprintf(stderr, "starting write\n");
  //debug_print_qbuffer(&heavy->buf);

  if(ch->hints & QIO_HINT_DIRECT) {
    // Round out the data to write to the end of the current chunk.
    qbuffer_iter_ceil_part(&heavy->buf, &write_end);
  }

  if(ch->flags & QIO_FDFLAG_WRITEABLE) {
    while( qbuffer_iter_num_bytes(write_start, write_end) > 0 ) {
      err = EINVAL;
      num_written = 0;
      switch (method) {
        case QIO_METHOD_READWRITE:
          err = qio_writev(ch->file->fd, &heavy->buf, write_start, write_end, &num_written);
          break;
        case QIO_METHOD_PREADPWRITE:
          err = qio_pwritev(ch->file->fd, &heavy->buf, write_start, write_end, write_start.offset, &num_written);
          break;
        case QIO_METHOD_FREADFWRITE:
          err = qio_fwritev(ch->file->fp, &heavy->buf, write_start, write_end, &num_written);
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
      qbuffer_iter_advance(&heavy->buf, &write_start, num_written);
    }
  } else {
    // just pretend like we wrote it; in fact we just deallocate
    // the buffer space below.
    write_start = write_end;
  }

  err = 0;

error:
  //fprintf(stderr, "before trim\n");
  //debug_print_qbuffer(&heavy->buf);

  start = qbuffer_begin(&heavy->buf);
  // Now remove parts we wrote. This might invalidate iterators!
  qbuffer_trim_front(&heavy->buf, qbuffer_iter_num_bytes(start, write_start));

  //fprintf(stderr, "after trim\n");
  //debug_print_qbuffer(&heavy->buf);

done:
  if( !err ) {
    _qio_buffered_setup_cached(ch);
  }

  return err;
}

static
err_t _qio_buffered_require(qio_channel_t* ch, int64_t amt, int writing)
{
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  err_t err;

  int64_t n_available;
  int64_t n_needed;

  // Include whatever data we got in cached_cur/cached_end
  _qio_buffered_advance_cached(ch);

  n_available = heavy->av_end - _right_mark_start(heavy);
  // Is the data already available?
  if( amt <= n_available ) {
    // Great! Don't do anything.
    return 0;
  }

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
err_t _qio_buffered_read(qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* amt_read)
{
  qbuffer_iter_t start;
  qbuffer_iter_t end;
  int64_t gotlen = 0;
  qio_buffered_channel_t* heavy = &ch->u.buffered;
  err_t err;
  int eof;

  // do the actual read. (require calls advance_cached)
  err = _qio_buffered_require(ch, len, 0);
  eof = 0;
  if( err == EEOF ) eof = 1;
  else if( err ) goto error;

  // figure out the end of the data to copy
  gotlen = heavy->av_end - _right_mark_start(heavy);
  start = _right_mark_start_iter(heavy);
  if( len < gotlen ) {
    gotlen = len;
    end = start;
    qbuffer_iter_advance(&heavy->buf, &end, len);
  } else {
    end = _av_end_iter(heavy);
  }

  // Now copy out the data.
  err = qbuffer_copyout(&heavy->buf, start, end, ptr, gotlen);
  if( err ) goto error;

  // now advance the start of the available buffer by the amount.
  _set_right_mark_start(heavy, end.offset);

  // did we get to a different part? if so, we can release some
  // buffers.
  err = _qio_buffered_behind(ch, false);
  if( err ) goto error;

  err = 0;
  if( eof ) err = EEOF;

error:
  *amt_read = gotlen;
  return err;
}

/*
err_t qio_buffered_read_until(qio_channel_t* ch, qbytes_t** ret, recognizer_t recognize, void* state)
{
  assert(0);
}
*/

static
err_t _qio_buffered_write(qio_channel_t* ch, const void* ptr, ssize_t len, ssize_t *amt_written)
{
  qbuffer_iter_t start;
  qbuffer_iter_t end;
  int64_t gotlen = 0;
  qio_buffered_channel_t* heavy = & ch->u.buffered;
  err_t err;
  int eof;

  // make sure we have buffer space. (require calls advance_cached)
  err = _qio_buffered_require(ch, len, 1);
  eof = 0;
  if( err == EEOF ) eof = 1;
  else if( err ) goto error;

  // figure out the end of the data to copy
  start = _right_mark_start_iter(heavy);
  end = start;
  gotlen = qbuffer_iter_num_bytes_after(&heavy->buf, end);
  if( len < gotlen ) gotlen = len;
  qbuffer_iter_advance(&heavy->buf, &end, gotlen);

  // now copy the data in to the buffer.
  err = qbuffer_copyin(&heavy->buf, start, end, ptr, gotlen);
  if( err ) goto error;

  // now move start forward.
  _set_right_mark_start(heavy, end.offset);

  // did we get to a different part? If so, call write()
  err = _qio_buffered_behind(ch, false);
  if( err ) goto error;

  err = 0;
  if( eof ) err = EEOF;
error:
  *amt_written = gotlen;
  return err;
}

static
err_t _qio_unbuffered_write(qio_channel_t* ch, const void* ptr, ssize_t len_in, ssize_t *amt_written)
{
  ssize_t num_written;
  size_t num_written_u;
  ssize_t len;
  err_t err;
  qio_unbuffered_channel_t *light = & ch->u.unbuffered;
  qio_method_t method = ch->hints & QIO_METHODMASK;
  int return_eof = 0;

  // do not exceed end_pos.
  if( light->pos + len_in > ch->end_pos ) {
    len_in = ch->end_pos - light->pos;
    return_eof = 1;
  }
  len = len_in;

  if( method == QIO_METHOD_MMAP &&
      ch->file->mmap && light->pos + len <= ch->file->mmap->len) {
    // Copy the data to the mmap.
    memcpy( VOID_PTR_ADD(ch->file->mmap->data,light->pos), ptr, len);
    light->pos += len;
  } else {
    while( len > 0 ) {
      err = EINVAL;
      num_written = 0;
      switch (method) {
        case QIO_METHOD_READWRITE:
          err = sys_write(ch->file->fd, ptr, len, &num_written);
          break;
        case QIO_METHOD_MMAP: // mmap uses pread/pwrite when we're 
                              // outside the mmap'd region.
        case QIO_METHOD_PREADPWRITE:
          err = sys_pwrite(ch->file->fd, ptr, len, light->pos, &num_written);
          break;
        case QIO_METHOD_FREADFWRITE:
          if( ch->file->fp ) {
            num_written_u = fwrite(ptr, 1, len, ch->file->fp);
            err = 0;
            if( num_written_u == 0 ) {
              err = ferror(ch->file->fp);
            }
            num_written = num_written_u;
          } else {
            err = EINVAL;
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
          err = EINVAL;
          break;
        // no default to get warnings when new methods are added
      }
      if( err ) {
        *amt_written = num_written + len_in - len;
        return err;
      }
      ptr = VOID_PTR_ADD(ptr, num_written);
      len -= num_written;
      light->pos += num_written;
    }
  }

  *amt_written = len_in;
  if( return_eof ) return EEOF;
  else return 0;
}

static
err_t _qio_unbuffered_read(qio_channel_t* ch, void* ptr, ssize_t len_in, ssize_t *amt_read)
{
  ssize_t num_read;
  size_t num_read_u;
  ssize_t len;
  err_t err;
  qio_unbuffered_channel_t *light = & ch->u.unbuffered;
  qio_method_t method = ch->hints & QIO_METHODMASK;
  int return_eof = 0;

  // do not exceed end_pos.
  if( light->pos + len_in > ch->end_pos ) {
    len_in = ch->end_pos - light->pos;
    return_eof = 1;
  }
  len = len_in;

  if( ch->file->mmap && light->pos + len <= ch->file->mmap->len) {
    // Copy the data out of the mmap.
    memcpy( ptr, VOID_PTR_ADD(ch->file->mmap->data,light->pos), len);
    light->pos += len;
  } else {
    while( len > 0 ) {
      err = EINVAL;
      num_read = 0;
      switch (method) {
        case QIO_METHOD_READWRITE:
          err = sys_read(ch->file->fd, ptr, len, &num_read);
          break;
        case QIO_METHOD_MMAP:
        case QIO_METHOD_PREADPWRITE:
          err = sys_pread(ch->file->fd, ptr, len, light->pos, &num_read);
          break;
        case QIO_METHOD_FREADFWRITE:
          if( ch->file->fp ) {
            num_read_u = fread(ptr, 1, len, ch->file->fp);
            err = 0;
            if( num_read_u == 0 ) {
              if( feof(ch->file->fp) ) err = EEOF;
              else err = ferror(ch->file->fp);
            }
            num_read = num_read_u;
          } else {
            err = EINVAL;
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
          err = EINVAL;
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
      light->pos += num_read;
    }
  }

  *amt_read = len_in;
  if( return_eof ) return EEOF;
  else return 0;
}

err_t _qio_channel_flush_unlocked(qio_channel_t* ch)
{
  err_t err;
  qio_method_t method = ch->hints & QIO_METHODMASK;
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  switch (type) {
    case QIO_CH_UNBUFFERED:
      err = 0; // no user buffer, nothing to flush
      break;
    case QIO_CH_BUFFERED:
      _qio_buffered_advance_cached(ch);
      err = _qio_buffered_behind(ch, true);
      if( err ) return err;
      err = 0;
      break;
    // No default; hope for useful errors if any added
  }

  if( err ) return err;

  // Also flush cstdio buffer if we're using fread/fwrite.
  if( method == QIO_METHOD_FREADFWRITE ) {
    int got = fflush(ch->file->fp);
    if( got ) err = errno;
    else err = 0;
  }

  return err;
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
err_t _qio_slow_write(qio_channel_t* ch, const void* ptr, ssize_t len, ssize_t* amt_written)
{
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  *amt_written = 0;

  if( ! (ch->flags & QIO_FDFLAG_WRITEABLE ) ) {
    return EINVAL;
  }

  switch (type) {
    case QIO_CH_UNBUFFERED:
      return _qio_unbuffered_write(ch, ptr, len, amt_written);
    case QIO_CH_BUFFERED:
      return _qio_buffered_write(ch, ptr, len, amt_written);
    // No default; hope for useful errors if any added
  }

  return EINVAL;
}

err_t _qio_slow_read(qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* amt_read)
{
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  *amt_read = 0;

  if( ! (ch->flags & QIO_FDFLAG_READABLE ) ) {
    return EINVAL;
  }

  switch (type) {
    case QIO_CH_UNBUFFERED:
      return _qio_unbuffered_read(ch, ptr, len, amt_read);
    case QIO_CH_BUFFERED:
      return _qio_buffered_read(ch, ptr, len, amt_read);
    // No default; hope for useful errors if any added
  }

  return EINVAL;
}


err_t _qio_channel_require_unlocked(qio_channel_t* ch, int64_t space, int writing)
{
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  if( type != QIO_CH_BUFFERED ) return EINVAL;

  return _qio_buffered_require(ch, space, writing); 
}

/*
static
void switch_mmap_to_pwrite(qio_channel_t* ch)
{
  qio_hint_t hints = ch->hints;
  qio_method_t method = ch->hints & QIO_METHODMASK;
  if( method == QIO_METHOD_MMAP ) {
    hints = hints & ~QIO_METHODMASK; // clear method number.
    method = QIO_METHOD_PREADPWRITE;
    hints |= method; // set to PREAD/PWRITE
  }
}
*/

// Only returns  EINVAL if unknown buffer type or a locking error.
int64_t qio_channel_offset_unlocked(qio_channel_t* ch)
{
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  switch( type ) {
    case QIO_CH_UNBUFFERED:
      return ch->u.unbuffered.pos;
    case QIO_CH_BUFFERED:
      _qio_buffered_advance_cached(ch);
      return _right_mark_start(& ch->u.buffered);
    // no default in case more are added.
  }

  assert(0); // unknown ch->type.
}

// Only returns locking errors (ie when threadsafe=true).
err_t qio_channel_offset(const int threadsafe, qio_channel_t* ch, int64_t* offset_out)
{
  err_t err;

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


static
err_t _qio_channel_put_bytes_unlocked(qio_channel_t* ch, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes)
{
  qio_buffered_channel_t* heavy;
  error_t err;
  int64_t use_len, use_skip, copylen;

  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;
  qio_method_t method = ch->hints & QIO_METHODMASK;

  if( type != QIO_CH_BUFFERED ) return EINVAL;

  heavy = & ch->u.buffered;

  // advance past any data put in cached_cur
  _qio_buffered_advance_cached(ch);

  // check that we don't write past the channel's region.
  {
    int64_t start, end;
    start = _right_mark_start(heavy);
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
    if( err == EEOF ) err = 0; // ignore EOF, we'll just append in a moment.
    if( err ) goto error;
  } else {
    // Remove everything after write-mark from the channel buffer
    // so we can zero-copy the bytes in here.
    heavy->av_end = _right_mark_start(heavy);
    qbuffer_trim_back(& heavy->buf, qbuffer_end_offset(&heavy->buf) - heavy->av_end);
  }

  if( heavy->av_end != qbuffer_end_offset(&heavy->buf) ) {
   err = EINVAL;
   goto error;
  }

  // Now, copy data from bytes in as long as there's
  // buffer area to copy in to.
  copylen = heavy->av_end - _right_mark_start( heavy );
  if( copylen > 0 ) {
    err = qbuffer_copyin(& heavy->buf,
                         _right_mark_start_iter( heavy ),
                         qbuffer_end(&heavy->buf),
                         VOID_PTR_ADD(bytes->data, use_skip), copylen);
    if( err ) goto error;

    use_skip += copylen;
    use_len -= copylen;

    // Update the channel position.
    _set_right_mark_start(heavy, _right_mark_start(heavy) + copylen);
  }

  // Now, append bytes until there's no more appending to do.
  if( use_len > 0 ) {
    if( method == QIO_METHOD_MMAP ) return EINVAL;

    // If we're a MEMORY buffer, append to the file->buf.
    // We take care of locking file above
    if( method == QIO_METHOD_MEMORY ) {
      err = qbuffer_append( ch->file->buf, bytes, use_skip, use_len);
      if( err ) goto error;
    }

    // Append to the channel.
    err = qbuffer_append(& heavy->buf, bytes, use_skip, use_len);
    if( err ) goto error;

    // Update the channel position.
    heavy->av_end = qbuffer_end_offset(&heavy->buf);
    _set_right_mark_start(heavy, _right_mark_start(heavy) + use_len);
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

err_t qio_channel_put_bytes(const int threadsafe, qio_channel_t* ch, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes)
{
  err_t err;

  if( ! (ch->flags & QIO_FDFLAG_WRITEABLE) ) return EBADF;

  if( skip_bytes < 0 || len_bytes < 0 ) return EINVAL;

  if( skip_bytes + len_bytes > bytes->len ) {
    return EINVAL;
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
err_t _qio_channel_put_buffer_unlocked(qio_channel_t* ch, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end)
{
  qio_buffered_channel_t* heavy;
  error_t err;
  int64_t use_len, copylen;
  qbuffer_iter_t src_copy_end;

  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;
  qio_method_t method = ch->hints & QIO_METHODMASK;

  if( type != QIO_CH_BUFFERED ) return EINVAL;

  heavy = & ch->u.buffered;

  // advance past any data put in cached_cur
  _qio_buffered_advance_cached(ch);

  // check that we don't write past the channel's region.
  {
    int64_t start, end;

    start = _right_mark_start(heavy);
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
    if( err == EEOF ) err = 0; // ignore EOF, we'll just append in a moment.
    if( err ) goto error;
  } else {
    // Remove everything after write-mark from the channel buffer
    // so we can zero-copy the bytes in here.
    heavy->av_end = _right_mark_start(heavy);
    qbuffer_trim_back(& heavy->buf, qbuffer_end_offset(&heavy->buf) - heavy->av_end);
  }

  if( heavy->av_end != qbuffer_end_offset(&heavy->buf) ) {
    err = EINVAL;
    goto error;
  }

  // Now, copy data from src_buffer in as long as there's
  // buffer area to copy in to.
  copylen = heavy->av_end - _right_mark_start(heavy);
  if( copylen > 0 ) {
    src_copy_end = src_start;
    qbuffer_iter_advance(src, &src_copy_end, copylen);

    err = qbuffer_copyin_buffer(& heavy->buf,
                                _right_mark_start_iter(heavy),
                                qbuffer_end(&heavy->buf),
                                src,
                                src_start,
                                src_copy_end);
    if( err ) goto error;

    qbuffer_iter_advance(src, &src_start, copylen);
    use_len -= copylen;

    // Update the channel position.
    _set_right_mark_start(heavy, _right_mark_start(heavy) + copylen);
  }

  // Now, append bytes until there's no more appending to do.
  if( use_len > 0 ) {
    if( method == QIO_METHOD_MMAP ) return EINVAL;

    // If we're a MEMORY buffer, append to the file->buf.
    // We take care of locking file above
    if( method == QIO_METHOD_MEMORY ) {
      err = qbuffer_append_buffer( ch->file->buf, src, src_start, src_end);
      if( err ) goto error;
    }

    // Append to the channel.
    err = qbuffer_append_buffer(& heavy->buf, src, src_start, src_end);
    if( err ) goto error;

    // Update the channel position.
    heavy->av_end = qbuffer_end_offset(&heavy->buf);
    _set_right_mark_start(heavy, _right_mark_start(heavy) + use_len);
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

err_t qio_channel_put_buffer(const int threadsafe, qio_channel_t* ch, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end)
{
  err_t err;

  if( ! (ch->flags & QIO_FDFLAG_WRITEABLE) ) return EBADF;

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
err_t qio_channel_begin_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t require, int writing, qbuffer_t** buf_out, qbuffer_iter_t* start_out, qbuffer_iter_t* end_out)
{
  qio_buffered_channel_t* heavy;
  err_t err;

  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  *buf_out = NULL;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  if( type != QIO_CH_BUFFERED ) {
    qio_unlock(&ch->lock);
    return EINVAL;
  }

  // require calls advance_cached.
  err = _qio_channel_require_unlocked(ch, require, writing);
  if( err ) {
    qio_unlock(&ch->lock);
    return err;
  }

  heavy = & ch->u.buffered;

  *buf_out = &heavy->buf;
  *start_out = _right_mark_start_iter(heavy);
  *end_out = _av_end_iter(heavy);

  err = 0;

  return err;
}


err_t _qio_buffered_behind(qio_channel_t* ch, int flushall);

err_t qio_channel_end_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t advance)
{
  qio_buffered_channel_t* heavy;
  err_t err;
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  if( type != QIO_CH_BUFFERED ) {
    err = EINVAL;
    goto error;
  }

  heavy = & ch->u.buffered;

  _add_right_mark_start(heavy, advance);

  err = _qio_buffered_behind(ch, false);

error:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

err_t qio_channel_mark(const int threadsafe, qio_channel_t* ch)
{
  err_t err;
  qio_buffered_channel_t* heavy;
  int64_t pos = -1;
  size_t new_size;
  size_t i;
  int64_t* new_buf;

  if( (ch->hints & QIO_CHTYPEMASK) != QIO_CH_BUFFERED ) return EINVAL;

  heavy = & ch->u.buffered;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }
  
  // calls qio_buffered_advance_cached
  pos = qio_channel_offset_unlocked(ch);
  
  if( heavy->mark_next >= heavy->mark_stack_size ) {
    new_size = 2 * heavy->mark_next;
    if( new_size == 0 ) new_size = 2;

    // Reallocate the mark buffer.
    if( heavy->mark_stack == heavy->mark_space ) {
      new_buf = qio_malloc(new_size*sizeof(int64_t));
      if( ! new_buf ) {
        err = ENOMEM;
        goto error;
      }
      // Copy the values from our old stack.
      for( i = 0; i < heavy->mark_next; i++ ) new_buf[i] = heavy->mark_stack[i];
    } else {
      new_buf = qio_realloc(heavy->mark_space, new_size*sizeof(int64_t));
      if( ! new_buf ) {
        err = ENOMEM;
        goto error;
      }
      // Realloc already copies the values if necessary.
    }
    // Now clear out the new elements.
    for( i = heavy->mark_next + 1; i < new_size; i++ ) {
      new_buf[i] = -1;
    }

    heavy->mark_stack = new_buf;
    heavy->mark_stack_size = new_size;
  }

  // Now set the current value.
  heavy->mark_stack[heavy->mark_next] = pos;
  heavy->mark_next++;

  err = 0;

error:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}


/* Always advances, calls qio_buffered_behind, but ignores an
 * error if there is one. The error will come up again in a call
 * to read/write/flush.
 */
void qio_channel_advance_unlocked(qio_channel_t* ch, int64_t nbytes)
{
  qio_buffered_channel_t* heavy;
  err_t err;

  assert(nbytes >= 0);

  _qio_buffered_advance_cached(ch);

  heavy = & ch->u.buffered;

  _add_right_mark_start(heavy, nbytes);

  // If qio_buffered_behind fails, it will presumably
  // fail again on flush/close. So we ignore the
  // error code.
  err = _qio_buffered_behind(ch, false);
  if( err ) fprintf(stderr, "Warning: qio_buffered_behind returned error %i in qio_channel_advance_unlocked\n", err);
}

void qio_channel_revert_unlocked(qio_channel_t* ch)
{
  qio_buffered_channel_t* heavy;

  assert( (ch->hints & QIO_CHTYPEMASK) == QIO_CH_BUFFERED );

  heavy = & ch->u.buffered;

  assert(heavy->mark_next > 0);

  // Might need to call advance_cached if there was
  // writes before we marked...
  _qio_buffered_advance_cached(ch);

  heavy->mark_next--;
  heavy->mark_stack[heavy->mark_next] = -1;

  // Now we just have lots of extra buffer space. No need
  // to call write-behind.

  // Set up the buffered pointers again.
  _qio_buffered_setup_cached(ch);
}

void qio_channel_commit_unlocked(qio_channel_t* ch)
{
  qio_buffered_channel_t* heavy;
  int64_t pos = -1;
  int64_t mark = -1;

  assert( (ch->hints & QIO_CHTYPEMASK) == QIO_CH_BUFFERED );

  heavy = & ch->u.buffered;

  assert(heavy->mark_next > 0);

  // For sure we need to save any data written to
  // the cached pointers
  _qio_buffered_advance_cached(ch);

  heavy->mark_next--;
  mark = heavy->mark_stack[heavy->mark_next];
  heavy->mark_stack[heavy->mark_next] = -1;
 
  // If we're at the end of the mark stack, advance
  // av_end and call read/write-behind.
  if( heavy->mark_next == 0 ) {
    // calls advance_cached
    pos = qio_channel_offset_unlocked(ch);

    assert( mark >= pos );

    qio_channel_advance_unlocked(ch, mark - pos);
  }
  // Otherwise, we just keep going with the mark stack,
  // until we get to the end.
}

err_t qio_channel_advance(const int threadsafe, qio_channel_t* ch, int64_t nbytes)
{
  qio_buffered_channel_t* heavy;
  err_t err;
  qio_chtype_t type = ch->hints & QIO_CHTYPEMASK;

  if( type != QIO_CH_BUFFERED ) {
    return EINVAL;
  }
  if( nbytes < 0 ) return EINVAL;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  _qio_buffered_advance_cached(ch);

  heavy = & ch->u.buffered;

  _add_right_mark_start(heavy, nbytes);

  err = _qio_buffered_behind(ch, false);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}


/*
// Peek until a recognizer function is satisfied.
// In *amount, returns the number of bytes we passed.
err_t qio_channel_require_until(const int threadsafe, qio_channel_t* ch, recognizer_t recognize, void* state, int64_t* amount)
{
  err_t err, newerr;
  qio_buffered_channel_t* heavy;
  qbuffer_iter_t start, end;
  int64_t offset;
  int64_t got_offset;

  if( ! (ch->flags & QIO_FDFLAG_READABLE ) ) return EINVAL;
  if( ch->type != QIO_CH_BUFFERED ) return EINVAL;

  *amount = 0;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  // Skip past data in cached/cur
  err = _qio_buffered_advance_cached(ch);
  if( err ) goto error;

  heavy = &ch->u.buffered;
  offset = heavy->av_start;
  while( 1 ) {
    start = qbuffer_iter_at(&heavy->buf, offset);
    end = _av_end_iter(heavy);
    
    got_offset = -1;
    // Now run the recognizer on whatever we have buffered up.
    err = recognize(&heavy->buf, start, end, state, &got_offset);
    if( err ) break;
    got_offset = -1;

    // if we havn't recognized yet, advance.
    offset = heavy->av_end;

    // ask to read more.
    err = _qio_buffered_require(ch, 1 + offset - heavy->av_start, 0);
    if( err ) break;
  }

  if( got_offset != -1 && err == -1 ) {
    // we have recognized!
    *amount = got_offset - heavy->av_start;
  } else if( err == EEOF ) {
    *amount = heavy->av_end - heavy->av_start;
  }

error:
  if( threadsafe ) {
    newerr = qio_unlock(&ch->lock);
    if( ! err ) err = newerr;
  }
  return 0;
}

err_t until_char(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* state, int64_t* offset)
{
  qbytes_t* bytes;
  int64_t skip;
  int64_t len;
  const char* st = (const char*) state;
  char terminator = *st;
  char* ptr_start;
  char* ptr;
  char* ptr_end;

  while( qbuffer_iter_num_bytes(start, end) > 0 ) {
    qbuffer_iter_get(start, end, &bytes, &skip, &len);

    // Look for the character in bytes + skip ...
    ptr_start = bytes->data + skip;
    ptr_end = bytes->data + skip + len;
    for( ptr = ptr_start; ptr != ptr_end; ptr++ ) {
      if( *ptr == terminator ) {
        *offset = start.offset + ptr - ptr_start;
        return -1; // success.
      }
    }

    qbuffer_iter_next_part(buf, &start);
  }

  return 0;
}

err_t until_table(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* state, int64_t* offset)
{
  qbytes_t* bytes;
  int64_t skip;
  int64_t len;
  const char* table = (const char*) state;
  char* ptr_start;
  char* ptr;
  char* ptr_end;
  int ch;

  while( qbuffer_iter_num_bytes(start, end) > 0 ) {
    qbuffer_iter_get(start, end, &bytes, &skip, &len);

    // Look for the character in bytes + skip ...
    ptr_start = bytes->data + skip;
    ptr_end = bytes->data + skip + len;
    for( ptr = ptr_start; ptr != ptr_end; ptr++ ) {
      ch = *ptr;
      if( table[ch] ) {
        *offset = start.offset + ptr - ptr_start;
        return -1; // success.
      }
    }

    qbuffer_iter_next_part(buf, &start);
  }

  return 0;
}

err_t until_char_noescape(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* st, int64_t* offset)
{
  qbytes_t* bytes;
  int64_t skip;
  int64_t len;
  until_noescape_t* st = (until_noescape_t*) state;
  char start_char = st->start_char;
  char terminator = st->end_char;
  char escape = st->escape_char;
  char state = st->state;
  char ch;
  char* ptr_start;
  char* ptr;
  char* ptr_end;
  int64_t pos;

  while( qbuffer_iter_num_bytes(start, end) > 0 ) {
    qbuffer_iter_get(start, end, &bytes, &skip, &len);

    // Look for the character in bytes + skip ...
    ptr_start = bytes->data + skip;
    ptr_end = bytes->data + skip + len;
    for( ptr = ptr_start; ptr != ptr_end; ptr++ ) {
      ch = *ptr;

      if( state == 0 ) {
        // not yet in string
        // the character must be start_char, or we fail.
        if( ch != start_char ) {
          *offset = start.offset;
          return EFORMAT;
        }
      } else if( state == 1 ) {
        if( ch == escape ) state = 2;
        else if( ch == terminator ) {
          *offset = start.offset + ptr - ptr_start;
          return -1; // success.
        }
      } else {
        // after reading escape'd character, go back to in string.
        state = 1;
      }
      last_char = ch;
    }

    qbuffer_iter_next_part(buf, &start);
  }

  st->last_char = last_char;

  return 0;
}
*/
