#include "qbuffer.h"
#include <assert.h>

void test_qbytes(void)
{
  qbytes_t* b;
  int err;

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
    int err;

    err = qbuffer_flatten(buf, start, end, &flat);
    assert(!err);

    // check that flat has the right data in it.
    j = 0;
    for( i = 0; i < num; i++ ) {
      assert( 0 == memcmp(flat->data + j, qb[i]->data + skip, qb[i]->len - skip - trunc) );
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
          assert( 0 == memcmp(flat2->data + j, qb[i]->data + sk, ln) );
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
      assert( 0 == memcmp(tmp->data + skip, flat->data + j, 1) );

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
        assert( 0 == memcmp(tmp->data + skip, flat->data + j, 1) );

        j--;
      } while( ! qbuffer_iter_equals(cur, start) );
    }

    // overwrite it with zeros.
    err = qbuffer_memset(buf, start, end, 0);
    assert(!err);

    err = qbuffer_flatten(buf, start, end, &zeros);
    assert(!err);

    for( off = 0; off < zeros->len; off++ ) {
      unsigned char byte = * (unsigned char*) (zeros->data + off);
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

    qbytes_release(flat);
    qbytes_release(flat2);
  }
  

}

void test_qbuffer_with(qbytes_t** qb, int num, int skip, int trunc)
{
  qbuffer_t buf;
  int err;
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
  int err;
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

int main(int argc, char** argv)
{
  test_qbytes();

  test_qbuffer();

  return 0;
}

