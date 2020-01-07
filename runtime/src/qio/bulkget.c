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

#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-comm-compiler-macros.h"
#include "chpl-linefile-support.h"

#include "bulkget.h"

// The initial ref count in the return qbytes buffer is 1.
// The caller is responsible for calling qbytes_release on it when done.
qbytes_t* bulk_get_bytes(int64_t src_locale, qbytes_t* src_addr)
{
  qbytes_t tmp;
  qbytes_t* ret;
  int64_t src_len;
  void* src_data;
  qioerr err;

  // Zero-initialize tmp.
  memset(&tmp, 0, sizeof(qbytes_t));

  // First, get the length and local pointer to the bytes.
  chpl_gen_comm_get(&tmp, src_locale, src_addr, sizeof(qbytes_t),
                    CHPL_COMM_UNKNOWN_ID, -1, CHPL_FILE_IDX_INTERNAL);
  src_len = tmp.len;
  src_data = tmp.data;

  // The initial ref count is 1.
  err = qbytes_create_calloc(&ret, src_len);
  if( err ) return NULL;

  // TODO -- note -- technically, this should be gasnet_get_bulk,
  // since we don't want to require src/dst to have a particular alignment.

  // Next, get the data itself.
  if( src_data ) {
    chpl_gen_comm_get(ret->data, src_locale, src_data,
                      sizeof(uint8_t) * src_len,
                      CHPL_COMM_UNKNOWN_ID, -1, CHPL_FILE_IDX_INTERNAL);
  }

  // Great! All done.
  return ret; 
}

qioerr bulk_put_buffer(int64_t dst_locale, void* dst_addr, int64_t dst_len,
                      qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end)
{
  int64_t num_bytes = qbuffer_iter_num_bytes(start, end);
  ssize_t num_parts = qbuffer_iter_num_parts(start, end);
  struct iovec* iov = NULL;
  size_t iovcnt;
  size_t i,j;
  MAYBE_STACK_SPACE(struct iovec, iov_onstack);
  qioerr err;
 
  if( num_bytes < 0 || num_parts < 0 || start.offset < buf->offset_start || end.offset > buf->offset_end )  QIO_RETURN_CONSTANT_ERROR(EINVAL, "range outside of buffer");

  MAYBE_STACK_ALLOC(struct iovec, num_parts, iov, iov_onstack);
  if( ! iov ) return QIO_ENOMEM;

  err = qbuffer_to_iov(buf, start, end, num_parts, iov, NULL, &iovcnt);
  if( err ) goto error;

  j = 0;
  for( i = 0; i < iovcnt; i++ ) {
    if( j + iov[i].iov_len > dst_len ) goto error_nospace;
    //chpl_memcpy(PTR_ADDBYTES(ptr, j), iov[i].iov_base, iov[i].iov_len);

    // TODO -- note -- technically, this should be gasnet_put_bulk,
    // since we don't want to require src/dst to have a particular alignment.
    chpl_gen_comm_put( iov[i].iov_base,
                       dst_locale,
                       PTR_ADDBYTES(dst_addr, j),
                       sizeof(uint8_t)*iov[i].iov_len,
                       CHPL_COMM_UNKNOWN_ID, -1, CHPL_FILE_IDX_INTERNAL);

    j += iov[i].iov_len;
  }

  MAYBE_STACK_FREE(iov, iov_onstack);
  return 0;

error_nospace:
  QIO_RETURN_CONSTANT_ERROR(EMSGSIZE, "no space in buffer");
error:
  MAYBE_STACK_FREE(iov, iov_onstack);
  return err;
}

