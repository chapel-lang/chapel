/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _QIO_H_
#define _QIO_H_

#include "sys_basic.h"
#include "bswap.h"
#include "qbuffer.h"
#include "sys.h"
#include "qio_style.h"
#include "qio_error.h"

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

// On some systems contains #define mmap mmap64
// rather than an inline function -- which means that it must be included before
// any mention of the token "mmap".  Most unfortunate. <hilde>
#include <sys/mman.h>

#define DEBUG_QIO 0

#ifdef __cplusplus
extern "C" {
#endif

// synonym for iovec
typedef struct iovec qiovec_t;

typedef enum {
  QIO_FDFLAG_UNK  = 1,
  QIO_FDFLAG_READABLE = 2,
  QIO_FDFLAG_WRITEABLE = 4,
  QIO_FDFLAG_SEEKABLE = 8,
} qio_fdflag_t;

typedef uint32_t qio_hint_t;

// TODO: make these better values
#ifndef FTYPE_NONE
#define FTYPE_NONE 0
#endif

#ifndef FTYPE_LUSTRE
#define FTYPE_LUSTRE LUSTRE_SUPER_MAGIC
#endif

// The qio lock must be re-entrant in order to handle
// e.g. qio_printf, which has will lock the lock, then
// call printf, which will call cookie write, and lock the lock again.
// UPDATE - not implementing qio_printf anymore in terms of FILE* printf,
// but maybe we will in the future support FILE*<->channel..
// UPDATE 2 -- we need reentrant locks for Chapel because
// write(a,b,c) might call
//    a.writeThis() which calls
//        write(16) for example
// UPDATE 3 -- I don't believe this is necessary anymore
//             because of the way writeThis now works

#ifdef __cplusplus
} // end extern "C"
#endif

#ifdef _chplrt_H_
#include "chpl-tasks.h"

// also export iohint_t and fdflag_t
typedef qio_hint_t iohints;
typedef qio_fdflag_t fdflag_t;


// make a re-entrant lock.
typedef struct {
  chpl_sync_aux_t sv;
  chpl_taskID_t owner; // task ID of owner.
  uint64_t count; // how many times owner has locked.
} qio_lock_t;

#define NULL_OWNER chpl_nullTaskID

#ifdef __cplusplus
extern "C" {
#endif

qioerr qio_lock(qio_lock_t* x);
void qio_unlock(qio_lock_t* x);

static inline qioerr qio_lock_init(qio_lock_t* x) {
  x->owner = NULL_OWNER;
  x->count = 0;
  chpl_sync_initAux(&x->sv);
  return 0;
}

static inline void qio_lock_destroy(qio_lock_t* x) {
  chpl_sync_destroyAux(&x->sv);
}

#ifdef __cplusplus
} // end extern "C"
#endif

#else

#ifndef CHPL_RT_UNIT_TEST
#error Chapel runtime should be included before QIO or CHPL_RT_UNIT_TEST should be set
#endif

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef pthread_mutex_t qio_lock_t;
// these should return 0 on success; otherwise, an error number.
static inline qioerr qio_lock(qio_lock_t* x) {
  return qio_int_to_err(pthread_mutex_lock(x));
}

// qio_unlock does not return an error because it can only usefully
// return the error that the mutex isn't owned by this process
// (or possibly that it's invalid), both of which are programming errors
// and not anything that can reasonably be responded to.
static inline void qio_unlock(qio_lock_t* x) { int rc = pthread_mutex_unlock(x); if( rc ) { assert(rc == 0); abort(); } }

static inline qioerr qio_lock_init(qio_lock_t* x) {
  pthread_mutexattr_t attr;
  err_t err, newerr;

  err = pthread_mutexattr_init(&attr);
  if( err ) return qio_int_to_err(err);
  err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  if( ! err ) {
    err = pthread_mutex_init(x, &attr);
  }

  newerr = pthread_mutexattr_destroy(&attr);
  if( ! err ) err = newerr;

  return qio_int_to_err(err);
}

// returns void for the same reason as qio_unlock.
static inline void qio_lock_destroy(qio_lock_t* x) { int rc = pthread_mutex_destroy(x); if( rc ) { assert(rc == 0); abort(); } }

#ifdef __cplusplus
} // end extern "C"
#endif

#endif


extern ssize_t qio_too_small_for_default_mmap;
extern ssize_t qio_too_large_for_default_mmap;
extern ssize_t qio_mmap_chunk_iobufs;

#ifdef __cplusplus
extern "C" {
#endif

/* Wrap system calls readv, writev, preadv, pwritev
 * to take a buffer.
 */

qioerr qio_freadv(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_read);
qioerr qio_fwritev(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_written);


qioerr qio_recv(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end,
              int flags,
              sys_sockaddr_t* src_addr_out, /* can be NULL */
              void* ancillary_out, socklen_t* ancillary_len_inout, /* can be NULL */
              ssize_t* num_recvd_out);

qioerr qio_send(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end,
              int flags,
              const sys_sockaddr_t* dst_addr, /* can be NULL */
              const void* ancillary, /* can be NULL */
              socklen_t ancillary_len,
              ssize_t* num_sent_out);

typedef enum {
  QIO_CH_ALWAYS_UNBUFFERED = 1,
  QIO_CH_ALWAYS_BUFFERED,
  QIO_CH_BUFFERED,
  QIO_CHTYPEMASK = 0x000f,
  QIO_CHTYPE_CLOSED = 0x000f
} qio_chtype_t;

#define QIO_CH_DEFAULT 0
#define QIO_CH_MIN_TYPE QIO_CH_ALWAYS_UNBUFFERED
#define QIO_CH_MAX_TYPE QIO_CH_BUFFERED

char* qio_chtype_to_string(qio_chtype_t type);

/*
 if file->fp is set, we use fread/fwrite.

     -- default ->
            read-only, 16k < file size < 2 GB, mmap
            otherwise, pread/pwrite
     -- sequential -- same as default
     -- random -- same as default
     -- noreuse -- pread/pwrite
     -- cached -- mmap for reads and writes
     -- force_readwrite
 */

#define QIO_HINT_AFTERCHTYPE 0x0010

typedef enum {
  QIO_METHOD_READWRITE = QIO_HINT_AFTERCHTYPE,
  QIO_METHOD_PREADPWRITE = 2*QIO_HINT_AFTERCHTYPE,
  QIO_METHOD_FREADFWRITE = 3*QIO_HINT_AFTERCHTYPE,
  QIO_METHOD_MMAP = 4*QIO_HINT_AFTERCHTYPE,
  QIO_METHOD_MEMORY = 5*QIO_HINT_AFTERCHTYPE,
  //QIO_METHOD_LIBEVENT,
} qio_method_t;
#define QIO_METHODMASK 0x00f0
#define QIO_HINT_AFTERMETHOD 0x0100
#define QIO_METHOD_DEFAULT 0
#define QIO_MIN_METHOD QIO_METHOD_READWRITE
#define QIO_MAX_METHOD QIO_METHOD_MEMORY

enum {
  QIO_HINT_RANDOM       = QIO_HINT_AFTERMETHOD,
  QIO_HINT_SEQUENTIAL   = QIO_HINT_RANDOM<<1,
  QIO_HINT_LATENCY      = QIO_HINT_SEQUENTIAL<<1,
  QIO_HINT_BANDWIDTH    = QIO_HINT_LATENCY<<1,
  QIO_HINT_CACHED       = QIO_HINT_BANDWIDTH<<1,
  QIO_HINT_PARALLEL     = QIO_HINT_CACHED<<1,
  QIO_HINT_DIRECT       = QIO_HINT_PARALLEL<<1,
     // note -- if DIRECT is set, you must do aligned I/O;
     // offset, request size must be 512-byte aligned, and
     // user buffer must be page-aligned. This should more or
     // less work the way you expect for a buffered channel,
     // but you'll have to round out all files to a multiple of 512
     // bytes. This library could help by turning off O_DIRECT when
     // encountering improper alignment, but the linux open man page
     // says:
//Applications should avoid mixing O_DIRECT and normal I/O to the same file, and
//especially to overlapping byte regions in the same file.  Even when the file
//system correctly handles the coherency issues in this situation, overall I/O
//throughput is likely to be slower than using either mode alone.  Likewise,
//applications should avoid mixing mmap(2) of files with direct I/O to the same
//files.

  QIO_HINT_NOREUSE      = QIO_HINT_DIRECT<<1,

  // This disables the 'fast path' for buffered channels; this is for testing.
  QIO_HINT_NOFAST       = QIO_HINT_NOREUSE<<1,

  // We don't own stdin, stdout and stderr, so it would be impolite to close
  // them.  This bit is set only if the qio_file_s is created with a file that
  // is opened within the qio implementation.  Otherwise, the user (or system)
  // has to close it.
  QIO_HINT_OWNED        = QIO_HINT_NOFAST<<1,
};


#define QIO_NUM_HINT_BITS 8
#define QIO_HINTMASK 0xffff00

char* qio_hints_to_string(qio_hint_t hint);
// This is included only when we're actually compiling qio.c.
// It's here so that the names next to the enum and are easy to update.
#ifdef _QIO_C
// returns a malloc'd string.
char* qio_hints_to_string(qio_hint_t hint)
{
  // Not particularly intelligent, but this stack-allocated
  // buffer should have enough room.
  char buf[QIO_NUM_HINT_BITS*20+16*20+100];
  qio_chtype_t type = (qio_chtype_t) (hint & QIO_CHTYPEMASK);
  qio_method_t method = (qio_method_t) (hint & QIO_METHODMASK);
  int ok = 0;

  buf[0] = '\0';
  if( type == QIO_CH_DEFAULT ) {
    strcat(buf, "default_type"); ok = 1;
  } else {
    switch ( type ) {
      case QIO_CH_ALWAYS_UNBUFFERED:
        strcat(buf, "always_unbuffered"); ok = 1;
        break;
      case QIO_CH_ALWAYS_BUFFERED:
        strcat(buf, "always_buffered"); ok = 1;
        break;
      case QIO_CH_BUFFERED:
        strcat(buf, "buffered"); ok = 1;
        break;
      case QIO_CHTYPE_CLOSED:
        strcat(buf, "closed"); ok = 1;
        break;
    }
  }
  if( !ok ) strcat(buf, "unknown_type");
 
  if( method == QIO_METHOD_DEFAULT ) {
    strcat(buf, " default"); ok = 1;
  } else {
    switch ( method ) {
      case QIO_METHOD_READWRITE:
        strcat(buf, " readwrite"); ok = 1;
        break;
      case QIO_METHOD_PREADPWRITE:
        strcat(buf, " preadpwrite"); ok = 1;
        break;
      case QIO_METHOD_FREADFWRITE:
        strcat(buf, " freadfwrite"); ok = 1;
        break;
      case QIO_METHOD_MMAP:
        strcat(buf, " mmap"); ok = 1;
        break;
      case QIO_METHOD_MEMORY:
        strcat(buf, " memory"); ok = 1;
        break;
      // no default to get warned if any are added.
    }
  }
  if( !ok ) strcat(buf, "unknown_method");


  if( hint & QIO_HINT_RANDOM ) strcat(buf, " random");
  if( hint & QIO_HINT_SEQUENTIAL ) strcat(buf, " sequential");
  if( hint & QIO_HINT_LATENCY ) strcat(buf, " latency");
  if( hint & QIO_HINT_BANDWIDTH ) strcat(buf, " bandwidth");
  if( hint & QIO_HINT_CACHED ) strcat(buf, " cached");
  if( hint & QIO_HINT_PARALLEL ) strcat(buf, " parallel");
  if( hint & QIO_HINT_DIRECT ) strcat(buf, " direct");
  if( hint & QIO_HINT_NOREUSE ) strcat(buf, " noreuse");
  if( hint & QIO_HINT_NOFAST ) strcat(buf, " nofast");
  if( hint & QIO_HINT_OWNED ) strcat(buf, " owned");

  return qio_strdup(buf);
}
#endif

typedef struct qio_file_s {
  // reference count which is atomically updated
  qbytes_refcnt_t ref_cnt;

  // these fields are fixed for the life of the object
  // and indicate how the file is backed.
  // We could potentially also support a "virtual file"
  // that handled pread/pwrite by calling some routine,
  // but mostly 
  // An (arguably) better solution is to put 
  FILE* fp; // set if this file wraps a FILE*
  fd_t fd; // -1 if not set
  int use_fp; // we only default to FREADFWRITE if this and fp are set.
  qbuffer_t* buf; // NULL if not set.
                  // if set, fp==NULL, fd==-1, is memory-only file.
                  // Note a qbuffer is not thread-safe, and
                  // so access to this must be protected
                  // by the file's lock.

  //void* fs_info; // Holds the filesystem information (as a user defined struct)
  void* file_info; // Holds the file information (as a user defined struct)

  qio_fdflag_t fdflags;
  bool closed;
  qio_hint_t hints;

  int64_t initial_length;
  int64_t initial_pos;

  qbytes_t* mmap;
  // (note -- data in mmap'd region may change,
  //  but the mapping is fixed for the lifetime of
  //  the file. That's so that no locking is necessary
  //  on the file object itself).
  
  // When writing files with buffered-mmap, we will mmap
  // the file in chunks. As a result, we might need to extend
  // the file to a size larger than the amount of data written
  // to it.
  // When each channel is created, it takes the following steps:
  // lock a file lock
  // file->max_initial_position =
  //    MAX(initial_position, file->max_initial_position)
  // unlock a file lock
  //
  // Then, when each buffered-mmap channel is destroyed,
  //
  // lock a file lock
  // if initial_position == file->max_initial_position
  //    and file length == final mapped position
  //   then truncate the file
  // unlock a file lock
  //
  // Thus the channel truncating the file surely the one
  // that wrote the end part of the file. Other channels writing
  // to a later part of the file must be constructed before they
  // can write. If the constructing happens first, one or both of
  // the checks before truncating will fail. If the truncating
  // happens first, the file will be extended and zero-filled
  // again (by the OS).
  //
  // The locking discipline is that channel locks must
  // always be held before a file lock.
  qio_lock_t lock;
  int64_t max_initial_position;

  qio_style_t style;
} qio_file_t;

typedef qio_file_t* qio_file_ptr_t;
#define QIO_FILE_PTR_NULL NULL
qioerr qio_readv(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_read);
qioerr qio_writev(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, ssize_t* num_written);
qioerr qio_preadv(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, ssize_t* num_read);
qioerr qio_pwritev(qio_file_t* file, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, ssize_t* num_written);

// if fp is not null, fd is ignored; if fp is null, we use fd.
// the QIO file takes ownership of fp or fd, closing it when the QIO file is closed.
qioerr qio_file_init(qio_file_t** file_out, FILE* fp, fd_t fd, qio_hint_t iohints, const qio_style_t* style, int usefilestar);
qioerr qio_file_open(qio_file_t** file_out, const char* path, int flags, mode_t mode, qio_hint_t iohints, const qio_style_t* style);
qioerr qio_file_open_access(qio_file_t** file_out, const char* pathname, const char* access, qio_hint_t iohints, const qio_style_t* style);
qioerr qio_file_open_mem_ext(qio_file_t** file_out, qbuffer_t* buf, qio_fdflag_t fdflags, qio_hint_t iohints, const qio_style_t* style);
qioerr qio_file_open_mem(qio_file_t** file_out, qbuffer_t* buf, const qio_style_t* style);

qioerr qio_file_open_tmp(qio_file_t** file_out, qio_hint_t iohints, const qio_style_t* style);

qioerr qio_file_init_plugin(qio_file_t** file_out, void* file_info, 
                            int fdflags, const qio_style_t* style);

qioerr qio_file_open_access_usr(qio_file_t** file_out, const char* pathname, 
                               const char* access, qio_hint_t iohints, 
                               const qio_style_t* style,
                               void* fs_info);

qioerr qio_get_fs_type(qio_file_t* fl, int* out);
qioerr qio_get_chunk(qio_file_t* fl, int64_t* len_out);
qioerr qio_locales_for_region(qio_file_t* fl, off_t start, off_t end, const char*** locale_names_out, int64_t* num_locs_out);

// This can be called to run close and to check the return value.
// That's important because some implementations (such as NFS)
// actually write data on the close() call, so here's where we'll
// find out if something goes wrong.
//
// Using the file after it is closed will result in undefined
// behavior (hopefully returning an error code, but maybe not).
// Therefore, this method should only be called when we have
// exclusive access to the file. Multiple closes are handled
// correctly by the file lock (ie once the file is closed, it
// stays closed).
//
// Note that this call will release the file descriptor,
// but not memory used by the file structure itself (that
// will be released when the reference count goes to 0).
qioerr qio_file_close(qio_file_t* f);

qioerr qio_file_sync(qio_file_t* f);

// This one gets called automatically.
void _qio_file_destroy(qio_file_t* f);

// returns a newly allocated string in *string_out which must be freed.
qioerr qio_file_path_for_fd(fd_t fd, const char** string_out);
qioerr qio_file_path_for_fp(FILE* fp, const char** string_out);
qioerr qio_file_path(qio_file_t* f, const char** string_out);

static inline
void qio_file_retain(qio_file_t* f) {
  DO_RETAIN(f);
}

static inline
void qio_file_release(qio_file_t* f) {
  DO_RELEASE(f, _qio_file_destroy);
}

static inline
qioerr qio_file_lock(qio_file_t* f)
{
  return qio_lock(&f->lock);
}

static inline
void qio_file_unlock(qio_file_t* f)
{
  qio_unlock(&f->lock);
}

static inline
void qio_file_get_style(qio_file_t* f, qio_style_t* style)
{
  *style = f->style;
}

static inline
bool qio_file_isopen(qio_file_t* f) 
{
  return !(f->closed);
}

static inline
void* qio_file_get_plugin(qio_file_t* f) {
  return f->file_info;
}

// Return the current length of a file.
// Calls stat for a file descriptor
// Calls fflush on a FILE* first.
qioerr qio_file_length(qio_file_t* f, int64_t *len_out);

/* CHANNELS ..... */

/* A Read and Write Buffered channels support:
 *  -- int require(int64_t n)
 *      -- a hint - make sure that n bytes are available in the buffer
 *  // not thread-happy. -- int advance(int64_t n)
 *      -- move ahead some number of bytes; if writing, we'll
 *         write whatever's already in the buffer (possibly set with
 *         peek_cached/peek_buffer).
 * -- int offset(int64_t* offset)
 *      -- return the current offset in the file.
 *
 * -- mark,revert,commit
 *
 * -- begin_peek_cached(), end_peek_cached(amt to advance)
 *      -- hold the lock on the buffer and use the cached buffer.
 *         no other buffer methods can be called between these
 *         two. This implements a 'common case' fast path,
 *         while begin_peek_buffer() and end_peek_buffer() are the
 *         general case.
 *
 * -- begin_peek_buffer(), end_peek_buffer(amt to advance)
 *      -- like begin_peek_cached() and end_peek_cached() but
 *         returns a current view to the internal buffer for a
 *         buffered channel. No other calls to channel methods
 *         can take place between these two. This buffer can be
 *         modified by the application. This implements the slow-path
 *         equivalent of begin_peek_cached() and end_peek_cached().
 *         Since the buffer consists of 'bytes' objects, these objects
 *         can be retained (e.g. with qbuffer_clone) and kept outside of
 *         begin_peek_buffer() and end_peek_buffer(),
 *         but if that happens, the application may contain data races
 *         with the data, or modify the data while it is being written
 *         to a file, for example. So use that feature at your own risk.
 *
 * A Read channel (buffered or no) supports:
 * -- read(void* ret, int64_t sz)
 *      -- copy data out of channel. returns EEOF if returning a short
 *         amount of data because of EOF.
 *
 * A Write channel (buffered or no) supports:
 * -- write(void* in, int64_t sz)
 *      -- copy data into the channel
 *
 * -- put_buffer(qbuffer_t* buffer, start, end)
 *      -- give some data to the channel. The application may
 *         no longer modify this data. It should be removed
 *         from the argument buffer.
 *
 * -- put_bytes(qbytes_t* bytes, skip, len)
 *      -- give some data to the channel. The application may
 *         no longer modify this data. The channel will increase
 *         the reference count of bytes.
 *
 * -- flush()
 *      -- write data in any user-space buffers to disk
 *         (use sys_fsync to guarantee data is on disk).
 *
 * FUTURE
 * -- readahead()
 *      -- system readahead in background
 * -- splice(output_channels[], int64_t len)
 *      -- copy an amount of data to some output channels
 * -- tee(output_channels[], int64_t len)

 */

/* A channel always goes sequentially through a file
 * it is not possible to seek() a channel
 * (instead, just create a new channel starting at the new
 *  position).
 *
 *  If both the channel lock and the file lock are needed, always
 *  lock the channel first.
 *
 *  Channels have cached start/cur/end pointers
 *  to specify the region in which it's easy to read/write
 *  a chunk. The channel offset is computed as
 *  (cached_cur - cached_start) + cached_start_pos
 */
#define MARK_INITIAL_STACK_SZ 4

typedef uint64_t qio_bitbuffer_t;
#define qio_bitbuffer_tobe(x) (htobe64(x))
#define qio_bitbuffer_unbe(x) (be64toh(x))
// intel right shift is mod 64, so we have this macro
// for a right shift that zeros if we want 0 bits left...
#define qio_bitbuffer_topn(x,amt) ( ((amt) != 0)?((x) >> (8*sizeof(qio_bitbuffer_t) - (amt))):(0) )

typedef struct qio_channel_s {
  // reference count which is atomically updated
  qbytes_refcnt_t ref_cnt;

  // frequently used data:
  void* cached_cur; // current offset in a buffer.
  void* cached_end; // end offset in a buffer; we don't exceed this.
  void* cached_start; // start offset in a buffer; when mark/commit/reverting we
                      // might back up to this point.
  int64_t cached_start_pos; // what position does cached_start correspond to?

  // sticky channel error. Channel I/O functions generally return
  // an error code, which if set, will also be stored here.
  // This error code can only be cleared by qio_channel_clear_error().
  // This error code can be checked with qio_channel_error().
  // Operations on the channel (besides qio_channel_error()) do not
  // check this error code and only write to it if there is an error.
  // Thus, the behavior is similar to the global errno.
  qioerr error;

  qio_file_t* file;
  void* chan_info; // plugin
  qio_lock_t lock;

  // less frequently used.
  int64_t start_pos; // what is the initial position of the channel?
                     // this is used in a thread-safe truncation
                     // scheme for mmap channels.
  int64_t end_pos; // we should not write at or beyond this position.
                   // use INT64_MAX for no limit.
  qio_hint_t hints;
  qio_fdflag_t flags;

  // buffered channel materials.
  /* When reading, we 'require' then read from
   * right_mark_start to (potentially) heavy->av_end
   * and then move right_mark_start forward
   * (that is in qio_buffered_read)
   *
   * When writing, we 'require' then write to 
   * right_mark_start to (potentially) end_iter(heavy->buf)
   * and then move right_mark_start forward
   * (that is in qio_buffered_write)
   *
   * When 'requiring', with MMAP, buffered_get_mmap
   * we put mmap'd data into buffer and update av_end to end of buffer.
   *
   * When 'requiring', with MEMORY, buffered_get_memory
   * we but file->buffer data into buffer, update av_end to end of buffer.
   *
   * When 'requiring', with pread, buffered_read_atleast
   * we allocate bufferspace (appending without updating av_end)
   * and then read, updating av_end to point to 1st not-yet-read byte.
   *
   * When 'requiring', with pwrite, buffered_makespace_atleast
   * we allocate bufferspace (appending without updating av_end)
   * and then update av_end to be the end of the buffer.
   *
   * right_mark_start is mark_space[mark_next-1] or av_start if mark_next==0.
   *
   * How does marking interact with cached_cur/start/end?
   *  - mark positions are always offsets from get_offset which includes cached_cur.
   *  - commit/revert functions use differences
   */

  /* Each buffer is divided into these sections:
   * _________________________________________________________________
   * |write-behind | user writeable/readable | read-ahead/buffer space|
   * |             | aka available           | aka allocated          |
   *             mark_stack[0]              av_end
   *             "av_start"
   *                  mark_stack[mark_next-1] 
   *                  "right_mark_start"
   * the available section is ready for user read/write.
   * Space to the right of av_end is allocated but not yet read from disk
   * (if we're reading). When writing, av_end == end of buf,
   * and so there is no extra space on the right. If one day
   * we do read-modify-write (which isn't supported in the initial version),
   * there might be between av_end and the end of the buffer
   * even when writing... but it might be easier to do read-modify-write
   * by coupling together 2 channels with the zero-copy capabilities.
   */

  int64_t av_end;

  qbuffer_t buf;

  // For reading/writing bits (ie less than a byte) at a time
  qio_bitbuffer_t bit_buffer;
  void* cached_end_bits; // cause flush before byte I/O
                         // once bit-I/O is used.
  int8_t bit_buffer_bits;
  int8_t bits_read_bytes; // how many bytes do we add
                          // to cached_cur when we move on to the next read?

  ssize_t mark_cur;
  ssize_t mark_stack_size;
  int64_t* mark_stack;
  // initial few entries so we don't have to malloc
  // for the common case of very few marks.
  int64_t mark_space[MARK_INITIAL_STACK_SZ];

  qio_style_t style;
} qio_channel_t;


typedef qio_channel_t* qio_channel_ptr_t;
#define QIO_CHANNEL_PTR_NULL NULL

void _qio_channel_destroy(qio_channel_t* ch);

static inline
void qio_channel_retain(qio_channel_t* ch) {
  DO_RETAIN(ch);
  //
  // Commenting this out because (a) PRIu64 breaks the build for the
  // Cray C++ compiler and (b) it's debugging code, off by default.
  //
  //  if (ch && DEBUG_QIO)
  //    fprintf(stdout, "Channel retain %p, ref_cnt = %" PRIu64 "\n", ch, DO_GET_REFCNT(ch));
}

static inline
void qio_channel_release(qio_channel_t* ch) {
  //
  // Commenting this out because (a) PRIu64 breaks the build for the
  // Cray C++ compiler and (b) it's debugging code, off by default.
  //
  //  if (ch && DEBUG_QIO)
  //    fprintf(stdout, "Channel release %p, ref_cnt = %" PRIu64 "\n", ch, DO_GET_REFCNT(ch));
  DO_RELEASE(ch, _qio_channel_destroy);
}

static inline
void qio_channel_clear_error(qio_channel_t* ch) {
  ch->error = 0;
}

static inline
void _qio_channel_set_error_unlocked(qio_channel_t* ch, qioerr err) {
  if( err ) ch->error = err;
}


static inline
qioerr qio_channel_error(qio_channel_t* ch) {
  return ch->error;
}

static inline
void* qio_channel_get_plugin(qio_channel_t* ch) {
  return ch->chan_info;
}

qioerr _qio_channel_init_buffered(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style);
qioerr _qio_channel_init_file(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style);


// maybe want to use INT64_MAX for end if it's not to be restricted.
qioerr qio_channel_create(qio_channel_t** ch_out, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style);


qioerr qio_relative_path(const char** path_out, const char* cwd, const char* path);
qioerr qio_shortest_path(qio_file_t* file, const char** path_out, const char* path_in);

qioerr qio_channel_path_offset(const int threadsafe, qio_channel_t* ch, const char** string_out, int64_t* offset_out);

qioerr _qio_slow_read(qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* amt_read);
qioerr _qio_slow_write(qio_channel_t* ch, const void* ptr, ssize_t len, ssize_t* amt_written);

// Flush any QIO buffers (does not flush a FILE* if there is one)
qioerr _qio_channel_flush_qio_unlocked(qio_channel_t* ch);
// Flush any QIO buffers and flush a FILE* if there is one
qioerr _qio_channel_flush_unlocked(qio_channel_t* ch);

// Returns an error code... but it always returns 0
// because the data will already be in the buffer.
// The actual error in flushing, if there was one,
// will be returned in a qio_channel_flush
static inline
qioerr _qio_channel_post_cached_write(qio_channel_t* restrict ch)
{
  qioerr err = 0;
  // Flush FILE* buffers after every write, so that C I/O
  // can be intermixed with QIO calls.
  if( (ch->hints & QIO_METHODMASK) == QIO_METHOD_FREADFWRITE) {
    if( qbuffer_is_initialized(&ch->buf) ) {
      if( ch->mark_cur == 0 ) {
        err = _qio_channel_flush_qio_unlocked(ch);
        _qio_channel_set_error_unlocked(ch, err);
      }
    }
  }

  // see note above -- never returns an error.
  return 0;
}

// qio_channel_read does not handle text (since we don't know
// when to stop reading) - only fixed-size elements.
// Returns an error code.
//
// On a read, returns EEOF and *amt_read=some amount for EOF.
static inline
qioerr qio_channel_read(const int threadsafe, qio_channel_t* restrict ch, void* restrict ptr, ssize_t len, ssize_t* restrict amt_read)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *amt_read = 0;
      return err;
    }
  }

  // Is there room in our fast path buffer?
  if( qio_space_in_ptr_diff(len, ch->cached_end, ch->cached_cur) ) {
    qio_memcpy( ptr, ch->cached_cur, len );
    ch->cached_cur = qio_ptr_add(ch->cached_cur, len);
    *amt_read = len;
    err = 0;
  } else {
    err = _qio_slow_read(ch, ptr, len, amt_read);
    _qio_channel_set_error_unlocked(ch, err);
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// returns a character >= 0 or negative for an error code.
// This function exists for performance reasons.
static inline
int32_t qio_channel_read_byte(const int threadsafe, qio_channel_t* restrict ch)
{
  int32_t ret;

  if( threadsafe ) {
    qioerr err;
    err_t errcode;
    err = qio_lock(&ch->lock);
    errcode = qio_err_to_int(err);
    if( errcode ) {
      ret = errcode < 0 ? errcode : - errcode;
      return ret;
    }
  }

  // Is there room in our fast path buffer?
  if( qio_space_in_ptr_diff(1, ch->cached_end, ch->cached_cur) ) {
    ret = *(unsigned char*) ch->cached_cur;
    ch->cached_cur = qio_ptr_add(ch->cached_cur, 1);
  } else {
    ssize_t amt_read;
    qioerr err;
    err_t errcode;
    uint8_t tmp;
    err = _qio_slow_read(ch, &tmp, 1, &amt_read);
    if( err == 0 && amt_read != 1 ) err = QIO_ESHORT;
    if( err == 0 ) ret = tmp;
    else {
      _qio_channel_set_error_unlocked(ch, err);
      errcode = qio_err_to_int(err);
      // Return an error, but make sure that it is negative.
      ret = errcode < 0 ? errcode : - errcode;
    }
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return ret;
}

// This function exists for performance reasons.
static inline
qioerr qio_channel_write_byte(const int threadsafe, qio_channel_t* restrict ch, uint8_t byte) {
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  //printf("IN WRITE BYTE %x %p %p %p\n", (int) byte, ch->cached_cur, ch->cached_end, ch->cached_end_bits);

  // Is there room in our fast path buffer?
  if( qio_space_in_ptr_diff(1, ch->cached_end, ch->cached_cur) ) {
    *(unsigned char*) ch->cached_cur = byte;
    ch->cached_cur = qio_ptr_add(ch->cached_cur, 1);
    err = _qio_channel_post_cached_write(ch);
  } else {
    ssize_t amt_written = 0;
    uint8_t tmp = byte;
    err = _qio_slow_write(ch, &tmp, 1, &amt_written);
    if( err == 0 && amt_written != 1 ) err = QIO_ESHORT;
    _qio_channel_set_error_unlocked(ch, err);
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
qioerr qio_channel_lock(qio_channel_t* ch)
{
  assert( ch != NULL );
  return qio_lock(&ch->lock);
}

static inline
void qio_channel_unlock(qio_channel_t* ch)
{
  qio_unlock(&ch->lock);
}

static inline
qio_file_t* qio_channel_get_file(qio_channel_t* ch)
{
  return ch->file;
}

// You should lock/ get ptr/ unlock
static inline
qio_style_t* qio_channel_style_ptr(qio_channel_t* ch)
{
  return &ch->style;
}
static inline
void qio_channel_get_style(qio_channel_t* ch, qio_style_t* style)
{
  *style = ch->style;
}
static inline
void qio_channel_set_style(qio_channel_t* ch, qio_style_t* style)
{
  ch->style = *style;
}
static inline
uint8_t qio_channel_binary(qio_channel_t* ch)
{
  return ch->style.binary;
}
static inline
uint8_t qio_channel_byteorder(qio_channel_t* ch)
{
  return ch->style.byteorder;
}
static inline
int64_t qio_channel_str_style(qio_channel_t* ch)
{
  return ch->style.str_style;
}

int64_t qio_channel_style_element(qio_channel_t* ch, int64_t element);
 
static inline
qioerr qio_channel_write(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, ssize_t len, ssize_t* restrict amt_written )
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *amt_written = 0;
      return err;
    }
  }

  // Is there room in our fast path buffer?
  if( qio_space_in_ptr_diff(len, ch->cached_end, ch->cached_cur) ) {
    qio_memcpy( ch->cached_cur, ptr, len );
    ch->cached_cur = qio_ptr_add(ch->cached_cur, len);
    *amt_written = len;
    err = _qio_channel_post_cached_write(ch);
  } else {
    err = _qio_slow_write(ch, ptr, len, amt_written);
    _qio_channel_set_error_unlocked(ch, err);
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}


static inline
qioerr qio_channel_read_amt(const int threadsafe, qio_channel_t* restrict ch, void* restrict ptr, ssize_t len) {
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  // Is there room in our fast path buffer?
  if( qio_space_in_ptr_diff(len, ch->cached_end, ch->cached_cur) ) {
    qio_memcpy( ptr, ch->cached_cur, len );
    ch->cached_cur = qio_ptr_add(ch->cached_cur, len);
    err = 0;
  } else {
    ssize_t amt_read = 0;
    err = _qio_slow_read(ch, ptr, len, &amt_read);
    if( err == 0 && amt_read != len ) err = QIO_ESHORT;
    _qio_channel_set_error_unlocked(ch, err);
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
qioerr qio_channel_write_amt(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, ssize_t len) {
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  // Is there room in our fast path buffer?
  if( qio_space_in_ptr_diff(len, ch->cached_end, ch->cached_cur) ) {
    qio_memcpy( ch->cached_cur, ptr, len );
    ch->cached_cur = qio_ptr_add(ch->cached_cur, len);
    err = _qio_channel_post_cached_write(ch);
  } else {
    ssize_t amt_written = 0;
    err = _qio_slow_write(ch, ptr, len, &amt_written);
    if( err == 0 && amt_written != len ) err = QIO_ESHORT;
    _qio_channel_set_error_unlocked(ch, err);
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr _qio_channel_require_unlocked(qio_channel_t* ch, int64_t space, int writing);

static inline
qioerr qio_channel_require_read(const int threadsafe, qio_channel_t* ch, int64_t space)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = _qio_channel_require_unlocked(ch, space, 0);
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
qioerr qio_channel_require_write(const int threadsafe, qio_channel_t* ch, int64_t space)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = _qio_channel_require_unlocked(ch, space, 1);
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// returns the number of bytes allocated - that is, the number
// of bytes from av_end to the end of the buffer.
int64_t qio_channel_nbytes_allocated_unlocked(qio_channel_t* ch);
// returns the number of bytes "available", that is, number of bytes
// from right_mark_start (aka current offset) to av_end.
int64_t qio_channel_nbytes_available_unlocked(qio_channel_t* ch);
// returns the number of bytes ready for write-behind, that is,
// the number of bytes from the start of the buffer to the av_start 
int64_t qio_channel_nbytes_write_behind_unlocked(qio_channel_t* ch);

// Returns a pointer,length for the next contiguous sequence in the
// allocated region, as well as the offset to start a read at.
// Ensures that amt_requested buffer space is available, first.
// (useful for QIO plugins when handling a 'read' I/O operation)
// TODO: rename to get-read-ahead-ptr-unlocked
qioerr qio_channel_get_allocated_ptr_unlocked(qio_channel_t* ch, int64_t amt_requested, void** ptr_out, ssize_t* len_out, int64_t* offset_out);
// Move the end of the available region forward len bytes
void qio_channel_advance_available_end_unlocked(qio_channel_t* ch, ssize_t len);

// Copies the len bytes starting at ptr to the channel
// allocating buffer space if necessary in the channel.
// (useful for QIO plugins when handling a 'read' I/O operation)
qioerr qio_channel_copy_to_available_unlocked(qio_channel_t* ch, void* ptr, ssize_t len);

// Returns a pointer,length for the next contiguous sequence in the
// write-behind region, as well as the offset to start a write at.
// (useful for QIO plugins when handling a 'write' I/O operation)
qioerr qio_channel_get_write_behind_ptr_unlocked(qio_channel_t* ch, void** ptr_out, ssize_t* len_out, int64_t* offset_out);

// Move the write-behind region forward len bytes
void qio_channel_advance_write_behind_unlocked(qio_channel_t* ch, ssize_t len);

// Copies from the buffer start len bytes into ptr
// (useful for QIO plugins when handling a 'write' I/O operation)
// TODO: rename to e.g. write_behind_to_pointer
qioerr qio_channel_copy_from_buffered_unlocked(qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* n_written_out);

void _qio_buffered_setup_cached(qio_channel_t* ch);
void _qio_buffered_advance_cached(qio_channel_t* ch);

// you don't have to call end_peek_cached if this returns an error
static inline
qioerr qio_channel_begin_peek_cached(const int threadsafe, qio_channel_t* ch, void** start_out, void** end_out)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *start_out = NULL;
      *end_out = NULL;
      return err;
    }
  }

  if( ! ch->cached_end ) _qio_buffered_setup_cached(ch);

  *start_out = ch->cached_cur;
  *end_out = ch->cached_end;
  err = 0;
  _qio_channel_set_error_unlocked(ch, err);

  return err;
}

static inline
qioerr qio_channel_end_peek_cached(const int threadsafe, qio_channel_t* ch, void* new_start)
{
  qioerr err;

  ch->cached_cur = new_start;
  err = 0;
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_advance_past_byte(const int threadsafe, qio_channel_t* ch, int byte);

qioerr qio_channel_begin_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t require, int writing, qbuffer_t** buf_out, qbuffer_iter_t* start_out, qbuffer_iter_t* end_out);

qioerr qio_channel_end_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t advance);

static inline
qioerr qio_channel_isbuffered(const int threadsafe, qio_channel_t* ch, char* isbuffered)
{
  *isbuffered = 1;
  return 0;
}


static inline
bool qio_channel_writable(qio_channel_t* ch) {
  return (ch->flags & QIO_FDFLAG_WRITEABLE) > 0;
}

qioerr qio_channel_offset(const int threadsafe, qio_channel_t* ch, int64_t* offset_out);

static inline
int64_t qio_channel_offset_unlocked(qio_channel_t* ch)
{
  int64_t cached_amt = qio_ptr_diff(ch->cached_cur, ch->cached_start);
  int writing = ch->flags & QIO_FDFLAG_WRITEABLE;
  int bytes_in_bits = 0;

  if( writing ) {
    bytes_in_bits = (ch->bit_buffer_bits + 7)/8;
  } else {
    bytes_in_bits = (7 + 8*ch->bits_read_bytes - ch->bit_buffer_bits) / 8;
  }

  if( ch->cached_start != NULL ) {
    //printf("offset %i %i %i\n", (int) bytes_in_bits, (int) cached_amt, (int) ch->cached_start_pos);
    return bytes_in_bits + cached_amt + ch->cached_start_pos;
  }

  //printf("offset %i - %i\n", (int) bytes_in_bits, (int) ch->mark_stack[ch->mark_cur]);
  // and if there's no cached data, cached_start_pos is not available:
  return bytes_in_bits + ch->mark_stack[ch->mark_cur]; // _right_mark_start(ch);
}

/* 
 * Returns the end position of the channel.
 *  - If the channel is unbounded and we have not
 *    yet encountered an EOF when reading, returns MAX_INT64
 *  - If the channel is bounded, returns the end offset
 *  - If an unbounded channel has encountered EOF, returns
 *    the position at which the EOF was encountered.
 */
static inline
int64_t qio_channel_end_offset_unlocked(qio_channel_t* ch)
{
  return ch->end_pos;
}

qioerr qio_channel_end_offset(const int threadsafe, qio_channel_t* ch, int64_t* offset_out);


qioerr qio_channel_advance(const int threadsafe, qio_channel_t* ch, int64_t nbytes);

qioerr qio_channel_put_bytes(const int threadsafe, qio_channel_t* ch, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes);

qioerr qio_channel_put_buffer(const int threadsafe, qio_channel_t* ch, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end);


static inline
qioerr qio_channel_flush(const int threadsafe, qio_channel_t* ch)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = _qio_channel_flush_unlocked(ch);
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr _qio_channel_final_flush_unlocked(qio_channel_t* ch);


// Flushes the channel for a final time. The channel cannot
// be used after this call.
//
// Using the channel after it is closed will result in undefined
// behavior (hopefully returning an error code, but maybe not).
// Therefore, this method should only be called when we have
// exclusive access to the channel. Multiple closes are handled
// correctly by the channel lock (ie once the channel is closed, it
// stays closed).
//
// Note that this call will flush the channel and possibly
// mark it closed, but it will not free memory used by the
// channel structure itself (that will be released when the
// reference count goes to 0).
static inline
qioerr qio_channel_close(const int threadsafe, qio_channel_t* ch)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = _qio_channel_final_flush_unlocked(ch);
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// Returns true for ch=NULL channel if ch has been closed
// (but not yet deallocated).
static inline
bool qio_channel_isclosed(const int threadsafe, qio_channel_t* ch)
{
  bool ret;

  if( ch == NULL ) return true;

  if( threadsafe ) {
    qio_lock(&ch->lock);
  }

  ret = false;
  {
    qio_chtype_t type = (qio_chtype_t) (ch->hints & QIO_CHTYPEMASK);
    if( type == QIO_CHTYPE_CLOSED ) ret = true;
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return ret;
}

qioerr qio_channel_mark(const int threadsafe, qio_channel_t* ch);
qioerr qio_channel_mark_maybe_flush_bits(const int threadsafe, qio_channel_t* ch, int flushbits);

// Returns an error, but that error can safely be ignored.
// also, if nbytes<0, it works as though nbytes = 0.
qioerr qio_channel_advance_unlocked(qio_channel_t* ch, int64_t nbytes);

void qio_channel_revert_unlocked(qio_channel_t* ch);

static inline
bool qio_channel_has_mark_unlocked(qio_channel_t* ch) {
  return (ch->mark_cur >= 1);
}

// Only returns threading errors; guaranteed not to return
// an error if threadsafe == false (it asserts instead on bad cases)
static inline
qioerr qio_channel_revert(const int threadsafe, qio_channel_t* ch)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }
  
  qio_channel_revert_unlocked(ch);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return 0;
}

qioerr qio_channel_seek(qio_channel_t* ch, int64_t start, int64_t end);

void qio_channel_commit_unlocked(qio_channel_t* ch);

// Only returns threading errors.
// Generally, if the data is already in the buffer, we postpone the
// error until later...
static inline
qioerr qio_channel_commit(const int threadsafe, qio_channel_t* ch)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }
  
  qio_channel_commit_unlocked(ch);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return 0;
}

/* Handle I/O of bits at a time */
qioerr _qio_channel_write_bits_slow(qio_channel_t* restrict ch, uint64_t v, int8_t nbits);
void _qio_channel_write_bits_cached_realign(qio_channel_t* restrict ch, uint64_t v, int8_t nbits);

static inline
qioerr qio_channel_write_bits(const int threadsafe, qio_channel_t* restrict ch, uint64_t v, int8_t nbits) {
  qioerr err = 0;
  qio_bitbuffer_t part_one_bits;
  qio_bitbuffer_t part_one_bits_be;
  qio_bitbuffer_t tmp_bits;
  uint64_t tmpv;
  int tmp_live;
  int part_one;
  int part_two;

  if( nbits < 0 ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative number of bits");
  if( nbits == 0 ) return 0;
  if( nbits < 64 && (v >> nbits) != 0 ) {
    // v must not have any extra bits set.
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "no more bits");
  }
  
  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  //printf("IN WRITE BITS %llx %i %p %p\n", (long long int) v, nbits, ch->cached_end, ch->cached_end_bits);

  //printf("cur %p end_bits %p end %p\n", ch->cached_cur, ch->cached_end_bits, ch->cached_end);

  tmp_live = ch->bit_buffer_bits;
  tmp_bits = ch->bit_buffer;

  if( qio_space_in_ptr_diff(sizeof(uint64_t)+2*sizeof(qio_bitbuffer_t),
                            ch->cached_end_bits, ch->cached_cur) ) {
    //printf("WRITE BITS CACHED WRITING %llx %i %llx %i\n", (long long int) tmp_bits, tmp_live, (long long int) v, nbits);

    // We have buffer for it...
    // Can we just put it into bitbuffer?
    if( tmp_live + nbits <= (int) (8*sizeof(qio_bitbuffer_t)) ) {
      //printf("WRITE BITS LOCAL WRITING %llx %i %llx %i\n", (long long int) tmp_bits, tmp_live, (long long int) v, nbits);
      tmp_bits = (tmp_bits << nbits) | v;
      tmp_live += nbits;
      ch->bit_buffer = tmp_bits;
      ch->bit_buffer_bits = tmp_live;
    } else {
      // We've got > 64 bits to write.
      // The value is split between tmp_bits and next_bits
      part_one = 8*sizeof(qio_bitbuffer_t) - tmp_live;
      part_two = nbits - part_one;
      tmpv = (part_two < 64) ? v : 0;
      part_one_bits = (tmp_bits << part_one) | ( tmpv >> part_two );
      part_one_bits_be = qio_bitbuffer_tobe(part_one_bits); // big endian now.
      tmp_bits = v;
      tmp_live = part_two;

      //printf("WRITE BITS CACHED PARTONE %llx tmp %llx %i\n", (long long int) part_one_bits, (long long int) tmp_bits, tmp_live);
      // If we are 8-byte aligned, write part_one_bits and
      // carry on.
      if( qio_ptr_align(ch->cached_cur, sizeof(qio_bitbuffer_t)) == 0 ) {
        // We have 8-byte alignment
        memcpy(ch->cached_cur, &part_one_bits_be, sizeof(qio_bitbuffer_t));
        ch->cached_cur = qio_ptr_add(ch->cached_cur, sizeof(qio_bitbuffer_t));
        ch->bit_buffer = tmp_bits;
        ch->bit_buffer_bits = tmp_live;
        //printf("WRITE BITS ALIGNED WRITING %llx\n", (long long int) part_one_bits);
      } else {
        //printf("WRITE BITS REALIGN\n");
        // Otherwise... write some so that the next write will
        // be 8-byte aligned.
        _qio_channel_write_bits_cached_realign(ch, v, nbits);
       }
    }
  } else {
   //printf("WRITE BITS SLOW WRITING %llx %i %llx %i\n", (long long int) tmp_bits, tmp_live, (long long int) v, nbits);
   err = _qio_channel_write_bits_slow(ch, v, nbits);
   _qio_channel_set_error_unlocked(ch, err);
   goto unlock;
  }

unlock:

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// Puts zeros at the end of any partial byte and writes it to the buffer.
qioerr qio_channel_flush_bits(const int threadsafe, qio_channel_t* restrict ch);

qioerr _qio_channel_read_bits_slow(qio_channel_t* restrict ch, uint64_t* restrict v, int8_t nbits);
void _qio_channel_read_bits_cached_realign(qio_channel_t* restrict ch, uint64_t* restrict v, int8_t nbits);

static inline
qioerr qio_channel_read_bits(const int threadsafe, qio_channel_t* restrict ch, uint64_t* restrict v, int8_t nbits) {
  qioerr err = 0;
  qio_bitbuffer_t part_two_bits;
  qio_bitbuffer_t tmp_bits;
  qio_bitbuffer_t value;
  int tmp_live;
  int part_two;

  if( nbits < 0 ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative number of bits");
  if( nbits == 0 ) return 0;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  //printf("IN READ BITS nbits=%i cached_end=%p cached_end_bits=%p\n", nbits, ch->cached_end, ch->cached_end_bits);

  //printf("cur %p end_bits %p end %p\n", ch->cached_cur, ch->cached_end_bits, ch->cached_end);

  tmp_live = ch->bit_buffer_bits;
  tmp_bits = ch->bit_buffer;

  if( qio_space_in_ptr_diff( sizeof(uint64_t)+2*sizeof(qio_bitbuffer_t),
                             ch->cached_end_bits, ch->cached_cur )) {
    //printf("READ BITS CACHED %llx %i\n", (long long int) tmp_bits, (int) tmp_live);

    // We have buffer for it...
    // Can we just read it from the bitbuffer?
    if( nbits <= tmp_live ) {
      //printf("READ BITS LOCAL\n");
      *v = tmp_bits >> (8*sizeof(qio_bitbuffer_t) - nbits);
      tmp_bits <<= nbits;
      tmp_live -= nbits;
      ch->bit_buffer = tmp_bits;
      ch->bit_buffer_bits = tmp_live;
    } else {
      // We've got to read across two words.
      // The value is split between tmp_bits and next_bits

      part_two = nbits - tmp_live;

      // Add some value to cached_cur
      ch->cached_cur = qio_ptr_add(ch->cached_cur, ch->bits_read_bytes);

      // If we are 8-byte aligned, read part_two_bits and carry on.
      if( qio_ptr_align(ch->cached_cur, sizeof(qio_bitbuffer_t)) == 0 ) {
        //printf("READ BITS CACHED ALIGNED\n");
        // We have 8-byte alignment
        // Read the next word.
        part_two_bits = *(qio_bitbuffer_t*)ch->cached_cur;
        part_two_bits = qio_bitbuffer_unbe(part_two_bits); // host endian now.
        // now we need tmp_live top bits from tmp_bits
        // and the rest from part_two_bits.
        
        // value we have now in bottom bits.
        value = qio_bitbuffer_topn(tmp_bits, tmp_live);

        // Now, from part_two_bits, read from the top to get
        // our number...

        value <<= part_two;
        value |= qio_bitbuffer_topn(part_two_bits,part_two);

        *v = value;
        ch->bit_buffer = (part_two_bits << part_two);
        ch->bit_buffer_bits = 8*sizeof(qio_bitbuffer_t) - part_two;
        ch->bits_read_bytes = sizeof(qio_bitbuffer_t);
      } else {
        //printf("READ BITS CACHED REALIGN\n");
        // Otherwise... read some so that the next read will
        // be 8-byte aligned.
        _qio_channel_read_bits_cached_realign(ch, v, nbits);
       }
    }
  } else {
   //printf("READ BITS SLOW %llx %i\n", (long long int) tmp_bits, (int) tmp_live);
   err = _qio_channel_read_bits_slow(ch, v, nbits);
   _qio_channel_set_error_unlocked(ch, err);
   goto unlock;
  }

unlock:

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
