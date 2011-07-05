#ifndef _QIO_H_
#define _QIO_H_

#include "qbuffer.h"
#include "sys.h"
#include "qio_style.h"

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

#define DEBUG_QIO 0

typedef enum {
  QIO_FDFLAG_UNK  = 1,
  QIO_FDFLAG_READABLE = 2,
  QIO_FDFLAG_WRITEABLE = 4,
  QIO_FDFLAG_SEEKABLE = 8,
  //QIO_FDFLAG_CLOSED = 16, // means channel/file was closed.
} qio_fdflag_t;

typedef uint32_t qio_hint_t;

// The qio lock must be re-entrant in order to handle
// e.g. qio_printf, which has will lock the lock, then
// call printf, which will call cookie write, and lock the lock again.
// UPDATE - not implementing qio_printf anymore in terms of FILE* printf,
// but maybe we will in the future support FILE*<->channel..
// UPDATE 2 -- we need reentrant locks for Chapel because
// write(a,b,c) might call
//    a.writeThis() which calls
//        write(16) for example

#ifdef _chplrt_H_
// also export iohint_t and fdflag_t
typedef qio_hint_t iohint_t;
typedef qio_fdflag_t fdflag_t;


// make a re-entrant lock.
typedef struct {
  chpl_sync_aux_t sv;
  int64_t owner; // task ID of owner.
  uint64_t count; // how many times owner has locked.
} qio_lock_t;

#define NULL_OWNER chpl_nullTaskID

static inline err_t qio_lock(qio_lock_t* x) {
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
static inline void qio_unlock(qio_lock_t* x) {
  int64_t id = chpl_task_getId();

  // recursive mutex based on glibc pthreads implementation
  if( x->owner != id ) {
    assert(0);
    abort();
  }

  if (--x->count != 0 ) {
    // we still hold the mutex.
    return;
  }

  x->owner = NULL_OWNER;
  chpl_sync_unlock(&x->sv);
}

static inline err_t qio_lock_init(qio_lock_t* x) {
  x->owner = NULL_OWNER;
  x->count = 0;
  chpl_sync_initAux(&x->sv);
  return 0;
}

static inline void qio_lock_destroy(qio_lock_t* x) {
  chpl_sync_destroyAux(&x->sv);
}

#else
typedef pthread_mutex_t qio_lock_t;
// these should return 0 on success; otherwise, an error number.
static inline err_t qio_lock(qio_lock_t* x) { return pthread_mutex_lock(x); }

// qio_unlock does not return an error because it can only usefully
// return the error that the mutex isn't owned by this process
// (or possibly that it's invalid), both of which are programming errors
// and not anything that can reasonably be responded to.
static inline void qio_unlock(qio_lock_t* x) { int rc = pthread_mutex_unlock(x); if( rc ) { assert(rc == 0); abort(); } }

static inline err_t qio_lock_init(qio_lock_t* x) {
  pthread_mutexattr_t attr;
  err_t err, newerr;

  err = pthread_mutexattr_init(&attr);
  if( err ) return err;
  err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  if( ! err ) {
    err = pthread_mutex_init(x, &attr);
  }

  newerr = pthread_mutexattr_destroy(&attr);
  if( ! err ) err = newerr;

  return err;
}

// returns void for the same reason as qio_unlock.
static inline void qio_lock_destroy(qio_lock_t* x) { int rc = pthread_mutex_destroy(x); if( rc ) { assert(rc == 0); abort(); } }
#endif


extern ssize_t qio_too_small_for_default_mmap;
extern ssize_t qio_too_large_for_default_mmap;
extern ssize_t qio_mmap_chunk_iobufs;

/* Wrap system calls readv, writev, preadv, pwritev
 * to take a buffer.
 */
err_t qio_readv(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_read);
err_t qio_writev(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_written);
err_t qio_preadv(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, int64_t* num_read);
err_t qio_pwritev(fd_t fd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t seek_to_offset, int64_t* num_written);

err_t qio_freadv(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_read);
err_t qio_fwritev(FILE* fp, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, int64_t* num_written);


err_t qio_recv(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end,
              int flags,
              sys_sockaddr_t* src_addr_out, /* can be NULL */
              void* ancillary_out, socklen_t* ancillary_len_inout, /* can be NULL */
              int64_t* num_recvd_out);

err_t qio_send(fd_t sockfd, qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end,
              int flags,
              const sys_sockaddr_t* dst_addr, /* can be NULL */
              const void* ancillary, /* can be NULL */
              socklen_t ancillary_len,
              int64_t* num_sent_out);

typedef enum {
  QIO_CH_UNBUFFERED = 1,
  QIO_CH_BUFFERED
} qio_chtype_t;

#define QIO_CH_DEFAULT 0
#define QIO_CH_MIN_TYPE QIO_CH_UNBUFFERED
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

#define QIO_CHTYPEMASK 0x000f
#define QIO_CHTYPE_CLOSED 0x000f
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
  QIO_HINT_DIRECT       = QIO_HINT_CACHED<<1,
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
  qio_chtype_t type = hint & QIO_CHTYPEMASK;
  qio_method_t method = hint & QIO_METHODMASK;
  int ok = 0;

  buf[0] = '\0';
  if( type == QIO_CH_DEFAULT ) {
    strcat(buf, "default_type"); ok = 1;
  } else {
    switch ( type ) {
      case QIO_CH_UNBUFFERED:
        strcat(buf, "unbuffered"); ok = 1;
        break;
      case QIO_CH_BUFFERED:
        strcat(buf, "buffered"); ok = 1;
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
  if( hint & QIO_HINT_BANDWIDTH ) strcat(buf, " bandwidth");
  if( hint & QIO_HINT_CACHED ) strcat(buf, " cached");
  if( hint & QIO_HINT_DIRECT ) strcat(buf, " direct");
  if( hint & QIO_HINT_NOREUSE ) strcat(buf, " noreuse");
  if( hint & QIO_HINT_NOFAST ) strcat(buf, " nofast");

  return qio_strdup(buf);
}
#endif

typedef struct qio_file_s {
  // reference count which is atomically updated
  long ref_cnt;

  // these fields are fixed for the life of the object
  // and indicate how the file is backed.
  // We could potentially also support a "virtual file"
  // that handled pread/pwrite by calling some routine,
  // but mostly 
  // An (arguably) better solution is to put 
  FILE* fp; // if fp is set, fd == fileno(fp)
  fd_t fd; // -1 if not set
  qbuffer_t* buf; // NULL if not set.
                  // if set, fp==NULL, fd==-1, is memory-only file.
                  // Note a qbuffer is not thread-safe, and
                  // so access to this must be protected
                  // by the file's lock.

  qio_fdflag_t fdflags;
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

// if fp is not null, fd is ignored; if fp is null, we use fd.
err_t qio_file_init(qio_file_t** file_out, FILE* fp, fd_t fd, qio_hint_t iohints, qio_style_t* style);
err_t qio_file_open(qio_file_t** file_out, const char* path, int flags, mode_t mode, qio_hint_t iohints, qio_style_t* style);
err_t qio_file_open_access(qio_file_t** file_out, const char* pathname, const char* access, qio_hint_t iohints, qio_style_t* style);
err_t qio_file_open_mem_ext(qio_file_t** file_out, qbuffer_t* buf, qio_fdflag_t fdflags, qio_hint_t iohints, qio_style_t* style);
err_t qio_file_open_mem(qio_file_t** file_out, qbuffer_t* buf, qio_style_t* style);

err_t qio_file_open_tmp(qio_file_t** file_out, qio_hint_t iohints, qio_style_t* style);

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
err_t qio_file_close(qio_file_t* f);

err_t qio_file_sync(qio_file_t* f);

// This one gets called automatically.
void _qio_file_destroy(qio_file_t* f);

// returns a newly allocated string in *string_out which must be freed.
err_t qio_file_path_for_fd(fd_t fd, const char** string_out);
err_t qio_file_path_for_fp(FILE* fp, const char** string_out);
err_t qio_file_path(qio_file_t* f, const char** string_out);

static inline
void qio_file_retain(qio_file_t* f) {
  DO_RETAIN(f);
}

static inline
void qio_file_release(qio_file_t* f) {
  DO_RELEASE(f, _qio_file_destroy);
}

static inline
err_t qio_file_lock(qio_file_t* f)
{
  return qio_lock(&f->lock);
}

static inline
void qio_file_unlock(qio_file_t* f)
{
  qio_unlock(&f->lock);
}

// You should lock/ get ptr/ unlock
static inline
qio_style_t* qio_file_style_ptr(qio_file_t* f)
{
  return &f->style;
}
static inline
void qio_file_get_style(qio_file_t* f, qio_style_t* style)
{
  *style = f->style;
}
static inline
void qio_file_set_style(qio_file_t* f, qio_style_t* style)
{
  f->style = *style;
}

// Return the current length of a file.
// Calls stat for a file descriptor
// Calls fflush on a FILE* first.
err_t qio_file_length(qio_file_t* f, int64_t *len_out);

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
 */
#define MARK_INITIAL_STACK_SZ 2
typedef struct qio_buffered_channel_s {
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
   * right_mark_start is av_start or mark_space[mark_next-1].
   */

  /* Each buffer is divided into these sections:
   * _________________________________________________________________
   * |write-behind | user writeable/readable | read-ahead/buffer space|
   *             av_start                  av_end
   *                  right_mark_start
   * the available section is ready for user read/write.
   * Space to the right of av_end is allocated but not yet read
   * (if we're reading). When writing, av_end == end of buf,
   * and so there is no extra space on the right. If one day
   * we do read-modify-write (which isn't supported in the initial version),
   * there might be between av_end and the end of the buffer
   * even when writing... but it might be easier to do read-modify-write
   * by coupling together 2 channels with the zero-copy capabilities.
   */

  int64_t av_start;
  int64_t av_end;

  qbuffer_t buf;

  size_t mark_next;
  size_t mark_stack_size;
  int64_t* mark_stack;
  // initial 2 entries so we don't have to malloc
  // for the common case of very few marks.
  int64_t mark_space[MARK_INITIAL_STACK_SZ];
} qio_buffered_channel_t;

typedef struct qio_unbuffered_channel_s {
  int64_t pos;
} qio_unbuffered_channel_t;

typedef struct qio_channel_s {
  // frequently used data:
  void* cached_cur; // current offset in a buffer.
  void* cached_end; // end offset in a buffer; we don't exceed this.

  // reference count which is atomically updated
  long ref_cnt;

  // for a buffered channel, cur is in part pointed to by av_start.
  
  qio_file_t* file;
  qio_lock_t lock;

  // less frequently used.
  int64_t start_pos; // what is the initial position of the channel?
                     // this is used in a thread-safe truncation
                     // scheme for mmap channels.
  int64_t end_pos; // we should not write at or beyond this position.
                   // use INT64_MAX for no limit.
  qio_hint_t hints;
  qio_fdflag_t flags;

  union {
    qio_unbuffered_channel_t unbuffered;
    qio_buffered_channel_t buffered;
  } u;

  qio_style_t style;
} qio_channel_t;


typedef qio_channel_t* qio_channel_ptr_t;
#define QIO_CHANNEL_PTR_NULL NULL

void _qio_channel_destroy(qio_channel_t* ch);

static inline
void qio_channel_retain(qio_channel_t* ch) {
  if( DEBUG_QIO ) fprintf(stdout, "Channel retain %p\n", ch);
  DO_RETAIN(ch);
}

static inline
void qio_channel_release(qio_channel_t* ch) {
  if( DEBUG_QIO ) fprintf(stdout, "Channel release %p\n", ch);
  DO_RELEASE(ch, _qio_channel_destroy);
}

err_t _qio_channel_init_unbuffered(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style);
err_t _qio_channel_init_buffered(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style);
err_t _qio_channel_init_file(qio_channel_t* ch, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style);


// maybe want to use INT64_MAX for end if it's not to be restricted.
err_t qio_channel_create(qio_channel_t** ch_out, qio_file_t* file, qio_hint_t hints, int readable, int writeable, int64_t start, int64_t end, qio_style_t* style);


err_t _qio_slow_read(qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* amt_read);
err_t _qio_slow_write(qio_channel_t* ch, const void* ptr, ssize_t len, ssize_t* amt_written);

err_t _qio_channel_flush_unlocked(qio_channel_t* ch);

// qio_channel_read does not handle text (since we don't know
// when to stop reading) - only fixed-size elements.
// Returns an error code.
//
// On a read, returns EEOF and *amt_read=some amount for EOF.
static inline
err_t qio_channel_read(const int threadsafe, qio_channel_t* ch, void* ptr, ssize_t len, ssize_t* amt_read)
{
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *amt_read = 0;
      return err;
    }
  }

  // Is there room in our fast path buffer?
  if( len <= VOID_PTR_DIFF(ch->cached_end, ch->cached_cur) ) {
    memcpy( ptr, ch->cached_cur, len );
    ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, len);
    *amt_read = len;
    err = 0;
  } else {
    err = _qio_slow_read(ch, ptr, len, amt_read);
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
err_t qio_channel_lock(qio_channel_t* ch)
{
  return qio_lock(&ch->lock);
}

static inline
void qio_channel_unlock(qio_channel_t* ch)
{
  qio_unlock(&ch->lock);
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





 
static inline
err_t qio_channel_write(const int threadsafe, qio_channel_t* ch, const void* ptr, ssize_t len, ssize_t* amt_written )
{
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *amt_written = 0;
      return err;
    }
  }

  // Is there room in our fast path buffer?
  if( len <= VOID_PTR_DIFF(ch->cached_end, ch->cached_cur) ) {
    memcpy( ch->cached_cur, ptr, len );
    ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, len);
    *amt_written = len;
    err = 0;
  } else {
    err = _qio_slow_write(ch, ptr, len, amt_written);
  }

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}


static inline
err_t qio_channel_read_amt(const int threadsafe, qio_channel_t* ch, void* ptr, ssize_t len) {
  err_t err;
  ssize_t amt = 0;
  err = qio_channel_read(threadsafe, ch, ptr, len, &amt);
  if( err ) return err;
  if( amt != len ) return ESHORT;
  return 0;
}

static inline
err_t qio_channel_write_amt(const int threadsafe, qio_channel_t* ch, const void* ptr, ssize_t len) {
  err_t err;
  ssize_t amt = 0;
  err = qio_channel_write(threadsafe, ch, ptr, len, &amt);
  if( err ) return err;
  if( amt != len ) return ESHORT;
  return 0;
}

err_t _qio_channel_require_unlocked(qio_channel_t* ch, int64_t space, int writing);

static inline
err_t qio_channel_require_read(const int threadsafe, qio_channel_t* ch, int64_t space)
{
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = _qio_channel_require_unlocked(ch, space, 0);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
err_t qio_channel_require_write(const int threadsafe, qio_channel_t* ch, int64_t space)
{
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  err = _qio_channel_require_unlocked(ch, space, 1);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

void _qio_buffered_setup_cached(qio_channel_t* ch);
void _qio_buffered_advance_cached(qio_channel_t* ch);

// you don't have to call end_peek_cached if this returns an error
static inline
err_t qio_channel_begin_peek_cached(const int threadsafe, qio_channel_t* ch, void** start_out, void** end_out)
{
  err_t err;

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

  return err;
}

static inline
err_t qio_channel_end_peek_cached(const int threadsafe, qio_channel_t* ch, void* new_start)
{
  err_t err;

  ch->cached_cur = new_start;
  err = 0;

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}
err_t qio_channel_begin_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t require, int writing, qbuffer_t** buf_out, qbuffer_iter_t* start_out, qbuffer_iter_t* end_out);

err_t qio_channel_end_peek_buffer(const int threadsafe, qio_channel_t* ch, int64_t advance);

static inline
err_t qio_channel_isbuffered(const int threadsafe, qio_channel_t* ch, char* isbuffered)
{
  if( (ch->hints & QIO_CHTYPEMASK) == QIO_CH_BUFFERED ) {
    *isbuffered = 1;
  } else {
    *isbuffered = 0;
  }
  return 0;
}


err_t qio_channel_offset(const int threadsafe, qio_channel_t* ch, int64_t* offset_out);
int64_t qio_channel_offset_unlocked(qio_channel_t* ch);


err_t qio_channel_advance(const int threadsafe, qio_channel_t* ch, int64_t nbytes);

err_t qio_channel_put_bytes(const int threadsafe, qio_channel_t* ch, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes);

err_t qio_channel_put_buffer(const int threadsafe, qio_channel_t* ch, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end);


static inline
err_t qio_channel_flush(const int threadsafe, qio_channel_t* ch)
{
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = _qio_channel_flush_unlocked(ch);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

err_t _qio_channel_final_flush_unlocked(qio_channel_t* ch);


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
err_t qio_channel_close(const int threadsafe, qio_channel_t* ch)
{
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = _qio_channel_final_flush_unlocked(ch);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
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

void qio_channel_advance_unlocked(qio_channel_t* ch, int64_t nbytes);

static inline
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

// Only returns threading errors; guaranteed not to return
// an error if threadsafe == false (it asserts instead on bad cases)
static inline
err_t qio_channel_revert(const int threadsafe, qio_channel_t* ch)
{
  err_t err;

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

  return err;
}


// Only returns threading errors; guaranteed not to return
// an error if threadsafe == false (it asserts instead on bad cases)
static inline
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


// Only returns threading errors; guaranteed not to return
// an error if threadsafe == false (it asserts instead on bad cases)
static inline
err_t qio_channel_commit(const int threadsafe, qio_channel_t* ch)
{
  err_t err;

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

/*
err_t qio_channel_fopen(const int threadsafe, qio_channel_t* ch, int reading, int writing, FILE** out);
err_t qio_vprintf(const int threadsafe, qio_channel_t* ch,
                  const char* fmt, va_list ap);
err_t qio_printf(const int threadsafe, qio_channel_t* ch,
                 const char* fmt, ...);
err_t qio_vscanf(const int threadsafe, qio_channel_t* ch,
                 int* nmatched, const char* fmt, va_list ap);
err_t qio_scanf(const int threadsafe, qio_channel_t* ch,
                 int* nmatched, const char* fmt, ...);

// returns EFORMAT if it didn't match.
err_t qio_scanf1(const int threadsafe, qio_channel_t* ch,
                 const char* fmt, void* x);

qio_chtype_t qio_choose_io_type(qio_hint_t hints);
*/

/*
// recognizer returns 0 for not recognized yet,
// -1 if recognized, or a positive number for an error.
// offset is returned in the buffer's coordinate system (not a distance
// but an absolute position).
typedef err_t (*recognizer_t)(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* state, int64_t* offset);


// Peek until a recognizer function is satisfied.
// In *amount, returns the number of bytes we passed.
err_t qio_channel_require_until(const int threadsafe, qio_channel_t* ch, recognizer_t recognize, void* state, int64_t* amount);

// state should be pointer-to-char
err_t until_char(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* state, int64_t* offset);

// state should be pointer to char[256]
err_t until_table(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* state, int64_t* offset);

typedef struct until_noescape_s {
  char start_char;
  char end_char;
  char escape_char;
  char state; // 0 is at start, 1 is in string, 2 is in string after escape
} until_noescape_t;
err_t until_char_noescape(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* state, int64_t* offset);
*/

#endif
