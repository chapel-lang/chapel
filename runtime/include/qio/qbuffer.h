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

#ifndef _QBUFFER_H_
#define _QBUFFER_H_

// This macro set to obtain the portable format macro PRIu64 for debug output.
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS 1
#endif

#include "sys_basic.h"
#include "qio_error.h"
#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <inttypes.h>
#include <stdint.h>

#include <sys/uio.h>
#include "deque.h"

typedef uint_least64_t qb_refcnt_base_t;
#if defined(__cplusplus) && defined(QIO_USE_STD_ATOMICS_REF_CNT)
// work around for issues with older GCC cstdlib atomics
#include <atomic>
typedef std::atomic<qb_refcnt_base_t> qbytes_refcnt_t;
#define ATOMIC_INIT(a, val) a.store(val)
#define ATOMIC_LOAD(a) a.load()
#define ATOMIC_INCR(a) a.fetch_add(1)
#define ATOMIC_DECR(a) a.fetch_sub(1)
#define ATOMIC_DEST(a)
#else
#include "chpl-atomics.h"
typedef atomic_uint_least64_t qbytes_refcnt_t;
#define ATOMIC_INIT(a, val) atomic_init_uint_least64_t(&a, val)
#define ATOMIC_LOAD(a) atomic_load_uint_least64_t(&a)
#define ATOMIC_INCR(a) atomic_fetch_add_uint_least64_t(&a, 1)
#define ATOMIC_DECR(a) atomic_fetch_sub_uint_least64_t(&a, 1)
#define ATOMIC_DEST(a) atomic_destroy_uint_least64_t(&a)
#endif

// The ref count is initialized to 1, in anticipation that the returned pointer
// will be stored in some data structure.  If not, then the client is
// responsible for calling DO_RELEASE right away, to avoid memory leakage.
// This is a slight optimization, since execution on the "happy path" would
// create the ref count and then increment it right away.  Depending on the
// implementation, incrementing an atomic can be expensive.
#define DO_INIT_REFCNT(ptr) ATOMIC_INIT(ptr->ref_cnt, 1)
#define DO_GET_REFCNT(ptr) ATOMIC_LOAD(ptr->ref_cnt)

#define DO_RETAIN(ptr) \
{ \
  /* do nothing to NULL */ \
  if( ptr ) { \
    qb_refcnt_base_t old_cnt = ATOMIC_INCR(ptr->ref_cnt); \
    /* if it was 0, we couldn't have had a ref to it */ \
    /* if it is now 0, we overflowed the ref count (very unlikely). */ \
    if( old_cnt + 1 <= 1 ) *(volatile int *)(0) = 0; /* deliberately segfault. */ \
  } \
}

#define DO_RELEASE(ptr, free_function) \
{ \
  /* do nothing to NULL */ \
  if( ptr ) { \
    qb_refcnt_base_t old_cnt = ATOMIC_DECR(ptr->ref_cnt); \
    if( old_cnt == 1 ) { \
      /* that means, after we decremented it, the count is 0. */ \
      free_function(ptr); \
    } else { \
      /* old_cnt == 0 is a fatal error (underflow) */ \
      if( old_cnt == 0 ) *(volatile int *)(0) = 0; /* deliberately segfault. */ \
    } \
  } \
}

#define DO_DESTROY_REFCNT(ptr) ATOMIC_DEST(ptr->ref_cnt)

#ifdef __cplusplus
extern "C" {
#endif

// how large is an iobuf?
extern size_t qbytes_iobuf_size;

struct qbytes_s;

// a free function
typedef void (*qbytes_free_t)(struct qbytes_s*);

/* A qbytes_t is just some data with a length.
 * It is a reference-counted class type.
 * Once a qbytes_t is only the reference count may change;
 * all the other fields must remain fixed. (Note that the
 * memory pointed to by data may change).
 *
 * The free function must free both the bytes buffer as well
 * as the qbytes object itself.
 *
 * Multiple threads accessing qbytes must be managed by means
 * outside of the qbytes.
 */
typedef struct qbytes_s {
  // reference count which is atomically updated
  qbytes_refcnt_t ref_cnt;
  // all of the other fields 
  void* data;
  int64_t len;
  qbytes_free_t free_function;
  uint8_t flags; // is it const?
  uint8_t unused1;
  uint16_t unused2;
  uint32_t unused3; // this could be locale UID of the pointer!
} qbytes_t;

typedef qbytes_t _qbytes_ptr_t;
typedef qbytes_t* qbytes_ptr_t;
#define QBYTES_PTR_NULL NULL

// increment the reference count
static inline
void qbytes_retain(qbytes_t* qb)
{
  DO_RETAIN(qb);
}

// decrement the reference count; free on 0.
static inline
void qbytes_release(qbytes_t* qb)
{
  DO_RELEASE(qb, qb->free_function);
}

// for being called by free functions... frees only the qbytes itself.
void _qbytes_free_qbytes(qbytes_t* b);
// free a NULL one
void qbytes_free_null(qbytes_t* b);
// unmap the data
void qbytes_free_munmap(qbytes_t* b);
// free the data
void qbytes_free_qio_free(qbytes_t* b);

void _qbytes_init_generic(qbytes_t* ret, void* give_data, int64_t len, qbytes_free_t free_function);
qioerr qbytes_create_generic(qbytes_t** out, void* give_data, int64_t len, qbytes_free_t free_function);
qioerr _qbytes_init_iobuf(qbytes_t* ret);
qioerr qbytes_create_iobuf(qbytes_t** out);
qioerr _qbytes_init_calloc(qbytes_t* ret, int64_t len);

// The caller is responsible for calling qbytes_release on the return value.
qioerr qbytes_create_calloc(qbytes_t** out, int64_t len);

static inline
int64_t qbytes_len(qbytes_t* b)
{
  return b->len;
}

static inline
void* qbytes_data(qbytes_t* b)
{
  return b->data;
}

typedef enum {
  QB_PART_FLAGS_NONE = 0,
  QB_PART_FLAGS_EXTENDABLE_TO_ENTIRE_BYTES = 1,
} qbuffer_part_flags_t;

typedef struct qbuffer_part_s {
  // part refers to the region
  // [bytes->data + skip_bytes, bytes->data + skip_bytes + len_bytes)
  qbytes_t* bytes;
  int64_t skip_bytes;
  int64_t len_bytes; // does not include skip_bytes
  int64_t end_offset; // in bytes; subtract len_bytes from this to get start_offset
  qbuffer_part_flags_t flags;
  // for unicode strings, we might add a end_in_characters
} qbuffer_part_t;

/* A buffer is a group of bytes_t objects. It has:
 *  - some number of bytes_t objects (reference counted)
 *
 *  Buffers support O(1) push/pop on front/back of bytes, simple&fast iteration,
 *  and logarithmic indexing operations. They're not meant to support
 *  fast inserting at any position.
 *
 * Buffers are not inherently thread-safe and access should
 * be protected by some other means.
 * 
 * A buffer should be wrapped as a record; the destructor
 * should be called when it goes out of scope.
 */
typedef struct qbuffer_s {
  qbytes_refcnt_t ref_cnt; // atomically updated
  deque_t deque; // contains qbuffer_part_t s 
  int64_t offset_start;
  int64_t offset_end;
} qbuffer_t;

typedef qbuffer_t _qbuffer_ptr_t;
typedef qbuffer_t* qbuffer_ptr_t;
#define QBUFFER_PTR_NULL NULL

typedef struct qbuffer_iter_s {
  int64_t offset; // valid iter has offset_start <= offset <= offset_end
  deque_iterator_t iter;
} qbuffer_iter_t;

static inline
qbuffer_iter_t qbuffer_iter_null(void) {
  qbuffer_iter_t ret = {0, deque_iterator_null()};
  return ret;
}

void debug_print_qbuffer_iter(qbuffer_iter_t* iter);
void debug_print_qbuffer(qbuffer_t* buf);
void debug_print_iovec(const struct iovec* iov, int iovcnt, size_t maxbytes);


static inline
void qbuffer_init_uninitialized(qbuffer_t* buf) {
  deque_init_uninitialized(&buf->deque);
}
static inline
int qbuffer_is_initialized(qbuffer_t* buf) {
  return deque_is_initialized(&buf->deque);
}

/* Initialize a buffer */
qioerr qbuffer_init(qbuffer_t* buf);

/* Destroy a buffer inited with qbuffer_init */
qioerr qbuffer_destroy(qbuffer_t* buf);

/* Destroy a buffer and free() the pointer */
qioerr qbuffer_destroy_free(qbuffer_t* buf);

/* Create a reference-counted buffer ptr */
qioerr qbuffer_create(qbuffer_ptr_t* out);

/* Increment a reference count
 */
static inline
void qbuffer_retain(qbuffer_ptr_t buf)
{
  DO_RETAIN(buf);
}

/* Release a reference-counted buffer ptr */
static inline
void qbuffer_release(qbuffer_ptr_t buf)
{
  DO_RELEASE(buf, qbuffer_destroy_free);
}

void qbuffer_extend_back(qbuffer_t* buf);
void qbuffer_extend_front(qbuffer_t* buf);

/* Append a bytes_t to a buffer group.
 */
qioerr qbuffer_append(qbuffer_t* buf, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes);

/* Append a buffer to another buffer; the bytes will be shared (reference counts increased) */
qioerr qbuffer_append_buffer(qbuffer_t* buf, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end);

/* Prepend a bytes_t to a buffer group.
 */
qioerr qbuffer_prepend(qbuffer_t* buf, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes);

/* trim functions remove parts that are completely in the area
 * to be removed. */
void qbuffer_trim_front(qbuffer_t* buf, int64_t remove_bytes);
void qbuffer_trim_back(qbuffer_t* buf, int64_t remove_bytes);

/* Remove a part from the front or back. */
qioerr qbuffer_pop_front(qbuffer_t* buf);
qioerr qbuffer_pop_back(qbuffer_t* buf);

/* Without changing any memory, changes the offsets
 * used in the buffer (offset_start, offset_end, and offsets in the parts).
 */
void qbuffer_reposition(qbuffer_t* buf, int64_t new_offset_start);

qbuffer_iter_t qbuffer_begin(qbuffer_t* buf);
qbuffer_iter_t qbuffer_end(qbuffer_t* buf);

static inline
ssize_t qbuffer_num_parts(qbuffer_t* buf)
{
  return deque_size(sizeof(qbuffer_part_t), & buf->deque);
}

/* do a and b refer to the same part?
 */
static inline
char qbuffer_iter_same_part(qbuffer_iter_t a, qbuffer_iter_t b) {
  return deque_it_equals(a.iter, b.iter);
}

/* Moves to the beginning of the next part
 */
void qbuffer_iter_next_part(qbuffer_t* buf, qbuffer_iter_t* iter);

/* Moves to the beginning of the previous part
 */
void qbuffer_iter_prev_part(qbuffer_t* buf, qbuffer_iter_t* iter);

/* Move to the beginning of the current part 
 */
void qbuffer_iter_floor_part(qbuffer_t* buf, qbuffer_iter_t* iter);
/* Move to the end of the current part.
 */
void qbuffer_iter_ceil_part(qbuffer_t* buf, qbuffer_iter_t* iter);

/* Advances an iterator using linear search. 
 * Returns qbuffer_start() if the resulting offset was < start
 *         qbuffer_end() if the resulting offset was >= end
 */
void qbuffer_iter_advance(qbuffer_t* buf, qbuffer_iter_t* iter, int64_t amt);

/* Find offset in window in logarithmic time.
 * Note these offsets start at buf->offset_start, not 0.
 *
 * Returns qbuffer_start() if the offset was < start
 *         qbuffer_end() if the offset was >= end
 */
qbuffer_iter_t qbuffer_iter_at(qbuffer_t* buf, int64_t offset);

/* How many parts are in an iterator? end >= start
 * always returns 1 + end - start
 */
static inline
ssize_t qbuffer_iter_num_parts(qbuffer_iter_t start, qbuffer_iter_t end)
{
  return 1 + deque_it_difference(sizeof(qbuffer_part_t), end.iter, start.iter);
}

static inline
char qbuffer_iter_equals(qbuffer_iter_t start, qbuffer_iter_t end)
{
  return start.offset == end.offset;
}

/* How many bytes are in an iterator? end >= start
 */
static inline
int64_t qbuffer_iter_num_bytes(qbuffer_iter_t start, qbuffer_iter_t end)
{
  return end.offset - start.offset;
}

/* How many bytes are after an iterator?
 */
static inline
int64_t qbuffer_iter_num_bytes_after(qbuffer_t* buf, qbuffer_iter_t iter)
{
  return buf->offset_end - iter.offset;
}

/* How many bytes are before an iterator?
 */
static inline
int64_t qbuffer_iter_num_bytes_before(qbuffer_t* buf, qbuffer_iter_t iter)
{
  return iter.offset - buf->offset_start;
}

// Returns whats in the iterator. If the caller wants to hold on to
// the bytes, it should retain them (this function does not).
static inline
void qbuffer_iter_get(qbuffer_iter_t iter, qbuffer_iter_t end, qbytes_t** bytes_out, int64_t* skip_out, int64_t* len_out)
{
  qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter.iter);
  int64_t iter_offset_within = iter.offset - (qbp->end_offset - qbp->len_bytes);
  int64_t part_len = qbp->len_bytes - iter_offset_within;
  int64_t len = end.offset - iter.offset;
  
  if( len > part_len ) len = part_len;

  *bytes_out = qbp->bytes;
  *skip_out = qbp->skip_bytes + iter_offset_within;
  *len_out = len;
}

static inline
int64_t qbuffer_start_offset(qbuffer_t* buf)
{
  return buf->offset_start;
}

static inline
int64_t qbuffer_end_offset(qbuffer_t* buf)
{
  return buf->offset_end;
}

static inline
int64_t qbuffer_len(qbuffer_t* buf)
{
  return buf->offset_end - buf->offset_start;
}

/* Turn a range from a qbuffer into an io-vector. Note that this contains
 * pointers into the qbuffer and is only valid until the qbuffer is changed...
 */
qioerr qbuffer_to_iov(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, 
                   size_t max_iov, struct iovec *iov_out, 
                   qbytes_t** bytes_out /* can be NULL */,
                   size_t *iovcnt_out);

/* Returns a single bytes object for the qbuffer.
 * This is like the C++ string.c_str() method;
 *
 * It is the responsibility of the caller to call
 * qbytes_release on the result of this function.
 */
qioerr qbuffer_flatten(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, qbytes_t** bytes_out);

/* Create a reference-sharing version of the buffer,
 * starting from current iterator position.
 */
//qioerr qbuffer_clone(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, qbuffer_ptr_t* buf_out);

/* Copies bytes from start to end in buffer to ptr.
 * Returns an error if we would exceed ret_len
 * */
qioerr qbuffer_copyout(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* ptr, size_t ret_len);

/* Copies bytes from ptr to start to end in buffer.
 * Returns an error if we would exceed ret_len
 * */
qioerr qbuffer_copyin(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, const void* ptr, size_t ret_len);

qioerr qbuffer_copyin_buffer(qbuffer_t* dst, qbuffer_iter_t dst_start, qbuffer_iter_t dst_end,
                            qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end);

/* Overwrites the qbuffer buffers with a fixed byte.
 * */
qioerr qbuffer_memset(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, unsigned char byte);

#ifdef __cplusplus
} // end extern "C"
#endif

// How many bytes to try to store on stack in some functions that don't
// really want to call malloc
#define MAX_ON_STACK 128

#ifdef _chplrt_H_

#include "chpl-mem.h"
#define qio_malloc(size) chpl_mem_alloc(size, CHPL_RT_MD_IO_BUFFER, __LINE__, 0)
#define qio_calloc(nmemb, size) chpl_mem_allocManyZero(nmemb, size, CHPL_RT_MD_IO_BUFFER, __LINE__, 0)
#define qio_realloc(ptr, size) chpl_mem_realloc(ptr, size, CHPL_RT_MD_IO_BUFFER, __LINE__, 0)
#define qio_memalign(boundary, size)  chpl_memalign(boundary, size)
#define qio_free(ptr) chpl_mem_free(ptr, __LINE__, 0)
#define qio_memcpy(dest, src, num) chpl_memcpy(dest, src, num)

typedef chpl_bool qio_bool;

#else

#include "chpl-mem-sys.h"

#define qio_malloc(size) sys_malloc(size)
#define qio_calloc(nmemb, size) sys_calloc(nmemb,size)
#define qio_realloc(ptr, size) sys_realloc(ptr, size)
#define qio_memalign(boundary, size) sys_memalign(boundary, size)
#define qio_free(ptr) sys_free(ptr)
#define qio_memcpy(dest, src, num) memcpy(dest, src, num)

typedef bool qio_bool;

#endif


#ifdef __cplusplus
extern "C" {
#endif

static inline char* qio_strdup(const char* ptr)
{
  size_t len = strlen(ptr) + 1;
  char* ret = (char*) qio_malloc(len);
  if( ret ) qio_memcpy(ret, ptr, len);
  return ret;
}

#ifdef __cplusplus
} // end extern "C"
#endif

// Declare MAX_ON_STACK bytes. We declare it with the original
// type to make sure it's aligned as well as malloc would be.
#define MAYBE_STACK_SPACE(type,onstack) \
  type onstack[MAX_ON_STACK/sizeof(type)]

#define MAYBE_STACK_ALLOC(type, count, ptr, onstack) \
{ \
  /* check for integer overflow or negative count */ \
  if( count >= 0 && \
      (uint64_t) count <= (SIZE_MAX / sizeof(type)) ) { \
    /* check that count is positive and small enough to go on the stack */ \
    if( (size_t) count <= (sizeof(onstack)/sizeof(type)) ) { \
      ptr = onstack; \
    } else { \
      ptr = (type*) qio_malloc(count*sizeof(type)); \
    } \
  } else { \
    /* handle integer overflow */ \
    ptr = NULL; \
    assert(0 && "size overflow in MAYBE_STACK_ALLOC"); \
  } \
}


#define MAYBE_STACK_FREE(ptr, onstack) \
{ \
  if (ptr && ptr != onstack) { \
    qio_free(ptr); \
  } \
}

#ifdef __cplusplus
extern "C" {
#endif

/* Returns the difference between two pointers,
   but returns 0 if either pointer is NULL.
 */
static inline intptr_t qio_ptr_diff(void* a, void* b)
{
  if( a == NULL || b == NULL ) return 0;
  return ((intptr_t)a) - ((intptr_t)b);
}

/* Returns 1 if there is space for nbytes between cur and end
   (cur might represent a current buffer position and end might
    represent the boundary.)
   Takes into account the possibility that cur or end might be NULL;
   returns 0 if either is NULL and nbytes > 0.
    */
static inline int qio_space_in_ptr_diff(intptr_t nbytes, void* end, void* cur)
{
  return nbytes <= qio_ptr_diff(end,cur);
}

static inline void* qio_ptr_add(void* ptr, intptr_t amt)
{
  return ((void*)(((char*) (ptr)) + (amt)));
}

/* Returns the number of bytes between ptr and its alignment.
   align must be a power of 2.
 */
static inline uintptr_t qio_ptr_align(void* ptr, uintptr_t align)
{
 return (((uintptr_t)ptr) & (align - 1));
}

#ifdef __cplusplus
} // end extern "C"
#endif


#endif

