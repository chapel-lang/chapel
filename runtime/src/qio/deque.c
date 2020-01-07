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


#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "deque.h"

void debug_print_deque_iter(deque_iterator_t* it)
{
  fprintf(stderr, "deque_it %p: %p %p %p %p\n", it, it->cur, it->first, it->last, it->node);
}

void _deque_map_copy_forward(deque_node_t* start, deque_node_t* end, deque_node_t* dst)
{
  deque_node_t* cur;
  for( cur = start; cur < end; ++cur ) {
    *dst = *cur;
    dst++;
  }
}
void _deque_map_copy_backward(deque_node_t* start, deque_node_t* end, deque_node_t* dst_end)
{
  deque_node_t* cur;
  for( cur = end - 1; cur >= start; --cur ) {
    dst_end--;
    *dst_end = *cur;
  }
}


qioerr _deque_reallocate_map(const ssize_t item_size, const ssize_t buf_size, deque_t* d, ssize_t nodes_to_add, char add_at_front) {
  const ssize_t old_num_nodes = d->finish.node - d->start.node + 1;
  const ssize_t new_num_nodes = old_num_nodes + nodes_to_add;

  deque_node_t* new_nstart; 

  if( d->map_size > 2 * new_num_nodes ) {
    new_nstart = d->map + (d->map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0 );
    if( new_nstart < d->start.node ) {
      // copy from d->start.node to d->finish.node + 1 to new_nstart
      _deque_map_copy_forward(d->start.node, d->finish.node + 1, new_nstart);
    } else {
      // copy backward from d->start.node to d->finish.node + 1 to new_nstart + old_num_nodes elements
      _deque_map_copy_backward(d->start.node, d->finish.node + 1, new_nstart + old_num_nodes);
    }
  } else {
    ssize_t new_map_size = d->map_size + _DEQUE_MAX(d->map_size, nodes_to_add) + 2;
    deque_node_t* new_map = (deque_node_t*) deque_calloc(new_map_size, sizeof(deque_node_t));
    if( ! new_map ) return QIO_ENOMEM;

    new_nstart = new_map + (new_map_size - new_num_nodes ) / 2 + (add_at_front ? nodes_to_add : 0 );
    // copy from d->start.node to d->finish.node + 1 to new_nstart 
    _deque_map_copy_forward(d->start.node, d->finish.node + 1, new_nstart);
    deque_free(d->map);

    d->map = new_map;
    d->map_size = new_map_size;
  }
  _deque_set_node(item_size, buf_size, & d->start, new_nstart);
  _deque_set_node(item_size, buf_size, & d->finish, new_nstart + old_num_nodes - 1);
  return 0;
}


