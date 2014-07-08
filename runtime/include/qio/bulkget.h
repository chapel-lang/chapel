#ifndef _BULKGET_H_
#define _BULKGET_H_

#include "sys_basic.h"

#include <inttypes.h>
#include "qbuffer.h"
#include "qio_style.h"

// Clients of this routine must call qbytes_release() if the returned value
// is not retained.
// The initial value is 1, reflecting the assumption that it will be retained
// under normal program flow.
qbytes_t* bulk_get_bytes(int64_t src_locale, qbytes_t* src_addr);

err_t bulk_put_buffer(int64_t dst_locale, void* dst_addr, int64_t dst_len,
                      qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end);


#endif

