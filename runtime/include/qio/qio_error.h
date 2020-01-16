/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _QIO_ERROR_H
#define _QIO_ERROR_H

#include "sys_basic.h"
#include <assert.h>

#define QIO_ERROR_DOUBLE_CHECK 0

#ifdef __cplusplus
extern "C" {
#endif

typedef int err_t;

struct qio_err_s {
  int code;
  const char* const_msg;
  // These are not currently output but could be output
  // in some kind of debug mode. For now, we save them
  // for debugging purposes.
  const char* const_fn;
  const char* const_file;
  int lineno;
};
// qioerr is meant to store either an error code directly
// or a pointer to a struct err_s. There are several ways
// it could do that - for example, we could have already
// allocated memory for the system error codes... but here
// we just use pointers with the lowest bit set, since that
// would not be allowed for a pointer to a struct err_s
// because it must be aligned.
//
// NULL means no error.
typedef const struct qio_err_s* qioerr;

/* A qioerr is sometimes a pointer to constant data allocated in the
   program image (like C strings). However, on some multilocale Chapel
   configurations, the program image is not loaded in the same location
   on different locales. We've observed this in particular on Mac OS X.
   As a result, the implementation here uses a difference between the
   indicated error structure's address and a "base" address in the program
   image (which is really just another constant structure - not actually
   the start address of the image). That allows the implementation to
   work program images loaded at different base addresses, but it will again
   cause problems if any of these error structures are allocated in dynamic
   libraries.
   
 */

// Get the address of a particular constant global data
const struct qio_err_s* qio_error_get_base(void);
// Translate a difference back into a local pointer
const struct qio_err_s* qio_err_local_ptr(qioerr a);
// Translate a local pointer into an offset from qio_error_get_base
qioerr qio_err_local_ptr_to_err(const struct qio_err_s* a);


// on linux, normal error codes are < 255
enum { EXTEND_ERROR_OFFSET = 1000 };
// on linux, these are -1 to -100.
enum { GAI_ERROR_OFFSET = (EXTEND_ERROR_OFFSET+2000) };
enum { MAX_ERROR_OFFSET = GAI_ERROR_OFFSET+1000 };

// Get the error code associated with a qioerr.
static inline int32_t qio_err_to_int(qioerr a) {
  intptr_t num = (intptr_t) a;
  if( num == 0 ) return 0;
  if( num & 1 ) {
    // byte-aligned so can't be an error record.
    return num >> 1; // trim off the 1 we added to distinguish
  }
  return qio_err_local_ptr(a)->code;
}

// Get the error message string associated with a qioerr.
const char* qio_err_msg(qioerr a);

// Get a qioerr referring to a numeric error code
qioerr qio_int_to_err(int32_t a);

// Check if the error codes from two qioerrs are equal.
static inline int qio_err_eq(qioerr a, qioerr b) {
  // Returns true if they have the same code.
  if( a == NULL && b == NULL ) return 1;
  if( (a == NULL) != (b == NULL) ) return 0;
  return qio_err_to_int(a) == qio_err_to_int(b);
}

// Return true if the passed qioerr represents an error
static inline int qio_err_iserr(qioerr a) {
  return a != NULL;
}

// Create a qioerr with the error code in errno
qioerr qio_mkerror_errno(void);

// This macro assigns to 'ptr' a qioerr with a particular
// code and a particular message. The code and the message
// must be constant.
#define QIO_GET_CONSTANT_ERROR(ptr,code,note) { \
  static const struct qio_err_s qio_macro_tmp_err__ = {code, note, __func__, __FILE__, __LINE__}; \
  ptr = qio_err_local_ptr_to_err(&qio_macro_tmp_err__); \
  if( QIO_ERROR_DOUBLE_CHECK ) \
    assert( qio_err_to_int(ptr) == code ); \
}

// This macro returns a qioerr with a particular code and
// a particular message. The code and the message
// must be constant.
#define QIO_RETURN_CONSTANT_ERROR(code,note) { \
  static const struct qio_err_s qio_macro_tmp_err__ = {code, note, __func__, __FILE__, __LINE__}; \
  if( QIO_ERROR_DOUBLE_CHECK ) \
    assert( qio_err_to_int(&qio_macro_tmp_err__) == code ); \
  return qio_err_local_ptr_to_err(&qio_macro_tmp_err__); \
}

// custom errors
// if you update these, also update the extended_errors array.

#ifndef EEOF
#define EEOF (EXTEND_ERROR_OFFSET+0)
#endif

#ifndef ESHORT
#define ESHORT (EXTEND_ERROR_OFFSET+1)
#endif

#ifndef EFORMAT
#define EFORMAT (EXTEND_ERROR_OFFSET+2)
#endif

// These are errors not available on every platform.
#ifndef EILSEQ
#define EILSEQ (EXTEND_ERROR_OFFSET+3)
#endif
#ifndef EOVERFLOW
#define EOVERFLOW (EXTEND_ERROR_OFFSET+4)
#endif
#ifndef ENODATA
#define ENODATA (EXTEND_ERROR_OFFSET+5)
#endif

#define EXTEND_ERROR_NUM 6


#define QIO_ENOMEM (qio_int_to_err(ENOMEM))
#define QIO_ESHORT (qio_int_to_err(ESHORT))
#define QIO_EEOF (qio_int_to_err(EEOF))


// This could be done optionally only under Chapel.
typedef qioerr syserr;
static inline int chpl_macro_int_EEOF(void) { return EEOF; }
static inline int chpl_macro_int_ESHORT(void) { return ESHORT; }
static inline int chpl_macro_int_EFORMAT(void) { return EFORMAT; }

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
