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

/* A C implementation based on the C++ deque in
 * stl_deque from gnu gcc C++ STL implementation.
 *
 * What used to be an STL parameter (really, item_size),
 * is now an argument to all of these functions, and
 * they're all inline. Users should have macros (or something)
 * to set the size, so that a given data structure does not
 * vary the size (as that is unsupported).
 *
 */

#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "sys_basic.h"
#include "qio_error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef CHPL_RT_UNIT_TEST
#include "chpl-mem.h"
#define deque_calloc(nmemb, size) chpl_mem_allocManyZero(nmemb, size, CHPL_RT_MD_IO_BUFFER, 0, 0)
#define deque_free(ptr) chpl_mem_free(ptr, 0, 0)
#define deque_memcpy(dest, src, num) chpl_memcpy(dest, src, num)
#else
#include "chpl-mem-sys.h"
#define deque_calloc(nmemb, size) sys_calloc(nmemb,size)
#define deque_free(ptr) sys_free(ptr)
#define deque_memcpy(dest, src, num) memcpy(dest, src, num)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct deque_node_s {
  void* data;
} deque_node_t;

typedef struct deque_iterator_s {
  void* cur; // element type
  void* first;
  void* last;
  deque_node_t* node;
} deque_iterator_t;

void debug_print_deque_iter(deque_iterator_t* it);

static inline
deque_iterator_t deque_iterator_null(void) {
  deque_iterator_t ret = {NULL, NULL, NULL, NULL};
  return ret;
}

typedef struct deque_s {
  deque_node_t* map;
  ssize_t map_size;
  deque_iterator_t start;
  deque_iterator_t finish;
} deque_t;

#define _DEQUE_BUFFER_SIZE 512
#define _DEQUE_INITIAL_MAP_SIZE 8

#define _DEQUE_MAX(a,b) (a>b?a:b)

// deque buffer size in items
static inline
ssize_t __deque_buf_size(const ssize_t item_size)
{
  return item_size < _DEQUE_BUFFER_SIZE ? (_DEQUE_BUFFER_SIZE / item_size) : 1;
}

/** 
 *  Prepares to traverse new_node.  Sets everything except
 *  _M_cur, which should therefore be set by the caller
 *  immediately afterwards, based on _M_first and _M_last.
 */
static inline
void _deque_set_node(const ssize_t item_size, const ssize_t buf_size, deque_iterator_t* it, deque_node_t* new_node)
{
  it->node = new_node;
  it->first = new_node->data;
  it->last = PTR_ADDBYTES(it->first, buf_size*item_size);
}

static inline
void deque_it_forward_one(const ssize_t item_size, deque_iterator_t* it)
{
  it->cur = PTR_ADDBYTES(it->cur, item_size);
  if( it->cur == it->last ) {
    _deque_set_node(item_size, __deque_buf_size(item_size), it, it->node + 1);
    it->cur = it->first;
  }
}

static inline
void deque_it_back_one(const ssize_t item_size, deque_iterator_t* it)
{
  if( it->cur == it->first ) {
    _deque_set_node(item_size, __deque_buf_size(item_size), it, it->node - 1);
    it->cur = it->last;
  }
  it->cur = PTR_ADDBYTES(it->cur, -item_size);
}

// advance n items.
static inline
void deque_it_forward_n(const ssize_t item_size, deque_iterator_t* it, ssize_t n)
{
  const ssize_t buf_size = __deque_buf_size(item_size);
  const ssize_t offset = n + PTR_DIFFBYTES(it->cur, it->first)/item_size;
  if( offset >= 0 && offset < buf_size ) {
    it->cur = PTR_ADDBYTES(it->cur, item_size * n);
  } else {
    const ssize_t node_offset =
      offset > 0 ? offset / buf_size
                 : -((-offset - 1) / buf_size ) - 1;
    _deque_set_node(item_size, __deque_buf_size(item_size), it, it->node + node_offset);
    it->cur = PTR_ADDBYTES(it->first, item_size * (offset - node_offset * buf_size));
  }
}

static inline
void deque_it_get_cur(const ssize_t item_size, const deque_iterator_t it, void* out)
{
  deque_memcpy(out, it.cur, item_size);
}

static inline
void* deque_it_get_cur_ptr(const ssize_t item_size, const deque_iterator_t it)
{
  return it.cur;
}

static inline
void deque_it_set_cur(const ssize_t item_size, const deque_iterator_t it, void* in)
{
  deque_memcpy(it.cur, in, item_size);
}

static inline
char deque_it_equals(const deque_iterator_t a, const deque_iterator_t  b)
{
  return a.cur == b.cur;
}

static inline
ssize_t deque_it_difference(const ssize_t item_size, const deque_iterator_t x, const deque_iterator_t y)
{
  ssize_t num_nodes = x.node - y.node - 1;
  ssize_t x_amt = PTR_DIFFBYTES(x.cur, x.first)/item_size;
  ssize_t y_amt = PTR_DIFFBYTES(y.last, y.cur)/item_size;
  return (__deque_buf_size(item_size)) * num_nodes + x_amt + y_amt;
}


static inline
void _deque_destroy_nodes(deque_node_t* start, deque_node_t* finish)
{
  deque_node_t* cur;
  for( cur = start; cur < finish; ++cur ) {
    // would call destructors on items in there!
    deque_free(cur->data);
    cur->data = NULL;
  }
}

static inline
qioerr _deque_create_nodes(const ssize_t item_size, deque_node_t* start, deque_node_t* finish)
{
  deque_node_t* cur;
  for( cur = start; cur < finish; ++cur ) {
    cur->data = deque_calloc( __deque_buf_size(item_size), item_size );
    if( ! cur->data ) {
      _deque_destroy_nodes(start, cur);
      return QIO_ENOMEM;
    }
  }

  return 0;
}

static inline
qioerr _deque_initialize_map(const ssize_t item_size, deque_t* d, ssize_t num_elements)
{
  const ssize_t buf_size = __deque_buf_size(item_size);
  const ssize_t num_nodes = (num_elements / buf_size) + 1;
  deque_node_t* nstart;
  deque_node_t* nfinish;
  qioerr err;

  if( num_elements < 0 ) QIO_RETURN_CONSTANT_ERROR(EINVAL, "negative num_elements");

  d->map_size = _DEQUE_MAX(_DEQUE_INITIAL_MAP_SIZE, num_nodes + 2);
  d->map = (deque_node_t*) deque_calloc(d->map_size, sizeof(deque_node_t));
  if( ! d->map ) {
    return QIO_ENOMEM;
  }

  // For "small" maps (needing less than _M_map_size nodes), allocation
  // starts in the middle elements and grows outwards.  So nstart may be
  // the beginning of _M_map, but for small maps it may be as far in as
  // _M_map+3.

  nstart = d->map + (d->map_size - num_nodes) / 2;
  nfinish = nstart + num_nodes;

  err = _deque_create_nodes(item_size, nstart, nfinish);
  if( err ) {
    deque_free(d->map);
    d->map = NULL;
    d->map_size = 0;
    return err;
  }

  _deque_set_node(item_size, buf_size, & d->start, nstart);
  _deque_set_node(item_size, buf_size, & d->finish, nfinish - 1);
  d->start.cur = d->start.first;
  d->finish.cur = PTR_ADDBYTES(d->finish.first, (num_elements*item_size) % buf_size);

  return 0;
}

static inline
qioerr deque_init(const ssize_t item_size, deque_t* d, ssize_t num_elements)
{
  return _deque_initialize_map(item_size, d, num_elements);
}

static inline
void deque_destroy(deque_t* d)
{
  _deque_destroy_nodes(d->start.node, d->finish.node + 1);
  deque_free(d->map);
  // Clear any pointers we had left over...
  memset(d, 0, sizeof(deque_t));
}

static inline
void deque_init_uninitialized(deque_t* d)
{
  d->map = NULL;
}

static inline
int deque_is_initialized(deque_t* d)
{
  return d->map != NULL;
}

static inline
deque_iterator_t deque_begin(deque_t* d)
{
  return d->start;
}

static inline
deque_iterator_t deque_end(deque_t* d)
{
  return d->finish;
}

static inline
deque_iterator_t deque_last(const ssize_t item_size, deque_t* d)
{
  deque_iterator_t ret;
  ret = d->finish;
  deque_it_back_one(item_size, &ret);
  return ret;
}


static inline
ssize_t deque_size(const ssize_t item_size, deque_t* d)
{
  return deque_it_difference(item_size, d->finish, d->start);
}

// These functions are in the C file.
void _deque_map_copy_forward(deque_node_t* start, deque_node_t* end, deque_node_t* dst);
void _deque_map_copy_backward(deque_node_t* start, deque_node_t* end, deque_node_t* dst_end);
qioerr _deque_reallocate_map(const ssize_t item_size, const ssize_t buf_size, deque_t* d, ssize_t nodes_to_add, char add_at_front);

static inline
qioerr _deque_reserve_map_at_back(const ssize_t item_size, deque_t* d, ssize_t nodes_to_add)
{
  if( nodes_to_add + 1 > d->map_size - (d->finish.node - d->map) ) {
    return _deque_reallocate_map(item_size, __deque_buf_size(item_size), d, nodes_to_add, 0);
  } else {
    return 0;
  }
}

static inline
qioerr _deque_reserve_map_at_front(const ssize_t item_size, deque_t* d, ssize_t nodes_to_add)
{
  if( nodes_to_add > (d->start.node - d->map) ) {
    return _deque_reallocate_map(item_size, __deque_buf_size(item_size), d, nodes_to_add, 1);
  } else {
    return 0;
  }
}

// Called only if _M_impl._M_finish._M_cur == _M_impl._M_finish._M_last - 1.
static inline
qioerr _deque_push_back_aux(const ssize_t item_size, deque_t* d, void* value)
{
  void *newdata;
  qioerr err;

  err = _deque_reserve_map_at_back(item_size, d, 1);
  if( err ) return err;

  newdata = deque_calloc( __deque_buf_size(item_size), item_size );
  if( !newdata ) {
    return QIO_ENOMEM;
  }

  (d->finish.node + 1)->data = newdata;

  //construct(d->finish.cur, v);
  deque_memcpy(d->finish.cur, value, item_size);

  _deque_set_node(item_size, __deque_buf_size(item_size), & d->finish, d->finish.node + 1);
  d->finish.cur = d->finish.first;

  return 0;
}

// Called only if _M_impl._M_start._M_cur == _M_impl._M_start._M_first.
static inline
qioerr _deque_push_front_aux(const ssize_t item_size, deque_t* d, void* value)
{
  void *newdata;
  qioerr err;

  err = _deque_reserve_map_at_front(item_size, d, 1);
  if( err ) return err;

  newdata = deque_calloc( __deque_buf_size(item_size), item_size );
  if( !newdata ) {
    return QIO_ENOMEM;
  }

  (d->start.node - 1)->data = newdata;

  _deque_set_node(item_size, __deque_buf_size(item_size), & d->start, d->start.node - 1);

  d->start.cur = PTR_ADDBYTES(d->start.last, -item_size);

  //construct(d->start.cur, v); // calls the constructor
  deque_memcpy(d->start.cur, value, item_size);

  return 0;
}

// Called only if _M_impl._M_finish._M_cur == _M_impl._M_finish._M_first.
static inline
void _deque_pop_back_aux(const ssize_t item_size, deque_t* d)
{
  //deallocate_node(d->finish.first);
  deque_free(d->finish.first);

  _deque_set_node(item_size, __deque_buf_size(item_size), & d->finish, d->finish.node - 1);
  d->finish.cur = PTR_ADDBYTES(d->finish.last, - item_size);
  // destroy(d->finish.cur); calls the destructor
  memset(d->finish.cur, 0, item_size);
}

// Called only if _M_impl._M_start._M_cur == _M_impl._M_start._M_last - 1.
// Note that if the deque has at least one element (a precondition for this
// member function), and if
//   _M_impl._M_start._M_cur == _M_impl._M_start._M_last,
// then the deque must have at least two nodes.
static inline
void _deque_pop_front_aux(const ssize_t item_size, deque_t* d)
{
  //destroy(d->start.cur); // calls the destructor
  memset(d->start.cur, 0, item_size);

  //deallocate_node(d->start.first);
  deque_free(d->start.first);

  _deque_set_node(item_size, __deque_buf_size(item_size), & d->start, d->start.node + 1);
  d->start.cur = d->start.first;
}

static inline
qioerr deque_push_front(const ssize_t item_size, deque_t* d, void* value)
{
  if( d->start.cur != d->start.first ) {
    //construct(d->start.cur - item_size, value);
    deque_memcpy(PTR_ADDBYTES(d->start.cur, -item_size), value, item_size);

    d->start.cur = PTR_ADDBYTES(d->start.cur, -item_size);

    return 0;
  } else {
    return _deque_push_front_aux(item_size, d, value);
  }
}

static inline
qioerr deque_push_back(const ssize_t item_size, deque_t* d, void* value)
{
  if( d->finish.cur != PTR_ADDBYTES(d->finish.last, -item_size) ) {
    //construct(d->finish.cur, value);
    deque_memcpy(d->finish.cur, value, item_size);

    d->finish.cur = PTR_ADDBYTES(d->finish.cur, item_size);

    return 0;
  } else {
    return _deque_push_back_aux(item_size, d, value);
  }
}

static inline
void deque_pop_front(const ssize_t item_size, deque_t* d)
{
  if( d->start.cur != PTR_ADDBYTES(d->start.last, -item_size) ) {
    //destroy(d->start.cur);
    memset(d->start.cur, 0, item_size);

    d->start.cur = PTR_ADDBYTES(d->start.cur, item_size);
  } else {
    _deque_pop_front_aux(item_size, d);
  }
}

static inline
void deque_pop_back(const ssize_t item_size, deque_t* d)
{
  if( d->finish.cur != d->finish.first ) {
    d->finish.cur = PTR_ADDBYTES(d->finish.cur, -item_size);

    //destroy(d->finish.cur);
    memset(d->finish.cur, 0, item_size);
  } else {
    _deque_pop_back_aux(item_size, d);
  }
}


#ifdef __cplusplus
} // end extern "C"
#endif

#endif
