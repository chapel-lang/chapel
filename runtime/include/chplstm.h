#ifndef _chplstm_h_
#define _chplstm_h_

#ifndef LAUNCHER

#include <assert.h>
#include "chpltypes.h"
#include "chplstmstats.h"
#include CHPL_STM_H

extern int32_t chpl_localeID, chpl_numLocales;   // see src/chplcomm.c
#define MYLOCALE chpl_localeID
#define NLOCALES chpl_numLocales

typedef chpl_stm_tx_t* chpl_stm_tx_p;
typedef chpl_stm_tx_env_t* chpl_stm_tx_env_p;

typedef void (*chpl_txfn_p)(chpl_stm_tx_p, void*);
extern chpl_txfn_p chpl_txftable[];

#define CHPL_STM_TX_ALLOC_PERMIT_ZERO(tx, size , description, ln, fn)	\
  ((size == 0) ? NULL : chpl_stm_tx_alloc(tx, size, description, ln, fn))

#define chpl_stm_tx_alloc(tx, size, description, ln, fn)	\
  chpl_stm_tx_malloc(tx, 1, size, description, ln, fn)

#define _TX_ARRAY_ALLOC(tx, x, type, size, ln, fn)			\
  (x)->_data = (size == 0) ? (void*)(0x0) : chpl_stm_tx_malloc(tx, size, sizeof(type), CHPL_RT_MD_STM_TX_ARRAY_ELEMENTS, ln, fn)

#define _TX_WIDE_ARRAY_ALLOC(tx, x, type, size, ln, fn)			\
  do {									\
    if (x.locale != chpl_localeID)					\
      chpl_error("tx array vector data is not local", ln, fn);		\
    _TX_ARRAY_ALLOC(tx, (x).addr, type, size, ln, fn);			\
  } while (0)

#define _TX_ARRAY_FREE(tx, x, lineno, fn)	\
  chpl_stm_tx_free(tx, (x)->_data, lineno, fn)

#define _TX_WIDE_ARRAY_FREE(tx, x, lineno, fn)				\
  do {									\
    if (x.locale != chpl_localeID)					\
      chpl_error("array vector data is not local", lineno, fn);		\
    _TX_ARRAY_FREE(tx, (x).addr, lineno, fn);				\
  } while (0)

#define CHPL_STM_TX_BEGIN(tx, txenvptr)					\
  tx = chpl_stm_tx_create();						\
  txenvptr = chpl_stm_tx_get_env(tx);					\
  if (txenvptr)								\
    chpl_stm_tx_set_env(*txenvptr);					\
  chpl_stm_tx_begin(tx)

#define CHPL_STM_TX_COMMIT(tx)			\
  chpl_stm_tx_commit(tx); chpl_stm_tx_destroy(tx)

#define CHPL_STM_COMM_WIDE_GET(tx, ldst, wide, type, ln, fn)		\
  do {									\
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_load(tx, &ldst, (wide).addr,				\
		       SPECIFY_SIZE(type), ln, fn);			\
    else 								\
      chpl_stm_tx_get(tx, &ldst, (wide).locale, (wide).addr,		\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while(0)

#define CHPL_STM_LOAD_REF(tx, dst, src, type, ln, fn)		\
  chpl_stm_tx_load(tx, &dst, src, SPECIFY_SIZE(type), ln, fn)

#define CHPL_STM_LOAD(tx, dst, src, type, ln, fn)		\
  chpl_stm_tx_load(tx, &dst, &src, SPECIFY_SIZE(type), ln, fn)

#define CHPL_STM_LOAD_FIELD_VALUE(tx, dst, src, type, ln, fn)	\
  CHPL_STM_LOAD(tx, dst, src, type, ln, fn);

#define CHPL_STM_LOAD_FIELD_VALUE_SVEC(tx, dst, src, type, ln, fn)	\
  CHPL_STM_LOAD(tx, dst, src, type, ln, fn);

#define CHPL_STM_LOAD_TUPLE_COMPONENT_VALUE(tx, dst, src, type, ln, fn)	\
  CHPL_STM_LOAD(tx, dst, src, type, ln, fn);

#define CHPL_STM_LOAD_TUPLE_COMPONENT_VALUE_SVEC(tx, dst, src, type, ln, fn) \
  CHPL_STM_LOAD(tx, dst, src, type, ln, fn);

#define CHPL_STM_COMM_WIDE_GET_LOCALEID(tx, ldst, wide, ln, fn)	\
  do {                                                                  \
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_load(tx, &ldst, &((wide).addr->locale),		\
		       SPECIFY_SIZE(int32_t), ln, fn);			\
    else                                                                \
      chpl_stm_tx_get(tx, &(ldst), (wide).locale, (wide).addr,		\
		      SPECIFY_SIZE(int32_t), ln, fn);			\
  } while (0)

#define CHPL_STM_LOAD_LOCALEID(tx, ldst, lwide, ln, fn)			\
  chpl_stm_tx_load(tx, &ldst, &(lwide).locale, SPECIFY_SIZE(int32_t), ln, fn);

#define CHPL_STM_COMM_WIDE_GET_FIELD_VALUE(tx, ldst, wide, stype, sfield, type, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_load(tx, &ldst, &((stype)((wide).addr))->sfield,	\
		       SPECIFY_SIZE(type), ln, fn);			\
    else								\
      chpl_stm_tx_get(tx, &ldst,					\
		      (wide).locale, &((stype)((wide).addr))->sfield,	\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while (0)

#define CHPL_STM_COMM_WIDE_GET_FIELD_VALUE_SVEC(tx, ldst, wide, stype, sfield, type, ln, fn) \
  CHPL_STM_COMM_WIDE_GET_FIELD_VALUE(tx, ldst, wide, stype, sfield, type, ln, fn)

#define CHPL_STM_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE(tx, ldst, wide, index, type, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      chpl_stm_tx_load(tx, &ldst, &(*(wide).addr)[index],		\
		       SPECIFY_SIZE(type), ln, fn);			\
    else                                                                \
      chpl_stm_tx_get(tx, &(ldst),					\
                    (wide).locale, &(*(wide).addr)[index],		\
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_STM_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE_SVEC(tx, ldst, wide, index, type, ln, fn) \
  CHPL_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE(tx, ldst, wide, index, type, ln, fn)

#define CHPL_STM_COMM_WIDE_CLASS_GET_TEST_CID(tx, ldst, wide, cid, stype, sfield, ln, fn) \
  do {									\
    chpl__class_id chpl_macro_tmp;					\
    CHPL_STM_COMM_WIDE_GET_FIELD_VALUE(tx, chpl_macro_tmp, wide, stype, \
				       sfield, chpl__class_id, ln, fn);	\
    ldst = chpl_macro_tmp == cid;					\
  } while(0)

#define CHPL_STM_CLASS_LOAD_TEST_CID(tx, dst, src, cid, ln, fn)		\
  do {									\
    chpl__class_id chpl_macro_tmp;					\
    chpl_stm_tx_load(tx, &chpl_macro_tmp, &src,				\
		  SPECIFY_SIZE(chpl__class_id), ln, fn);		\
    dst = chpl_macro_tmp == cid;					\
  } while(0)

#define CHPL_STM_COMM_WIDE_ARRAY_GET(tx, wide, cls, ind, stype, sfield, etype, ln, fn) \
  do {                                                                  \
    (wide).locale = (cls).locale;					\
    CHPL_STM_COMM_WIDE_GET_FIELD_VALUE(tx, (wide).addr, cls, stype,	\
				       sfield, etype, ln, fn);		\
    (wide).addr += ind;						\
  } while (0)

#define CHPL_STM_COMM_WIDE_ARRAY_GET_SVEC(tx, wide, cls, ind, stype, sfield, etype, ln, fn) \
  CHPL_STM_COMM_WIDE_ARRAY_GET(tx, wide, cls, ind, stype, sfield, etype, ln, fn)

#define CHPL_STM_ARRAY_LOAD(tx, dst, src, ind, type, ln, fn)	\
  do {									\
    chpl_stm_tx_load(tx, &dst, &((src)->_data),				\
		     SPECIFY_SIZE(type), ln, fn);			\
    dst += ind;								\
  } while(0)

#define CHPL_STM_COMM_WIDE_ARRAY_GET_VALUE(tx, ldst, wide_type, cls, ind, stype, sfield, etype, etype2, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;						\
    CHPL_STM_COMM_WIDE_ARRAY_GET(tx, chpl_macro_tmp, cls, ind, stype,	\
				 sfield, etype, ln, fn);		\
    CHPL_STM_COMM_WIDE_GET(tx, ldst, chpl_macro_tmp, etype2, ln, fn);	\
  } while (0)

#define CHPL_STM_ARRAY_LOAD_VALUE(tx, dst, src, ind, type, ln, fn)	\
  do {									\
    chpl_stm_tx_load(tx, &dst, &((src)->_data[ind]),			\
		     SPECIFY_SIZE(type), ln, fn);			\
  } while(0)

#define CHPL_STM_COMM_WIDE_PUT(tx, wide, lsrc, type, ln, fn)		\
  do {									\
    type chpl_macro_tmp = lsrc;						\
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_store(tx, &chpl_macro_tmp, (wide).addr,		\
			SPECIFY_SIZE(type), ln, fn);			\
    else 								\
      chpl_stm_tx_put(tx, &chpl_macro_tmp, (wide).locale, (wide).addr,	\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while(0)

#define CHPL_STM_COMM_WIDE_PUT_SVEC(tx, wide, lsrc, type, ln, fn)	\
  do {									\
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_store(tx, &lsrc, (wide).addr,				\
			SPECIFY_SIZE(type), ln, fn);			\
    else 								\
      chpl_stm_tx_put(tx, &lsrc, (wide).locale, (wide).addr,		\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while(0)

#define CHPL_STM_STORE_REF(tx, dst, src, type, ln, fn)			\
  do {									\
    type chpl_macro_tmp = src;						\
    chpl_stm_tx_store(tx, &chpl_macro_tmp, dst,				\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while (0)

#define CHPL_STM_STORE_REF_SVEC(tx, dst, src, type, ln, fn)		\
  chpl_stm_tx_store(tx, &src, dst, SPECIFY_SIZE(type), ln, fn);

#define CHPL_STM_STORE(tx, dst, src, type, ln, fn)			\
  do {									\
    type chpl_macro_tmp = src;						\
    chpl_stm_tx_store(tx, &chpl_macro_tmp, &dst,			\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while (0)

#define CHPL_STM_STORE_SVEC(tx, dst, src, type, ln, fn)			\
  chpl_stm_tx_store(tx, &src, &dst, SPECIFY_SIZE(type), ln, fn);

#define CHPL_STM_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE(tx, wide, lsrc, stype, index, type, ln, fn) \
  do {                                                                  \
    type chpl_macro_tmp = lsrc;						\
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_store(tx, &chpl_macro_tmp, &(*(wide).addr)[index],	\
			SPECIFY_SIZE(type), ln, fn);			\
    else                                                                \
      chpl_stm_tx_put(tx, &chpl_macro_tmp, (wide).locale,		\
		      &(*(wide).addr)[index],				\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while (0)

#define CHPL_STM_COMM_WIDE_SET_FIELD_VALUE(tx, wide, lsrc, stype, sfield, type, ln, fn) \
  do {									\
    type chpl_macro_tmp = lsrc;						\
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_store(tx, &chpl_macro_tmp,				\
			&((stype)((wide).addr))->sfield,		\
			SPECIFY_SIZE(type), ln, fn);			\
    else								\
      chpl_stm_tx_put(tx, &chpl_macro_tmp, (wide).locale,		\
		      &((stype)((wide).addr))->sfield,			\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while (0)

#define CHPL_STM_COMM_WIDE_SET_FIELD_VALUE_SVEC(tx, wide, lsrc, stype, sfield, type, ln, fn) \
  do {									\
    if (chpl_localeID == (wide).locale)					\
      chpl_stm_tx_store(tx, &lsrc, &((stype)((wide).addr))->sfield,	\
			SPECIFY_SIZE(type), ln, fn);			\
    else								\
      chpl_stm_tx_put(tx, &lsrc, (wide).locale,				\
		      &((stype)((wide).addr))->sfield,			\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while (0)

#define CHPL_STM_COMM_WIDE_ARRAY_SET_VALUE(tx, wide_type, cls, ind, stype, sfield, etype, val, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp1;						\
    CHPL_STM_COMM_WIDE_ARRAY_GET(tx, chpl_macro_tmp1, cls, ind, stype,	\
				 sfield, etype, ln, fn);		\
    CHPL_STM_COMM_WIDE_PUT(tx, chpl_macro_tmp1, val, etype, ln, fn);	\
  } while (0)

#define CHPL_STM_ARRAY_STORE_VALUE(tx, dst, ind, src, type, ln, fn)	\
  do {									\
    type chpl_macro_tmp = src;						\
    chpl_stm_tx_store(tx, &chpl_macro_tmp, &((dst)->_data[ind]),	\
		      SPECIFY_SIZE(type), ln, fn);			\
  } while(0)

//
// initializes the STM layer, currently supported options -- gtm, none.
// invoked ONCE by $CHPL_ROOT/runtime/src/main.c
//
void chpl_stm_init(void);

//
// cleans up everything on a clean exit.
// invoked ONCE by $CHPL_ROOT/runtime/src/chplexit.c 
//
void chpl_stm_exit(void);

//
// returns a transaction descriptor
//
chpl_stm_tx_p chpl_stm_tx_create(void);

//
// reclaims the storage of a transaction descriptor
//
void chpl_stm_tx_destroy(chpl_stm_tx_p tx);

//
// global transaction begin
// only flat nesting currently supported
// no remote communication 
//
void chpl_stm_tx_begin(chpl_stm_tx_p tx);

//
// global transaction commit 
// 
void chpl_stm_tx_commit(chpl_stm_tx_p tx);

//
// global transaction abort 
// (related to chpl_stm_comm_tx_begin, chpl_stm_comm_tx_commit)
//
void chpl_stm_tx_abort(chpl_stm_tx_p tx);

//
// returns thread's execution state at the time chpl_stm_tx_begin(_local) 
// was called if called inside a nested transaction, returns NULL 
// (flat nesting)
// 
chpl_stm_tx_env_p chpl_stm_tx_get_env(chpl_stm_tx_p tx);

// 
// transactional load 'size' bytes of data from 'srcaddr' to addr at 'dstaddr'
//
void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string fn);

// 
// transactional store 'size' bytes of data at 'dstaddr' from data at 
// 'srcaddr'  
//
void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string fn);

// 
// transactional load 'size' bytes of remote data at 'srcaddr' 
// on 'srclocale' to local addr at 'dstaddr'
//
void chpl_stm_tx_get(chpl_stm_tx_p tx, void* dstaddr, int32_t srclocale, void* srcaddr, size_t size, int ln, chpl_string fn);

// 
// transactional store 'size' bytes of local data at 'srcaddr' to 
// remote data at 'dstaddr' on 'dstlocale' 
//
void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, int32_t dstlocale, void* dstaddr, size_t size, int ln, chpl_string fn);

//
// transactional fork operation, equivalent to chpl_comm_fork
//
void chpl_stm_tx_fork(chpl_stm_tx_p tx, int dstlocale, chpl_fn_int_t fid, void *arg, size_t argsize);

//
// transactional malloc
//
void* chpl_stm_tx_malloc(chpl_stm_tx_p tx, size_t number, size_t size, chpl_memDescInt_t description, int32_t ln, chpl_string fn);

void chpl_stm_tx_free(chpl_stm_tx_p tx, void* ptr, int32_t ln, chpl_string fn);

void chpl_startStmStats(void);
void chpl_stopStmStats(void);

#else // LAUNCHER

#define chpl_stm_exit()

#endif // LAUNCHER

#endif
