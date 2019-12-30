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

#include "sys_basic.h"

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "qio_error.h"

const struct qio_err_s* qio_error_get_base(void)
{
  static struct qio_err_s qio_error_base_global;
  return &qio_error_base_global;
}

const struct qio_err_s* qio_err_local_ptr(qioerr a)
{
  intptr_t num = (intptr_t) a;
  intptr_t base = (intptr_t) qio_error_get_base();
  return (qioerr) (num + base);
}

qioerr qio_err_local_ptr_to_err(const struct qio_err_s* a)
{
  intptr_t num = (intptr_t) a;
  intptr_t base = (intptr_t) qio_error_get_base();
  return (qioerr) (num - base);
}


const char* qio_err_msg(qioerr a) {
  intptr_t num = (intptr_t) a;
  if( num == 0 ) return 0;
  if( num & 1 ) {
    // byte-aligned so can't be an error record.
    return NULL;
  }
  return qio_err_local_ptr(a)->const_msg;
}

qioerr qio_int_to_err(int32_t a) {
  intptr_t num = a;
  if( num != 0 ) {
    // add a 1 bit on the right to mark it as non-pointer.
    num <<= 1;
    num += 1;
  }
  return (qioerr) num;
}

qioerr qio_mkerror_errno(void) {
  return qio_int_to_err(errno);
}

