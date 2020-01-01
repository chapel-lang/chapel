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

qioerr bulk_put_buffer(int64_t dst_locale, void* dst_addr, int64_t dst_len,
                      qbuffer_t* buf, qbuffer_iter_t start, qbuffer_iter_t end);


#endif

