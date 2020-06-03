/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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



#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "qbuffer.h"

#include "error.h"

#include "sys.h"

#include <limits.h>
#include <sys/mman.h>

#include <ctype.h>
#include <assert.h>

// 64kb blocks...
// this really should be a multiple of page size...
// but we can't know page size at compile time
size_t qbytes_iobuf_size = 64*1024;

// prototypes.

void qbytes_free_iobuf(qbytes_t* b);
void debug_print_bytes(qbytes_t* b);
qioerr qbuffer_init_part(qbuffer_part_t* p, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes, int64_t end_offset);


// global, shared pools.

void _qbytes_free_qbytes(qbytes_t* b)
{
  b->data = NULL;
  b->len = 0;
  b->free_function = NULL;
  DO_DESTROY_REFCNT(b);
  qio_free(b);
}

void qbytes_free_null(qbytes_t* b) {
  _qbytes_free_qbytes(b);
}

void qbytes_free_munmap(qbytes_t* b) {
  err_t err;

  /* I don't believe this is required, but
   * I've heard here and there it might be for NFS...
   *
  rc = msync(b->data, b->len, MS_ASYNC);
  if( rc ) fprintf(stderr, "Warning - in qbytes_free_munmap, msync failed with %i, errno %i\n", rc, errno);
  */

  err = sys_munmap(b->data, b->len);

  if (err) {
    chpl_internal_error("sys_munmap() failed");
  }

  _qbytes_free_qbytes(b);
}

void qbytes_free_qio_free(qbytes_t* b) {
  qio_free(b->data);
  _qbytes_free_qbytes(b);
}
void qbytes_free_iobuf(qbytes_t* b) {
  // iobuf is just something to be freed with free()
  qbytes_free_qio_free(b);
}

void debug_print_bytes(qbytes_t* b)
{
  fprintf(stderr, "bytes %p: data=%p len=%lli ref_cnt=%" PRIu64 " free_function=%p flags=%i\n",
          b, b->data, (long long int) b->len, DO_GET_REFCNT(b),
          b->free_function, b->flags);
}

void debug_print_iovec(const struct iovec* iov, int iovcnt, size_t maxbytes)
{
  int i;
  size_t j;
  size_t nb = 0;
  for( i = 0; i < iovcnt; i++ ) {
    for( j = 0; j < iov[i].iov_len && nb < maxbytes; j++,nb++) {
      char* buf = (char*) iov[i].iov_base;
      char c = '.';
      if (isprint(buf[j]))
        c = buf[j];
      printf("%c", c);
    }
  }
  printf("\n");
}

// On return the ref count is 1.
// The callee owns the returned qbytes buffer until qbytes_release is called.
void _qbytes_init_generic(qbytes_t* ret, void* give_data, int64_t len, qbytes_free_t free_function)
{
  ret->data = give_data;
  ret->len = len;
  DO_INIT_REFCNT(ret);
  ret->flags = 0;
  ret->free_function = free_function;
}

// On return, the ref count is 1.
qioerr qbytes_create_generic(qbytes_t** out, void* give_data, int64_t len, qbytes_free_t free_function)
{
  qbytes_t* ret = NULL;

  ret = (qbytes_t*) qio_calloc(1, sizeof(qbytes_t));
  if( ! ret ) return QIO_ENOMEM;

  // On return the ref count is 1.
  _qbytes_init_generic(ret, give_data, len, free_function);

  *out = ret;

  return 0;
}

// On return, the ref count in ret is 1.
// The caller owns the returned qbytes buffer until qbytes_release
// is called on it.
qioerr _qbytes_init_iobuf(qbytes_t* ret)
{
  void* data = NULL;

  // qbytes_iobuf_size is generally >= page size. However in
  // some testing configurations, it is very small (e.g. 5 bytes).
  size_t page_size = sys_page_size();
  if (qbytes_iobuf_size >= page_size)
    data = qio_memalign(page_size, qbytes_iobuf_size);
  else
    data = qio_malloc(qbytes_iobuf_size);

  if( !data ) return QIO_ENOMEM;
  memset(data, 0, qbytes_iobuf_size);

  // The ref count in ret is initially 1.
  _qbytes_init_generic(ret, data, qbytes_iobuf_size, qbytes_free_iobuf);

  return 0;
}


qioerr qbytes_create_iobuf(qbytes_t** out)
{
  qbytes_t* ret = NULL;
  qioerr err;

  ret = (qbytes_t*) qio_calloc(1, sizeof(qbytes_t));
  if( ! ret ) {
    *out = NULL;
    return QIO_ENOMEM;
  }

  err = _qbytes_init_iobuf(ret);
  if( err ) {
    qio_free(ret);
    *out = NULL;
    return err;
  }

  *out = ret;
  return 0;
}

/*
qioerr _qbytes_init_calloc(qbytes_t* ret, int64_t len)
{
  void* data;

  data = qio_calloc(1,len);
  if( data == NULL ) {
    return QIO_ENOMEM;
  }

  _qbytes_init_generic(ret, data, len, qbytes_free_free);

  return 0;
}*/

// The ref count returned in 'out' is initially 1.
// The caller is responsible for calling qbytes_release on it.
qioerr qbytes_create_calloc(qbytes_t** out, int64_t len)
{
  qbytes_t* ret = NULL;
  void* data;

  ret = (qbytes_t*) qio_calloc(1, sizeof(qbytes_t) + len);
  if( ! ret ) {
    *out = NULL;
    return QIO_ENOMEM;
  }

  data = ret + 1; // ie ret + sizeof(qbytes_t)
  // On return, the ref count in ret is 1.
  _qbytes_init_generic(ret, data, len, qbytes_free_null);

  *out = ret;
  return 0;
}

/*
static inline int leadz64(uint64_t i)
{
  return __builtin_clzll(i);
}

static inline int log2lli(uint64_t i)
{
  return 63 - leadz64(i);
}

int qbytes_append_realloc(qbytes_t* qb, size_t item_size, void* item)
{
   int64_t one = 1;
   int64_t exp;
   int64_t len = qb->len;
   void* a = qb->data;

   if( item_size == 0 ) return 0;
   if( 0 == len ) exp = 0;
   else exp = one << log2lli(len);
   if( len == exp ) {
      // the array is full - double its size.
      if( exp == 0 ) exp = 1;
      while( len + item_size > exp ) exp = 2*exp;

      a = (unsigned char*) realloc(a, exp);
      if( !a ) return QIO_ENOMEM;
      qb->data = a;
   }
   // now put the new value in.
   qio_memcpy(a + len, item, item_size);
   qb->len += item_size; // increment the item size.

   return 0;
}
*/

static inline
void qbuffer_clear_cached(qbuffer_t* buf)
{
}

void debug_print_qbuffer_iter(qbuffer_iter_t* iter)
{
  fprintf(stderr, "offset=%lli ", (long long int) iter->offset);
  debug_print_deque_iter(& iter->iter);
}

void debug_print_qbuffer(qbuffer_t* buf)
{
  deque_iterator_t cur = deque_begin(& buf->deque);
  deque_iterator_t end = deque_end(& buf->deque);

  fprintf(stderr, "buf %p: offset_start=%lli offset_end=%lli deque:\n",
          buf, (long long int) buf->offset_start, (long long int) buf->offset_end);

  // Print out the deque iterators
  debug_print_deque_iter(&cur);
  debug_print_deque_iter(&end);

  while( ! deque_it_equals(cur, end) ) {
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr( sizeof(qbuffer_part_t), cur);
    fprintf(stderr, "part %p: bytes=%p (data %p) skip=%lli len=%lli end=%lli\n",
            qbp, qbp->bytes, qbp->bytes->data,
            (long long int) qbp->skip_bytes,
            (long long int) qbp->len_bytes,
            (long long int) qbp->end_offset);

    deque_it_forward_one( sizeof(qbuffer_part_t), &cur );
  }
}

qioerr qbuffer_init(qbuffer_t* buf)
{
  memset(buf, 0, sizeof(qbuffer_t));
  DO_INIT_REFCNT(buf);
  return deque_init(sizeof(qbuffer_part_t), & buf->deque, 0);
  //return 0;
}

qioerr qbuffer_destroy(qbuffer_t* buf)
{
  qioerr err = 0;
  deque_iterator_t cur = deque_begin(& buf->deque);
  deque_iterator_t end = deque_end(& buf->deque);

  while( ! deque_it_equals(cur, end) ) {
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), cur);

    // release the qbuffer.
    qbytes_release(qbp->bytes);

    deque_it_forward_one(sizeof(qbuffer_part_t), &cur);
  }

  // remove any cached data
  qbuffer_clear_cached(buf);

  // destroy the deque
  deque_destroy(& buf->deque);

  DO_DESTROY_REFCNT(buf);

  return err;
}

qioerr qbuffer_create(qbuffer_t** out)
{
  qbuffer_t* ret = NULL;
  qioerr err;

  ret = (qbuffer_t*) qio_malloc(sizeof(qbuffer_t));
  if( ! ret ) {
    *out = NULL;
    return QIO_ENOMEM;
  }

  err = qbuffer_init(ret);
  if( err ) {
    qio_free(ret);
    *out = NULL;
    return err;
  }

  *out = ret;

  return 0;
}

qioerr qbuffer_destroy_free(qbuffer_t* buf)
{
  qioerr err;
  err = qbuffer_destroy(buf);
  qio_free(buf);
  return err;
}

qioerr qbuffer_init_part(qbuffer_part_t* p, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes, int64_t end_offset)
{
  if( len_bytes < 0 || skip_bytes < 0 || skip_bytes + len_bytes > bytes->len )
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");

  qbytes_retain(bytes);

  p->bytes = bytes;
  p->skip_bytes = skip_bytes;
  p->len_bytes = len_bytes;
  p->end_offset = end_offset;

  p->flags = QB_PART_FLAGS_NONE;
  if( skip_bytes == 0 && len_bytes == bytes->len ) p->flags = QB_PART_FLAGS_EXTENDABLE_TO_ENTIRE_BYTES;

  return 0;
}

void qbuffer_extend_back(qbuffer_t* buf)
{
  if( deque_size(sizeof(qbuffer_part_t), &buf->deque) > 0 ) {
    // Get the last part.
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr( sizeof(qbuffer_part_t), deque_last(sizeof(qbuffer_part_t), &buf->deque));
    if( (qbp->flags & QB_PART_FLAGS_EXTENDABLE_TO_ENTIRE_BYTES) &&
        qbp->len_bytes < qbp->bytes->len ) {
      qbp->end_offset = (qbp->end_offset - qbp->len_bytes) + qbp->bytes->len;
      qbp->len_bytes = qbp->bytes->len;
      buf->offset_end = qbp->end_offset;
    }
  }
}

void qbuffer_extend_front(qbuffer_t* buf)
{
  if( deque_size(sizeof(qbuffer_part_t), &buf->deque) > 0 ) {
    // Get the first part.
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr( sizeof(qbuffer_part_t), deque_begin(& buf->deque));
    if( (qbp->flags & QB_PART_FLAGS_EXTENDABLE_TO_ENTIRE_BYTES) &&
        qbp->skip_bytes > 0 ) {
      qbp->len_bytes = qbp->bytes->len;
      qbp->skip_bytes = 0;
      buf->offset_start = qbp->end_offset - qbp->len_bytes;
    }
  }
}

qioerr qbuffer_append(qbuffer_t* buf, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes)
{
  qbuffer_part_t part;
  int64_t new_end;
  qioerr err;

  new_end = buf->offset_end + len_bytes;
  // init part retains the bytes.
  err = qbuffer_init_part(&part, bytes, skip_bytes, len_bytes, new_end);
  if( err ) return err;

  err = deque_push_back(sizeof(qbuffer_part_t), &buf->deque, &part);
  if( err ) {
    qbytes_release(bytes); // release the bytes.
    return err;
  }

  buf->offset_end = new_end;

  // invalidate cached entries.
  qbuffer_clear_cached(buf);

  return 0;
}

qioerr qbuffer_append_buffer(qbuffer_t* buf, qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end)
{
  qbuffer_iter_t src_cur = src_start;
  qbytes_t* bytes;
  int64_t skip;
  int64_t len;
  qioerr err;

  if( buf == src ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "cannot append a buffer to itself");

  while( qbuffer_iter_num_bytes(src_cur, src_end) > 0 ) {
    qbuffer_iter_get(src_cur, src_end, &bytes, &skip, &len);

    err = qbuffer_append(buf, bytes, skip, len);
    if( err ) return err;

    qbuffer_iter_next_part(src, &src_cur);
  }

  return 0;
}

qioerr qbuffer_prepend(qbuffer_t* buf, qbytes_t* bytes, int64_t skip_bytes, int64_t len_bytes)
{
  qbuffer_part_t part;
  int64_t old_start, new_start;
  qioerr err;

  old_start = buf->offset_start;
  new_start = old_start - len_bytes;

  // init part retains the bytes.
  err = qbuffer_init_part(&part, bytes, skip_bytes, len_bytes, old_start);
  if( err ) return err;

  err = deque_push_front(sizeof(qbuffer_part_t), &buf->deque, &part);
  if( err ) {
    qbytes_release(bytes); // release the bytes.
    return err;
  }

  buf->offset_start = new_start;

  // invalidate cached entries.
  qbuffer_clear_cached(buf);

  return 0;
}

void qbuffer_trim_front(qbuffer_t* buf, int64_t remove_bytes)
{
  int64_t new_start = buf->offset_start + remove_bytes;

  if( remove_bytes == 0 ) return;

  assert( remove_bytes > 0 );
  assert( new_start <= buf->offset_end );

  while( deque_size(sizeof(qbuffer_part_t), &buf->deque) > 0 ) {
    // Get the first part.
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr( sizeof(qbuffer_part_t), deque_begin(& buf->deque));

    if( qbp->end_offset - qbp->len_bytes < new_start ) {
      // we might remove it entirely, or maybe
      // we just adjust its length and skip.
      if( qbp->end_offset <= new_start ) {
        qbytes_t* bytes = qbp->bytes;
        // ends entirely before new_start, remove the chunk.
        // Remove it from the deque
        deque_pop_front(sizeof(qbuffer_part_t), &buf->deque);
        // release the bytes.
        qbytes_release(bytes);
      } else {
        // Keep only a part of this chunk.
        int64_t remove_here = new_start - (qbp->end_offset - qbp->len_bytes);
        qbp->skip_bytes += remove_here;
        qbp->len_bytes -= remove_here;
        break; // this is the last one.
      }
    } else {
      break; // we're past
    }
  }

  // Now set the new offset.
  buf->offset_start = new_start;

  // invalidate cached entries.
  qbuffer_clear_cached(buf);
}

void qbuffer_trim_back(qbuffer_t* buf, int64_t remove_bytes)
{
  int64_t new_end = buf->offset_end - remove_bytes;

  if( remove_bytes == 0 ) return;
  assert( remove_bytes > 0 );
  assert( new_end >= buf->offset_start );

  // Go through the deque removing entire parts.
  while( deque_size(sizeof(qbuffer_part_t), &buf->deque) > 0 ) {
    // Get the last part.
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr( sizeof(qbuffer_part_t), deque_last(sizeof(qbuffer_part_t), &buf->deque));

    if( qbp->end_offset > new_end ) {
      // we might remove it entirely, or maybe
      // we just adjust its length and skip.
      if( qbp->end_offset - qbp->len_bytes >= new_end ) {
        qbytes_t* bytes = qbp->bytes;
        // starts entirely after new_end, remove the chunk.
        // Remove it from the deque
        deque_pop_front(sizeof(qbuffer_part_t), &buf->deque);
        // release the bytes.
        qbytes_release(bytes);
      } else {
        // Keep only a part of this chunk.
        int64_t remove_here = qbp->end_offset - new_end;
        qbp->len_bytes -= remove_here;
        qbp->end_offset -= remove_here;
        break; // this is the last one.
      }
    } else {
      break; // we're past
    }
  }

  // Now set the new offset.
  buf->offset_end = new_end;

  // invalidate cached entries.
  qbuffer_clear_cached(buf);
}

qioerr qbuffer_pop_front(qbuffer_t* buf)
{
  qbytes_t* bytes;
  int64_t skip;
  int64_t len;
  qbuffer_iter_t chunk;

  if ( qbuffer_num_parts(buf) == 0 ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "cannot pop from empty buffer");

  chunk = qbuffer_begin(buf);

  qbuffer_iter_get(chunk, qbuffer_end(buf), &bytes, &skip, &len);

  deque_pop_front(sizeof(qbuffer_part_t), &buf->deque);

  buf->offset_start += len;

  return 0;
}

qioerr qbuffer_pop_back(qbuffer_t* buf)
{
  qbytes_t* bytes;
  int64_t skip;
  int64_t len;
  qbuffer_iter_t chunk;

  if ( qbuffer_num_parts(buf) == 0 ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "cannot pop from empty buffer");

  chunk = qbuffer_end(buf);
  qbuffer_iter_prev_part(buf, &chunk);

  qbuffer_iter_get(chunk, qbuffer_end(buf), &bytes, &skip, &len);

  deque_pop_back(sizeof(qbuffer_part_t), &buf->deque);

  buf->offset_end -= len;

  return 0;
}

void qbuffer_reposition(qbuffer_t* buf, int64_t new_offset_start)
{
  deque_iterator_t start = deque_begin(& buf->deque);
  deque_iterator_t end = deque_end(& buf->deque);
  deque_iterator_t iter;
  qbuffer_part_t* qbp;
  int64_t diff;

  diff = new_offset_start - buf->offset_start;
  buf->offset_start += diff;
  buf->offset_end += diff;

  iter = start;

  while( ! deque_it_equals(iter, end) ) {
    qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter);
    qbp->end_offset += diff;
  }
}

qbuffer_iter_t qbuffer_begin(qbuffer_t* buf)
{
  qbuffer_iter_t ret;
  ret.offset = buf->offset_start;
  ret.iter = deque_begin(& buf->deque);
  return ret;
}

qbuffer_iter_t qbuffer_end(qbuffer_t* buf)
{
  qbuffer_iter_t ret;
  ret.offset = buf->offset_end;
  ret.iter = deque_end( & buf->deque );
  return ret;
}

void qbuffer_iter_next_part(qbuffer_t* buf, qbuffer_iter_t* iter)
{
  deque_iterator_t d_end = deque_end( & buf->deque );

  deque_it_forward_one(sizeof(qbuffer_part_t), & iter->iter);

  if( deque_it_equals(iter->iter, d_end) ) {
    // if we're not at the end now... offset is from buf
    iter->offset = buf->offset_end;
  } else {
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter->iter);
    iter->offset = qbp->end_offset - qbp->len_bytes;
  }
}

void qbuffer_iter_prev_part(qbuffer_t* buf, qbuffer_iter_t* iter)
{
  qbuffer_part_t* qbp;

  deque_it_back_one(sizeof(qbuffer_part_t), & iter->iter);

  qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter->iter);
  iter->offset = qbp->end_offset - qbp->len_bytes;
}

void qbuffer_iter_floor_part(qbuffer_t* buf, qbuffer_iter_t* iter)
{
  deque_iterator_t d_start = deque_begin( & buf->deque );
  deque_iterator_t d_end = deque_end( & buf->deque );

  if( deque_it_equals(iter->iter, d_end) ) {
    if( deque_it_equals(iter->iter, d_start) ) {
      // We're at the beginning. Do nothing.
      return;
    }

    // If we're at the end, just go back one.
    deque_it_back_one(sizeof(qbuffer_part_t), & iter->iter);
  }

  {
    // Now, just set the offset appropriately.
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter->iter);
    iter->offset = qbp->end_offset - qbp->len_bytes;
  }
}


void qbuffer_iter_ceil_part(qbuffer_t* buf, qbuffer_iter_t* iter)
{
  deque_iterator_t d_end = deque_end( & buf->deque );

  if( deque_it_equals(iter->iter, d_end) ) {
    // We're at the end. Do nothing.
  } else {
    qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter->iter);
    iter->offset = qbp->end_offset;
    deque_it_forward_one(sizeof(qbuffer_part_t), & iter->iter);
  }
}


/* Advances an iterator using linear search.
 */
void qbuffer_iter_advance(qbuffer_t* buf, qbuffer_iter_t* iter, int64_t amt)
{
  deque_iterator_t d_begin = deque_begin( & buf->deque );
  deque_iterator_t d_end = deque_end( & buf->deque );

  if( amt >= 0 ) {
    // forward search.
    iter->offset += amt;
    while( ! deque_it_equals(iter->iter, d_end) ) {
      qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter->iter);
      if( iter->offset < qbp->end_offset ) {
        // it's in this one.
        return;
      }
      deque_it_forward_one(sizeof(qbuffer_part_t), & iter->iter);
    }
    // If we get here, we didn't find it. Return the buffer end.
    *iter = qbuffer_end(buf);
  } else {
    // backward search.
    iter->offset += amt; // amt is negative

    if( ! deque_it_equals( iter->iter, d_end ) ) {
      // is it within the current buffer?
      qbuffer_part_t* qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter->iter);
      if( iter->offset >= qbp->end_offset - qbp->len_bytes ) {
        // it's in this one.
        return;
      }
    }

    // now we have a valid deque element.
    do {
      qbuffer_part_t* qbp;

      deque_it_back_one(sizeof(qbuffer_part_t), & iter->iter);

      qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter->iter);
      if( iter->offset >= qbp->end_offset - qbp->len_bytes ) {
        // it's in this one.
        return;
      }
    } while( ! deque_it_equals(iter->iter, d_begin) );
    // If we get here, we didn't find it. Return the buffer start.
    *iter = qbuffer_begin(buf);
  }
}




// find buffer iterator part in logarithmic time
// finds an offset in the window [offset_start,offset_end]
// (in other words, offset might not start at 0)
qbuffer_iter_t qbuffer_iter_at(qbuffer_t* buf, int64_t offset)
{
  qbuffer_iter_t ret;
  deque_iterator_t first = deque_begin(& buf->deque);
  deque_iterator_t last = deque_end(& buf->deque);
  deque_iterator_t middle;
  qbuffer_part_t* qbp;
  ssize_t num_parts = deque_it_difference(sizeof(qbuffer_part_t), last, first);
  ssize_t half;

  while( num_parts > 0 ) {
    half = num_parts >> 1;
    middle = first;

    deque_it_forward_n(sizeof(qbuffer_part_t), &middle, half);

    qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), middle);
    if( offset < qbp->end_offset ) {
      num_parts = half;
    } else {
      first = middle;
      deque_it_forward_one(sizeof(qbuffer_part_t), &first);
      num_parts = num_parts - half - 1;
    }
  }

  if( deque_it_equals(first, last) ) {
    ret = qbuffer_end(buf);
  } else {
    qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), first);
    if( offset < qbp->end_offset - qbp->len_bytes ) {
      ret = qbuffer_begin(buf);
    } else {
      ret.offset = offset;
      ret.iter = first;
    }
  }
  return ret;
}

qioerr qbuffer_to_iov(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end,
                     size_t max_iov, struct iovec *iov_out,
                     qbytes_t** bytes_out /* can be NULL */,
                     size_t *iovcnt_out)
{
  deque_iterator_t d_end = deque_end(& buf->deque);
  deque_iterator_t iter;
  qbuffer_part_t* qbp;
  size_t i = 0;

  iter = start.iter;

  // invalid range!
  if( start.offset > end.offset ) {
    *iovcnt_out = 0;
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "invalid range");
  }

  if( deque_it_equals(iter, d_end) ) {
    // start is actually pointing to the end of the deque. no data.
    *iovcnt_out = 0;
    return 0;
  }

  if( deque_it_equals(iter, end.iter) ) {
    // we're only pointing to a single block.
    qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter);
    if( i >= max_iov ) goto error_nospace;
    iov_out[i].iov_base = PTR_ADDBYTES(qbp->bytes->data, qbp->skip_bytes + (start.offset - (qbp->end_offset - qbp->len_bytes)));
    iov_out[i].iov_len = end.offset - start.offset;
    if( bytes_out ) bytes_out[i] = qbp->bytes;
    if( iov_out[i].iov_len > 0 ) i++;
  } else {
    // otherwise, there's a possibly partial block in start.
    qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter);
    if( i >= max_iov ) goto error_nospace;
    iov_out[i].iov_base = PTR_ADDBYTES(qbp->bytes->data, qbp->skip_bytes + (start.offset - (qbp->end_offset - qbp->len_bytes)));
    iov_out[i].iov_len = qbp->end_offset - start.offset;
    if( bytes_out ) bytes_out[i] = qbp->bytes;
    // store it if we had any data there.
    if( iov_out[i].iov_len > 0 ) i++;


    // Now, on to the next.
    deque_it_forward_one(sizeof(qbuffer_part_t), &iter);

    // until we get to the same block as end, we need to store full blocks.
    while( ! deque_it_equals( iter, end.iter ) ) {
      if( deque_it_equals( iter, d_end ) ) {
        // error: end is not in deque.
        *iovcnt_out = 0;
        QIO_RETURN_CONSTANT_ERROR(EINVAL, "end is not in deque");
      }

      qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter);
      if( i >= max_iov ) goto error_nospace;
      iov_out[i].iov_base = PTR_ADDBYTES(qbp->bytes->data, qbp->skip_bytes);
      iov_out[i].iov_len = qbp->len_bytes;
      if( bytes_out ) bytes_out[i] = qbp->bytes;
      // store it if we had any data there.
      if( iov_out[i].iov_len > 0 ) i++;

      // Now, on to the next.
      deque_it_forward_one(sizeof(qbuffer_part_t), &iter);
    }

    // at the end of the loop
    // is there any data in end?
    if( deque_it_equals(iter, d_end) ) {
      // we're currently pointing to the end; no need to add more.
    } else {
      qbp = (qbuffer_part_t*) deque_it_get_cur_ptr(sizeof(qbuffer_part_t), iter);
      // add a partial end block. We know it's different from
      // start since we handled that above.
      if( i >= max_iov ) goto error_nospace;
      iov_out[i].iov_base = PTR_ADDBYTES(qbp->bytes->data, qbp->skip_bytes);
      iov_out[i].iov_len = end.offset - (qbp->end_offset - qbp->len_bytes);
      if( bytes_out ) bytes_out[i] = qbp->bytes;
      if( iov_out[i].iov_len > 0 ) i++;
    }
  }

  *iovcnt_out = i;
  return 0;

error_nospace:
  *iovcnt_out = 0;
  // EOVERFLOW or ENOBUFS would make sense too
  QIO_RETURN_CONSTANT_ERROR(EMSGSIZE, "no space in buffer");
}

qioerr qbuffer_flatten(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, qbytes_t** bytes_out)
{
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i,j;
  qbytes_t* ret;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;

  if( num_bytes < 0 || num_parts < 0 ||
      start.offset < buf->offset_start || end.offset > buf->offset_end ) {
    *bytes_out = 0;
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");
  }

  err = qbytes_create_calloc(&ret, num_bytes);
  if( err ) {
    *bytes_out = 0;
    return err;
  }

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) {
    // The buffer was successfully allocated, so we have to release it here.
    qbytes_release(ret);
    *bytes_out = 0;
    return QIO_ENOMEM;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) {
    MAYBE_STACK_FREE(iov, iov_onstack);
    // The buffer was successfully allocated, so we have to release it here.
    qbytes_release(ret);
    *bytes_out = 0;
    return err;
  }

  j = 0;
  for( i = 0; i < iovcnt; i++ ) {
    qio_memcpy(PTR_ADDBYTES(ret->data, j), iov[i].iov_base, iov[i].iov_len);
    j += iov[i].iov_len;
  }

  MAYBE_STACK_FREE(iov, iov_onstack);

  *bytes_out = ret;
  return 0;
}

/*
qioerr qbuffer_clone(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, qbuffer_ptr_t* buf_out)
{
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  qbytes_t** bytes = NULL;
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  MAYBE_STACK_SPACE(qbytes_t*, bytes_onstack);
  qioerr err;
  qbuffer_ptr_t ret = NULL;

  if( num_bytes < 0 || num_parts < 0 || start.offset < buf->offset_start || end.offset > buf->offset_end ) return EINVAL;

  err = qbuffer_create(&ret);
  if( err ) return err;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  MAYBE_STACK_ALLOC(qbytes_t*, num_parts, bytes, bytes_onstack);
  if( ! iov || ! bytes ) {
    err = QIO_ENOMEM;
    goto error;
  }

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, bytes, &iovcnt);
  if( err ) goto error;

  // now append them all to our present buffer.
  for( i = 0; i < iovcnt; i++ ) {
    int64_t skip = PTR_DIFFBYTES(iov[i].iov_base, bytes[i]->data);
    int64_t len = iov[i].iov_len;

    if( skip < 0 || skip + len > bytes[i]->len ) {
      err = EINVAL;
      goto error;
    }

    qbuffer_append(ret, bytes[i], skip, len);
  }

  MAYBE_STACK_FREE(iov, iov_onstack);
  MAYBE_STACK_FREE(bytes, bytes_onstack);

  *buf_out = ret;
  return 0;

error:
  MAYBE_STACK_FREE(iov, iov_onstack);
  MAYBE_STACK_FREE(bytes, bytes_onstack);
  qbuffer_destroy(ret);
  return err;
}
*/

qioerr qbuffer_copyout(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, void* ptr, size_t ret_len)
{
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i,j;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;

  if( num_bytes < 0 || num_parts < 0 ||
      start.offset < buf->offset_start || end.offset > buf->offset_end ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");
  }

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) return QIO_ENOMEM;

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  j = 0;
  for( i = 0; i < iovcnt; i++ ) {
    if( j + iov[i].iov_len > ret_len ) goto error_nospace;
    qio_memcpy(PTR_ADDBYTES(ptr, j), iov[i].iov_base, iov[i].iov_len);
    j += iov[i].iov_len;
  }

  MAYBE_STACK_FREE(iov, iov_onstack);
  return 0;

error_nospace:
  QIO_GET_CONSTANT_ERROR(err, EMSGSIZE, "no space in buffer");
error:
  MAYBE_STACK_FREE(iov, iov_onstack);
  return err;
}

qioerr qbuffer_copyin(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, const void* ptr, size_t ret_len)
{
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i,j;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;

  if( num_bytes < 0 || num_parts < 0 ||
      start.offset < buf->offset_start || end.offset > buf->offset_end ) {
    QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");
  }

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) return QIO_ENOMEM;

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  j = 0;
  for( i = 0; i < iovcnt; i++ ) {
    if( j + iov[i].iov_len > ret_len ) goto error_nospace;
    qio_memcpy(iov[i].iov_base, PTR_ADDBYTES(ptr, j), iov[i].iov_len);
    j += iov[i].iov_len;
  }

  MAYBE_STACK_FREE(iov, iov_onstack);
  return 0;

error_nospace:
  QIO_GET_CONSTANT_ERROR(err, EMSGSIZE, "no space in buffer");
error:
  MAYBE_STACK_FREE(iov, iov_onstack);
  return err;
}

qioerr qbuffer_copyin_buffer(qbuffer_t* dst, qbuffer_iter_t dst_start, qbuffer_iter_t dst_end,
                            qbuffer_t* src, qbuffer_iter_t src_start, qbuffer_iter_t src_end)
{
  int64_t dst_num_bytes = qbuffer_iter_num_bytes(dst_start, dst_end);
  ssize_t dst_num_parts = qbuffer_iter_num_parts(dst_start, dst_end);
  int64_t src_num_bytes = qbuffer_iter_num_bytes(src_start, src_end);
  ssize_t src_num_parts = qbuffer_iter_num_parts(src_start, src_end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;
  qbuffer_iter_t dst_cur, dst_cur_end;

  if( dst == src ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "cannot copy a buffer to itself");

  if( dst_num_bytes < 0 || dst_num_parts < 0 || dst_start.offset < dst->offset_start || dst_end.offset > dst->offset_end ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "dst range outside of buffer");
  if( src_num_bytes < 0 || src_num_parts < 0 || src_start.offset < src->offset_start || src_end.offset > src->offset_end ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "src range outside of buffer");

  MAYBE_STACK_ALLOC(struct iovec, src_num_parts, iov, iov_onstack);
  if( ! iov ) return QIO_ENOMEM;

  err = qbuffer_to_iov(src, src_start, src_end, src_num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  dst_cur = dst_start;
  for( i = 0; i < iovcnt; i++ ) {
    dst_cur_end = dst_cur;
    qbuffer_iter_advance(dst, &dst_cur_end, iov[i].iov_len);
    err = qbuffer_copyin(dst, dst_cur, dst_cur_end, iov[i].iov_base, iov[i].iov_len);
    if( err ) goto error;
    dst_cur = dst_cur_end;
  }

  MAYBE_STACK_FREE(iov, iov_onstack);
  return 0;

error:
  MAYBE_STACK_FREE(iov, iov_onstack);
  return err;
}

qioerr qbuffer_memset(qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end, unsigned char byte)
{
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;

  if( num_bytes < 0 || num_parts < 0 || start.offset < buf->offset_start || end.offset > buf->offset_end ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) return QIO_ENOMEM;

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  for( i = 0; i < iovcnt; i++ ) {
    memset(iov[i].iov_base, byte, iov[i].iov_len);
  }

  MAYBE_STACK_FREE(iov, iov_onstack);
  return 0;

error:
  MAYBE_STACK_FREE(iov, iov_onstack);
  return err;
}
