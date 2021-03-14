#include "qbuffer.h"
#include <assert.h>

void test_qbytes(void)
{
  qbytes_t* b;
  qioerr err;

  // easiest test -- create and release.
  err = qbytes_create_iobuf(&b);
  assert(!err);

  qbytes_release(b);

  // test 2 -- create, retain, release, release.
  err = qbytes_create_iobuf(&b);
  assert(!err);

  qbytes_retain(b);

  qbytes_release(b);

  qbytes_release(b);

  // test 3 -- create a particular length.
  err = qbytes_create_calloc(&b, 2000);
  assert(!err);

  assert( b->len == 2000 );

  qbytes_release(b);
}

void test_qbuffer_iterators(qbuffer_t* buf, qbytes_t** qb, int num, int skip, int trunc)
{
  qbuffer_iter_t cur;
  qbuffer_iter_t search;
  qbuffer_iter_t start;
  qbuffer_iter_t end;
  int i,j;
  int64_t off;
  int64_t total_len;

  total_len = 0;
  for( i = 0; i < num; i++ ) {
    total_len += qb[i]->len - skip - trunc;
  }
  assert( total_len == qbuffer_len(buf) );

  start = qbuffer_begin(buf);
  end = qbuffer_end(buf);

  i = 0;
  cur = start;
  // now try iterating through them
  while( ! qbuffer_iter_same_part(cur, end) ) {
    int64_t skip, len;
    qbytes_t* bytes;

    qbuffer_iter_get(cur, end, &bytes, &skip, &len);
    assert( bytes == qb[i] );
    assert( skip == skip );
    assert( len == qb[i]->len - skip - trunc );

    qbuffer_iter_next_part(buf, &cur);

    i++;
  }
 
  // now try iterating through them backwards.
  if( ! qbuffer_iter_same_part(start, end) ) {
    i = num - 1;
    cur = end;
    do {
      int64_t skip, len;
      qbytes_t* bytes;

      qbuffer_iter_prev_part(buf, &cur);

      qbuffer_iter_get(cur, end, &bytes, &skip, &len);
      assert( bytes == qb[i] );
      assert( skip == skip );
      assert( len == qb[i]->len - skip - trunc );

      i--;
    } while( ! qbuffer_iter_same_part(cur, start) );
  }

  // check qbuffer_flatten, qbuffer_copyout, qbuffer_copyin, advance, get
  {
    qbytes_t* flat;
    qbytes_t* flat2;
    qbytes_t* zeros;
    qioerr err;

    err = qbuffer_flatten(buf, start, end, &flat);
    assert(!err);

    // check that flat has the right data in it.
    j = 0;
    for( i = 0; i < num; i++ ) {
      assert( 0 == memcmp((char*)flat->data + j, (char*)qb[i]->data + skip, qb[i]->len - skip - trunc) );
      j += qb[i]->len - skip - trunc;
    }
    assert( qbuffer_len(buf) == j );

    // Try flattening the buffer with advanced iterators.
    if( qbuffer_len(buf) > 2 ) {
      cur = start;
      search = end;
      qbuffer_iter_advance(buf, &cur, 1);
      qbuffer_iter_advance(buf, &search, -1);

      err = qbuffer_flatten(buf, cur, search, &flat2);
      assert(!err);

      j = 0;
      for( i = 0; i < num; i++ ) {
        int64_t sk = skip;
        int64_t ln = qb[i]->len - skip - trunc;
        if( i == 0 ) {
          // take one byte off the front of the first one..
          sk++;
          ln--;
        }
        if( i == num - 1 ) {
          // take one byte off the end of the last one.
          ln--;
        }
        if( ln > 0 ) {
          assert( 0 == memcmp((char*)flat2->data + j, (char*)qb[i]->data + sk, ln) );
          j += ln;
        }
      }
      assert( qbuffer_len(buf) - 2 == j );

      qbytes_release(flat2);
    }

    // now try advancing forward through the buffer, checking every byte.
    cur = start;
    j = 0;
    while( ! qbuffer_iter_equals(cur, end) ) {
      qbytes_t* tmp;
      int64_t skip, len;

      qbuffer_iter_get(cur, end, &tmp, &skip, &len);
      assert( 0 == memcmp((char*)tmp->data + skip, (char*)flat->data + j, 1) );

      // check that binary search would lead us here too.
      search = qbuffer_iter_at(buf, cur.offset);
      assert( qbuffer_iter_equals( search, cur ) );
      assert( qbuffer_iter_same_part( search, cur ) );

      qbuffer_iter_advance(buf, &cur, 1);
      j++;
    }

    // now try advancing backward through the buffer, checking every byte.
    if( ! qbuffer_iter_equals(start, end) ) {
      cur = end;
      j = flat->len - 1;
      do {
        qbytes_t* tmp;
        int64_t skip, len;

        qbuffer_iter_advance(buf, &cur, -1);

        qbuffer_iter_get(cur, end, &tmp, &skip, &len);
        assert( 0 == memcmp((char*)tmp->data + skip, (char*)flat->data + j, 1) );

        j--;
      } while( ! qbuffer_iter_equals(cur, start) );
    }

    // overwrite it with zeros.
    err = qbuffer_memset(buf, start, end, 0);
    assert(!err);

    err = qbuffer_flatten(buf, start, end, &zeros);
    assert(!err);

    for( off = 0; off < zeros->len; off++ ) {
      unsigned char byte = * (unsigned char*) ((char*)zeros->data + off);
      assert(byte == 0);
    }

    qbytes_release(zeros);

    // now copy the data back in.
    err = qbuffer_copyin(buf, start, end, flat->data, flat->len);
    assert(!err);

    err = qbuffer_flatten(buf, start, end, &flat2);
    assert(!err);

    assert( flat->len == flat2->len );
    assert( 0 == memcmp(flat->data, flat2->data, flat->len) );


    // clear flat
    memset(flat->data, 0, flat->len);

    // now copy the data back out.
    err = qbuffer_copyout(buf, start, end, flat->data, flat->len);
    assert(!err);

    assert( flat->len == flat2->len );
    assert( 0 == memcmp(flat->data, flat2->data, flat->len) );

    qbytes_release(flat2);
    qbytes_release(flat);
  }
  

}

void test_qbuffer_with(qbytes_t** qb, int num, int skip, int trunc)
{
  qbuffer_t buf;
  qioerr err;
  int i;
  int j;

  err = qbuffer_init(&buf);
  assert(!err);

  // try appending them all.
  for( i = 0; i < num; i++ ) {
    err = qbuffer_append(&buf, qb[i], skip, qb[i]->len - skip - trunc);
    assert(!err);
  }

  test_qbuffer_iterators(&buf, qb, num, skip, trunc);

  qbuffer_destroy(&buf);



  err = qbuffer_init(&buf);
  assert(!err);

  // try prepending them all.
  for( i = num - 1; i >= 0; i-- ) {
    err = qbuffer_prepend(&buf, qb[i], skip, qb[i]->len - skip - trunc);
    assert(!err);
  }

  test_qbuffer_iterators(&buf, qb, num, skip, trunc);

  qbuffer_destroy(&buf);


  err = qbuffer_init(&buf);
  assert(!err);

  // try appending/prepending alternating
  // i is prepending index, j is appending index
  for( i = num / 2 - 1, j = i+1; i >= 0 || j < num; i--, j++ ) {
    if( i >= 0 ) {
      err = qbuffer_prepend(&buf, qb[i], skip, qb[i]->len - skip - trunc);
      assert(!err);
    }
    if( j < num ) {
      err = qbuffer_append(&buf, qb[j], skip, qb[j]->len - skip - trunc);
      assert(!err);
    }
  }

  test_qbuffer_iterators(&buf, qb, num, skip, trunc);

  qbuffer_destroy(&buf);
}

void fill_test_data(qbytes_t* b, int testnum)
{
  int64_t i;
  unsigned char* data = b->data;

  for( i = 0; i < b->len; i++ ) {
    data[i] = 50 * testnum  + (i % 50);
  }
}

void test_qbuffer(void)
{
  // create a qbuffer.
  qbuffer_t buf;
  qbytes_t* b[8];
  int i,j, skip, trunc;
  qioerr err;
  int maxskip = 3;
  int maxtrunc = 3;

  err = qbuffer_init(&buf);
  assert(!err);

  qbuffer_destroy(&buf);

  // create some bytes objects...
  i = 0;
  err = qbytes_create_calloc(&b[i], 10);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  err = qbytes_create_calloc(&b[i], 20);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  err = qbytes_create_calloc(&b[i], 30);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  err = qbytes_create_calloc(&b[i], 40);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  for( j = 0; j < i; j++ ) {
    for( skip = 0; skip < maxskip; skip++ ) {
      for( trunc = 0; trunc < maxtrunc; trunc++ ) {
        test_qbuffer_with(b, j, skip, trunc);
      }
    }
  }

  for( j = 0; j < i; j++ ) {
    qbytes_release(b[j]);
  }
}

void test_qbuffer_two(void)
{
  // We create two buffers and call 
  // copyin_buffer
  qbuffer_t bufa, bufb, bufab1, bufab2;
  qbytes_t* b[8];
  qbytes_t* a[8];
  qbytes_t* bufa_flat;
  qbytes_t* bufb_flat;
  qbytes_t* bufab1_flat;
  qbytes_t* bufab2_flat;
  qbytes_t* bufb2_flat;
  int i,j, x,y;
  qioerr err;

  // create some bytes objects...
  i = 0;
  err = qbytes_create_calloc(&b[i], 10);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  err = qbytes_create_calloc(&b[i], 20);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  err = qbytes_create_calloc(&b[i], 30);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  err = qbytes_create_calloc(&b[i], 40);
  assert(!err);
  fill_test_data(b[i], i);
  i++;

  // create some bytes objects...
  x = 0;
  err = qbytes_create_calloc(&a[x], 10);
  assert(!err);
  fill_test_data(a[x], x);
  x++;

  err = qbytes_create_calloc(&a[x], 7);
  assert(!err);
  fill_test_data(a[x], x);
  x++;

  err = qbytes_create_calloc(&a[x], 29);
  assert(!err);
  fill_test_data(a[x], x);
  x++;

  err = qbytes_create_calloc(&a[x], 100);
  assert(!err);
  fill_test_data(a[x], x);
  x++;


  err = qbuffer_init(&bufa);
  assert(!err);
  err = qbuffer_init(&bufb);
  assert(!err);
  err = qbuffer_init(&bufab1);
  assert(!err);
  err = qbuffer_init(&bufab2);
  assert(!err);

  for( j = 0; j < i; j++ ) {
    err = qbuffer_append(&bufb, b[j], 0, b[j]->len);
    assert(!err);
  }
  for( y = 0; y < x; y++ ) {
    err = qbuffer_append(&bufa, a[y], 0, a[y]->len);
    assert(!err);
  }

  err = qbuffer_flatten(&bufa, qbuffer_begin(&bufa), qbuffer_end(&bufa), &bufa_flat);
  assert(!err);

  err = qbuffer_flatten(&bufb, qbuffer_begin(&bufb), qbuffer_end(&bufb), &bufb_flat);
  assert(!err);

  // construct ab1 with append
  err = qbuffer_append(&bufab1, bufa_flat, 0, bufa_flat->len);
  assert(!err);
  err = qbuffer_append(&bufab1, bufb_flat, 0, bufb_flat->len);
  assert(!err);
  
  err = qbuffer_flatten(&bufab1, qbuffer_begin(&bufab1), qbuffer_end(&bufab1), &bufab1_flat);
  assert(!err);

  // construct ab2 with qbuffer_append_buffer
  err = qbuffer_append_buffer(&bufab2, &bufa, qbuffer_begin(&bufa), qbuffer_end(&bufa));
  assert(!err);
  err = qbuffer_append_buffer(&bufab2, &bufb, qbuffer_begin(&bufb), qbuffer_end(&bufb));
  assert(!err);

  err = qbuffer_flatten(&bufab2, qbuffer_begin(&bufab2), qbuffer_end(&bufab2), &bufab2_flat);
  assert(!err);

  assert( bufab1_flat->len == bufab2_flat->len);
  assert( 0 == memcmp(bufab1_flat->data, bufab2_flat->data, bufab1_flat->len) );

  {
    qbuffer_iter_t end = qbuffer_begin(&bufab1);
    qbuffer_iter_advance(&bufab1, &end, qbuffer_len(&bufb));

    // Now try overwriting some data in bufab1 with the data in bufb.
    err = qbuffer_copyin_buffer(&bufab1, qbuffer_begin(&bufab1), end,
                                &bufb, qbuffer_begin(&bufb), qbuffer_end(&bufb));
    assert(!err);

    // Now check that bufab1 starts with b.
    err = qbuffer_flatten(&bufab1, qbuffer_begin(&bufab1), end, &bufb2_flat);
    assert(!err);

    assert( bufb2_flat->len == bufb_flat->len);
    assert( 0 == memcmp(bufb_flat->data, bufb2_flat->data, bufb_flat->len) );
  }

  qbytes_release(bufa_flat);
  qbytes_release(bufb_flat);
  qbytes_release(bufab1_flat);
  qbytes_release(bufab2_flat);
  qbytes_release(bufb2_flat);

  qbuffer_destroy(&bufb);
  qbuffer_destroy(&bufa);
  qbuffer_destroy(&bufab1);
  qbuffer_destroy(&bufab2);

  for( j = 0; j < i; j++ ) {
    qbytes_release(b[j]);
  }
  for( y = 0; y < x; y++ ) {
    qbytes_release(a[y]);
  }
}

void test_qbuffer_edges(void)
{
  qbuffer_t buf;
  qbytes_t *b1;
  qbytes_t *b2;
  qbytes_t *b3;
  qioerr err;
  int len = 1;
  qbuffer_iter_t start, end, cur;
  int totallen = 3*len;

  // create a bytes object
  err = qbytes_create_calloc(&b1, len);
  assert(!err);
  fill_test_data(b1, 1);
  err = qbytes_create_calloc(&b2, len);
  assert(!err);
  fill_test_data(b2, 2);
  err = qbytes_create_calloc(&b3, len);
  assert(!err);
  fill_test_data(b3, 3);

  err = qbuffer_init(&buf);
  assert(!err);

  err = qbuffer_append(&buf, b1, 0, len);
  assert(!err);
  err = qbuffer_append(&buf, b2, 0, len);
  assert(!err);
  err = qbuffer_append(&buf, b3, 0, len);
  assert(!err);


  // Now check that we can advance from the end to the beginning
  // and from the beginning to the end.
  start = qbuffer_begin(&buf);
  end = qbuffer_end(&buf);

  cur = start;
  qbuffer_iter_advance(&buf, &cur, totallen);
  assert( qbuffer_iter_same_part(cur, end) );
  assert( qbuffer_iter_equals(cur, end) );
  cur = qbuffer_iter_at(&buf, totallen);
  assert( qbuffer_iter_same_part(cur, end) );
  assert( qbuffer_iter_equals(cur, end) );

  cur = end;
  qbuffer_iter_advance(&buf, &cur, -totallen);
  assert( qbuffer_iter_same_part(cur, start) );
  assert( qbuffer_iter_equals(cur, start) );
  cur = qbuffer_iter_at(&buf, -totallen);
  assert( qbuffer_iter_same_part(cur, start) );
  assert( qbuffer_iter_equals(cur, start) );

  cur = start;
  qbuffer_iter_advance(&buf, &cur, totallen+1);
  assert( qbuffer_iter_same_part(cur, end) );
  assert( qbuffer_iter_equals(cur, end) );
  cur = qbuffer_iter_at(&buf, totallen+1);
  assert( qbuffer_iter_same_part(cur, end) );
  assert( qbuffer_iter_equals(cur, end) );

  cur = end;
  qbuffer_iter_advance(&buf, &cur, -totallen-1);
  assert( qbuffer_iter_same_part(cur, start) );
  assert( qbuffer_iter_equals(cur, start) );
  cur = qbuffer_iter_at(&buf, -totallen-1);
  assert( qbuffer_iter_same_part(cur, start) );
  assert( qbuffer_iter_equals(cur, start) );

  qbuffer_destroy(&buf);
  qbytes_release(b1);
  qbytes_release(b2);
  qbytes_release(b3);
}


int main(int argc, char** argv)
{
  test_qbytes();

  test_qbuffer();

  test_qbuffer_two();

  test_qbuffer_edges();

  printf("qbuffer_test PASS\n");

  return 0;
}

