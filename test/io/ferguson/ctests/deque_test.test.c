#include "deque.h"
#include <assert.h>
#include <stdio.h>

void test_reallocate(void)
{
  // hit the different reallocate cases.
  // map_size > 2* num_new_nodes
  // add to front, add to back

  {
    // start with copy forward/copy backward.
    deque_node_t nodes[4];

    nodes[0].data = (void*) 0x100;
    nodes[1].data = (void*) 0x101;
    nodes[2].data = (void*) 0x102;
    nodes[3].data = (void*) 0x103;

    // test overlapping copy forward.
    // dstBeg should be before srcBegin
    _deque_map_copy_forward( &nodes[2], &nodes[4], &nodes[1]);
    assert(nodes[0].data == (void*) 0x100);
    assert(nodes[1].data == (void*) 0x102);
    assert(nodes[2].data == (void*) 0x103);
    assert(nodes[3].data == (void*) 0x103);


    nodes[0].data = (void*) 0x100;
    nodes[1].data = (void*) 0x101;
    nodes[2].data = (void*) 0x102;
    nodes[3].data = (void*) 0x103;


    // test overlapping copy backward.
    // dstEnd should be after srcEnd
    _deque_map_copy_backward( &nodes[0], &nodes[2], &nodes[3]);
    assert(nodes[0].data == (void*) 0x100);
    assert(nodes[1].data == (void*) 0x100);
    assert(nodes[2].data == (void*) 0x101);
    assert(nodes[3].data == (void*) 0x103);
  }

  // Now, suppose we've allocated our stuff...
  // let's reallocate.
  {
    deque_t d;
    qioerr err;
    ssize_t saved_size;
    deque_node_t saved_nodes[2*_DEQUE_INITIAL_MAP_SIZE];
    deque_node_t* cur;
    int i;

    // this time, try with map_size < 2*num_new_nodes
    err = deque_init(16, &d, 16*_DEQUE_INITIAL_MAP_SIZE);
    assert(!err);

    // save the pointers.
    assert(d.map_size < 2*_DEQUE_INITIAL_MAP_SIZE);// fits in test buffer
    saved_size = 0;
    for( cur = d.start.node; cur < d.finish.node; ++cur ) {
      saved_nodes[saved_size++].data = cur->data;
    }

    _deque_reallocate_map(16, 512, /* first two args dont matter in this test*/
                          &d, 1, 0 /* not at front */);
    assert(d.map_size > saved_size); // make sure it grew
    i = 0;
    for( cur = d.start.node; cur < d.finish.node && i < saved_size; ++cur ) {
      assert(saved_nodes[i++].data == cur->data);
    } 
    // don't care value of nodes[saved_size], the new slot.

    deque_destroy(&d);

    err = deque_init(16, &d, 16*_DEQUE_INITIAL_MAP_SIZE);
    assert(!err);

    assert(d.map_size < 2*_DEQUE_INITIAL_MAP_SIZE);// fits in test buffer
    saved_size = 0;
    for( cur = d.start.node; cur < d.finish.node; ++cur ) {
      saved_nodes[saved_size++].data = cur->data;
    }

    _deque_reallocate_map(16, 512, /* first two args dont matter in this test*/
                          &d, 1, 1 /* at front */);
    assert(d.map_size > saved_size); // make sure it grew
    // don't care value of nodes[0], the new slot.
    i = 0;
    for( cur = d.start.node; cur < d.finish.node && i < saved_size ; ++cur ) {
      assert(saved_nodes[i++].data == cur->data);
    } 
    // don't care of value of nodes[saved_size+1], the previous new slot.

    deque_destroy(&d);

    // this time, try with map_size > 2*num_new_nodes
    err = deque_init(16, &d, 0);
    assert(!err);

    // save the pointers.
    assert(d.map_size < 2*_DEQUE_INITIAL_MAP_SIZE);// fits in test buffer
    saved_size = 0;
    for( cur = d.start.node; cur < d.finish.node; ++cur ) {
      saved_nodes[saved_size++].data = cur->data;
    }

    _deque_reallocate_map(16, 512, /* first two args dont matter in this test*/
                          &d, 1, 0 /* not at front */);
    i = 0;
    for( cur = d.start.node; cur < d.finish.node; ++cur ) {
      assert(saved_nodes[i++].data == cur->data);
    }
    assert( i == saved_size );

    deque_destroy(&d);

    err = deque_init(16, &d, 0);
    assert(!err);

    // save the pointers.
    assert(d.map_size < 2*_DEQUE_INITIAL_MAP_SIZE);// fits in test buffer
    saved_size = 0;
    for( cur = d.start.node; cur < d.finish.node; ++cur ) {
      saved_nodes[saved_size++].data = cur->data;
    }

    _deque_reallocate_map(16, 512, /* first two args dont matter in this test*/
                          &d, 1, 1 /* at front */);
    i = 0;
    for( cur = d.start.node; cur < d.finish.node; ++cur ) {
      assert(saved_nodes[i++].data == cur->data);
    }
    assert( i == saved_size );

    deque_destroy(&d);
  }
}

void print_deque(size_t item_size, deque_t* d)
{
  deque_iterator_t cur = deque_begin(d);
  deque_iterator_t end = deque_end(d);
  int j;
  assert(item_size == 4);
  printf("deque %p: ", d);
  while( ! deque_it_equals(cur, end) ) {
    j = 0;
    deque_it_get_cur(item_size, cur, &j);
    printf(" %i", j);
    deque_it_forward_one(item_size, &cur);
  }
  printf("\n");
}
void iterate_counting(size_t item_size, deque_t* d, int num)
{
  deque_iterator_t begin = deque_begin(d);
  deque_iterator_t end = deque_end(d);
  deque_iterator_t cur = begin;
  deque_iterator_t test;
  int i = 0;
  int j;
  assert(item_size == 4);
  while( i < num && ! deque_it_equals(cur, end) ) {
    // check that the value is right.
    deque_it_get_cur(item_size, cur, &j);
    assert( j == i );
    
    // check that advancing from beginning i elements takes us here.
    test = begin;
    deque_it_forward_n(item_size, &test, i);
    assert( deque_it_equals( test, cur ) );

    // advance to the next element, checking that
    // forward_one is the same as forward_n(1) 
    test = cur;
    deque_it_forward_one(item_size, &cur);
    deque_it_forward_n(item_size, &test, 1);
    assert( deque_it_equals( test, cur ) );
    i++;
  }
}
void iterate_counting_reverse(size_t item_size, deque_t* d, int num)
{
  deque_iterator_t cur;
  deque_iterator_t first = deque_begin(d);
  deque_iterator_t last;
  deque_iterator_t test;
  int i = 0;
  int j;

  assert(item_size == 4);

  if( num == 0 ) return;
  if( deque_size(item_size, d) == 0 ) return;

  last = deque_last(item_size, d);
  cur = last;
  do {
    // check that the value is right.
    deque_it_get_cur(item_size, cur, &j);
    assert( j == i );

    // check that advancing from end -i elements takes us here.
    test = last;
    deque_it_forward_n(item_size, &test, - i);
    assert( deque_it_equals( test, cur ) );

    // advance to the next element, cheking that
    // back_one is the same as forward_n(-1)
    test = cur;
    deque_it_back_one(item_size, &cur);
    deque_it_forward_n(item_size, &test, -1);
    assert( deque_it_equals( test, cur ) );
    i++;
  } while ( i < num && ! deque_it_equals(cur,first) ); 
}

void push_front_counting(size_t item_size, deque_t* d, int num)
{
  int i;
  assert(item_size == 4);
  for( i = 0; i < num; i++ ) {
    deque_push_front(4, d, &i);
  }
}
void push_back_counting(size_t item_size, deque_t* d, int num)
{
  int i;
  assert(item_size == 4);
  for( i = 0; i < num; i++ ) {
    deque_push_back(4, d, &i);
  }
}
void push_both_counting(size_t item_size, deque_t* d, int num)
{
  int i;
  assert(item_size == 4);
  for( i = 0; i < num; i++ ) {
    deque_push_front(4, d, &i);
    deque_push_back(4, d, &i);
  }
}

void pop_back_check_constant(size_t item_size, deque_t* d, int num, int val)
{
  int i;
  int j;
  ssize_t sz = deque_size(item_size, d);
  assert(item_size == 4);
  assert(sz >= num);
  for( i = 0; i < num; i++ ) {
    assert(deque_size(item_size, d) > 0);
    j = 0;
    deque_it_get_cur(4, deque_last(item_size, d), &j);
    assert( j == val );
    deque_pop_back(4, d);
  }
}


void pop_front_check_constant(size_t item_size, deque_t* d, int num, int val)
{
  int i;
  int j;
  ssize_t sz = deque_size(item_size, d);
  assert(item_size == 4);
  assert(sz >= num);
  for( i = 0; i < num; i++ ) {
    assert(deque_size(item_size, d) > 0);
    j = 0;
    deque_it_get_cur(4, deque_begin(d), &j);
    assert( j == val );
    deque_pop_front(4, d);
  }
}

void pop_front_check_counting(size_t item_size, deque_t* d, int num)
{
  int i;
  int j;
  ssize_t sz = deque_size(item_size, d);
  assert(item_size == 4);
  assert(sz >= num);
  for( i = 0; i < num; i++ ) {
    assert(deque_size(item_size, d) > 0);
    j = 0;
    deque_it_get_cur(4, deque_begin(d), &j);
    assert( j == i );
    deque_pop_front(4, d);
  }
}

void pop_front_check_counting_reverse(size_t item_size, deque_t* d, int num)
{
  int i;
  int j;
  ssize_t sz = deque_size(item_size, d);
  assert(item_size == 4);
  assert(sz >= num);
  for( i = num - 1; i >= 0; i-- ) {
    assert(deque_size(item_size, d) > 0);
    j = 0;
    deque_it_get_cur(4, deque_begin(d), &j);
    assert( j == i );
    deque_pop_front(4, d);
  }
}

void pop_back_check_counting(size_t item_size, deque_t* d, int num)
{
  int i;
  int j;
  ssize_t sz = deque_size(item_size, d);
  assert(item_size == 4);
  assert(sz >= num);
  for( i = 0; i < num; i++ ) {
    assert(deque_size(item_size, d) > 0);
    j = 0;
    deque_it_get_cur(4, deque_last(item_size, d), &j);
    //printf("Got %i\n", j);
    assert( j == i );
    deque_pop_back(4, d);
  }
}

void pop_back_check_counting_reverse(size_t item_size, deque_t* d, int num)
{
  int i;
  int j;
  ssize_t sz = deque_size(item_size, d);
  assert(item_size == 4);
  assert(sz >= num);
  for( i = num - 1; i >= 0; i-- ) {
    assert(deque_size(item_size, d) > 0);
    j = 0;
    deque_it_get_cur(4, deque_last(item_size, d), &j);
    assert( j == i );
    deque_pop_back(4, d);
  }
}

void check_empty(size_t item_size, deque_t* d)
{
  ssize_t sz = deque_size(item_size, d);
  assert(item_size == 4);
  assert( sz == 0 );
}

int lots = 1 << 15;

void test_deque(int lots)
{
  deque_t d;
  // test: space for zeros.
  deque_init(4, &d, 0);
  check_empty(4, &d);
  deque_destroy(&d);

  // test: space for zeros.
  deque_init(4, &d, 10);
  pop_back_check_constant(4, &d, 10, 0);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: space for zeros.
  deque_init(4, &d, 10);
  pop_front_check_constant(4, &d, 10, 0);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push back lots, destroy
  deque_init(4, &d, 0);
  push_back_counting(4, &d, lots);
  iterate_counting(4, &d, lots);
  deque_destroy(&d);
  // test: push back lots, pop front lots.
  deque_init(4, &d, 0);
  push_back_counting(4, &d, lots);
  iterate_counting(4, &d, lots);
  pop_front_check_counting(4, &d, lots);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push back lots, pop back lots.
  deque_init(4, &d, 0);
  push_back_counting(4, &d, lots);
  pop_back_check_counting_reverse(4, &d, lots);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push front lots, pop front lots.
  deque_init(4, &d, 0);
  push_front_counting(4, &d, lots);
  iterate_counting_reverse(4, &d, lots);
  //print_deque(4, &d);
  pop_front_check_counting_reverse(4, &d, lots);
  //print_deque(4, &d);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push front lots, pop back lots.
  deque_init(4, &d, 0);
  push_front_counting(4, &d, lots);
  //print_deque(4, &d);
  pop_back_check_counting(4, &d, lots);
  //print_deque(4, &d);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push both lots, pop back lots, pop back lots.
  deque_init(4, &d, 0);
  push_both_counting(4, &d, lots);
  //print_deque(4, &d);
  pop_back_check_counting_reverse(4, &d, lots);
  pop_back_check_counting(4, &d, lots);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push both lots, pop front lots, pop front lots.
  deque_init(4, &d, 0);
  push_both_counting(4, &d, lots);
  //print_deque(4, &d);
  pop_front_check_counting_reverse(4, &d, lots);
  pop_front_check_counting(4, &d, lots);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push both lots, pop back lots, pop front lots
  deque_init(4, &d, 0);
  push_both_counting(4, &d, lots);
  //print_deque(4, &d);
  pop_back_check_counting_reverse(4, &d, lots);
  pop_front_check_counting_reverse(4, &d, lots);
  check_empty(4, &d);
  deque_destroy(&d);
  // test: push both lots, pop front lots, pop back lots
  deque_init(4, &d, 0);
  push_both_counting(4, &d, lots);
  //print_deque(4, &d);
  pop_front_check_counting_reverse(4, &d, lots);
  pop_back_check_counting_reverse(4, &d, lots);
  check_empty(4, &d);
  deque_destroy(&d);
}

int main(int argc, char** argv)
{
  int i;
  test_reallocate();
  {
    deque_t d;
    int lots = 2;
    deque_init(4, &d, 0);
    push_front_counting(4, &d, lots);
    //print_deque(4, &d);
    pop_back_check_counting(4, &d, lots);
    //print_deque(4, &d);
    check_empty(4, &d);
    deque_destroy(&d);
  }
  for( i = 0; i < 32; i++ ) {
    test_deque(i);
  }
  for( i = 127; i < 512; i++ ) {
    test_deque(i);
  }
  test_deque(1 << 15);

  printf("deque_test PASS\n");
  return 0;
}

