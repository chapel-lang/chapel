#ifndef _QIO_ERROR_H
#define _QIO_ERROR_H

#include "sys_basic.h"
#include <assert.h>

#define QIO_ERROR_DOUBLE_CHECK 0

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

// on linux, normal error codes are < 255
enum { EXTEND_ERROR_OFFSET = 1000 };
// on linux, these are -1 to -100.
enum { GAI_ERROR_OFFSET = (EXTEND_ERROR_OFFSET+2000) };
enum { MAX_ERROR_OFFSET = GAI_ERROR_OFFSET+1000 };

static inline int32_t qio_err_to_int(qioerr a) {
  intptr_t num = (intptr_t) a;
  if( num == 0 ) return 0;
  if( num & 1 ) {
    // byte-aligned so can't be an error record.
    return num >> 1; // trim off the 1 we added to distinguish
  }
  return ((const struct qio_err_s*) a)->code;
}
static inline const char* qio_err_msg(qioerr a) {
  intptr_t num = (intptr_t) a;
  if( num == 0 ) return 0;
  if( num & 1 ) {
    // byte-aligned so can't be an error record.
    return NULL;
  }
  return ((struct qio_err_s*) a)->const_msg;
}

static inline qioerr qio_int_to_err(int32_t a) {
  intptr_t num = a;
  if( num != 0 ) {
    // add a 1 bit on the right to mark it as non-pointer.
    num <<= 1;
    num += 1;
  }
  return (qioerr) num;
}
static inline int qio_err_eq(qioerr a, qioerr b) {
  // Returns true if they have the same code.
  if( a == NULL && b == NULL ) return 1;
  if( (a == NULL) != (b == NULL) ) return 0;
  return qio_err_to_int(a) == qio_err_to_int(b);
}
static inline int qio_err_iserr(qioerr a) {
  return a != NULL;
}
static inline qioerr qio_mkerror_errno(void) {
  return qio_int_to_err(errno);
}
#define QIO_GET_CONSTANT_ERROR(ptr,code,note) { \
  static const struct qio_err_s qio_macro_tmp_err__ = {code, note, __func__, __FILE__, __LINE__}; \
  ptr = &qio_macro_tmp_err__; \
  if( QIO_ERROR_DOUBLE_CHECK ) \
    assert( qio_err_to_int(ptr) == code ); \
}
#define QIO_RETURN_CONSTANT_ERROR(code,note) { \
  static const struct qio_err_s qio_macro_tmp_err__ = {code, note, __func__, __FILE__, __LINE__}; \
  if( QIO_ERROR_DOUBLE_CHECK ) \
    assert( qio_err_to_int(&qio_macro_tmp_err__) == code ); \
  return &qio_macro_tmp_err__; \
}

// EEOF 
#ifndef EEOF
#define EEOF (EXTEND_ERROR_OFFSET+0)
#endif

#ifndef ESHORT
#define ESHORT (EXTEND_ERROR_OFFSET+1)
#endif

#ifndef EFORMAT
#define EFORMAT (EXTEND_ERROR_OFFSET+2)
#endif

// Make sure we have an EILSEQ
#ifndef EILSEQ
#define EILSEQ (EXTEND_ERROR_OFFSET+3)
#endif

// Make sure we have EOVERFLOW
#ifndef EOVERFLOW
#define EOVERFLOW (EXTEND_ERROR_OFFSET+4)
#endif

#define EXTEND_ERROR_NUM 5


#define QIO_ENOMEM (qio_int_to_err(ENOMEM))
#define QIO_ESHORT (qio_int_to_err(ESHORT))
#define QIO_EEOF (qio_int_to_err(EEOF))


// This could be done optionally only under Chapel.
typedef qioerr syserr;
static inline int chpl_macro_int_EEOF(void) { return EEOF; }
static inline int chpl_macro_int_ESHORT(void) { return ESHORT; }
static inline int chpl_macro_int_EFORMAT(void) { return EFORMAT; }

#endif
