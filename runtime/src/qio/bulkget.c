#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-comm-compiler-macros.h"

#include "bulkget.h"

// The initial ref count in the return qbytes buffer is 1.
// The caller is responsible for calling qbytes_release on it when done.
qbytes_t* bulk_get_bytes(int64_t src_locale, qbytes_t* src_addr)
{
  qbytes_t* ret;
  int64_t src_len;
  err_t err;

  // First, get the length of the bytes.
  chpl_gen_comm_get( &src_len, src_locale, & src_addr->len, sizeof(int64_t), CHPL_TYPE_int64_t, 1, -1, "<internal>");
//  chpl_comm_get

  // The initial ref count is 1.
  err = qbytes_create_calloc(&ret, src_len);
  if( err ) return NULL;


  // TODO -- note -- technically, this should be gasnet_get_bulk,
  // since we don't want to require src/dst to have a particular alignment.

  // Next, get the data itself.
  chpl_gen_comm_get( ret->data, src_locale, & src_addr->data, sizeof(uint8_t), CHPL_TYPE_uint8_t, src_len, -1, "<internal>");

  // Great! All done.
  return ret; 
}

err_t bulk_put_buffer(int64_t dst_locale, void* dst_addr, int64_t dst_len,
                      qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end)
{
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i,j;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  err_t err;
 
  if( num_bytes < 0 || num_parts < 0 || start.offset < buf->offset_start || end.offset > buf->offset_end ) return EINVAL;

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) return ENOMEM;

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  j = 0;
  for( i = 0; i < iovcnt; i++ ) {
    if( j + iov[i].iov_len > dst_len ) goto error_nospace;
    //memcpy(PTR_ADDBYTES(ptr, j), iov[i].iov_base, iov[i].iov_len);

    // TODO -- note -- technically, this should be gasnet_put_bulk,
    // since we don't want to require src/dst to have a particular alignment.
    chpl_gen_comm_put( iov[i].iov_base,
                       dst_locale,
                       PTR_ADDBYTES(dst_addr, j),
                       sizeof(uint8_t), CHPL_TYPE_uint8_t,
                       iov[i].iov_len,
                       -1, "<internal>" );

    j += iov[i].iov_len;
  }

  MAYBE_STACK_FREE(iov, iov_onstack);
  return 0;

error_nospace:
  err = EMSGSIZE;
error:
  MAYBE_STACK_FREE(iov, iov_onstack);
  return err;
}

