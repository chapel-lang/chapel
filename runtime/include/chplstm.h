//
// chplstm.h
// This is the highest-level STM interface file. 
//
// TODO:
//   add comments
//   ensure load/store/get/put interface works for: none, gtm-stm (multicore/dist)
//   load/store call the appropriate comm interface      
//   do we need separate versions for start/commit/abort: ie. local vs global ?
//
// LESSONS LEARNT: no point fixing interfaces to early.
//

#ifndef _chplstm_h_
#define _chplstm_h_

#ifndef LAUNCHER

#include <setjmp.h>
#include "chplcomm.h"
#include CHPL_STM_H

typedef chpl_stm_tx_t* chpl_stm_tx_p;
typedef jmp_buf* chpl_stm_tx_env_p;
typedef int32_t proc_t;

#define CHPL_STM_ALLOC_PERMIT_ZERO(s,d,l,f) ((s == 0) ? NULL : chpl_stm_tx_alloc(s,d,l,f))
#define chpl_stm_tx_alloc(tx, size, description, ln, filename)	\
  chpl_stm_tx_malloc(tx, 1, size, description, ln, filename)

#define _TX_ARRAY_ALLOC(tx, x, type, size, ln, filename)		\
  (x)->_data = (size == 0) ? (void*)(0x0) : chpl_stm_tx_malloc(tx, size, sizeof(type), CHPL_RT_MD_ARRAY_ELEMENTS, ln, filename)

#define _TX_WIDE_ARRAY_ALLOC(tx, x, type, size, ln, filename)	\
  do {									\
    if (x.locale != chpl_localeID)					\
      chpl_stm_error("tx array vector data is not local", ln, filename); \
    _TX_ARRAY_ALLOC(tx, (x).addr, type, size, ln, filename);	\
  } while (0)

//
// FIXME: we don't want to be creating/destroying a new transaction descriptor
// for each transaction.
//
#define CHPL_STM_TX_BEGIN(tx, env)  tx = chpl_stm_tx_create();	env = chpl_stm_tx_get_env(tx);	if (!env) goto tx123; setjmp(*env); tx123:; chpl_stm_tx_begin(tx);

/*  do {						\
    tx = chpl_stm_tx_create();				\
    if (tx != NULL) {					\
      env = chpl_stm_tx_get_env(tx);			\
      if (env != NULL) setjmp(*env);			\
      chpl_stm_tx_begin(tx);				\
    }							\
    } while(0) */

#define CHPL_STM_TX_COMMIT(tx)	chpl_stm_tx_commit(tx);

//chpl_stm_tx_destroy(tx);

#define CHPL_STM_LOAD(tx, dst, src, type, ln, filename)		\
  chpl_stm_tx_load(tx, &dst, &src, SPECIFY_SIZE(type), ln, filename)

#define CHPL_STM_LOAD_REF(tx, dst, src, type, ln, filename)		\
  chpl_stm_tx_load(tx, &dst, src, SPECIFY_SIZE(type), ln, filename)

#define CHPL_STM_LOAD_LOCALEID(tx, ldst, lwide, ln, filename)	\
  chpl_stm_tx_load(tx, &ldst, &(lwide).addr->locale, SPECIFY_SIZE(int32_t), ln, filename);

#define CHPL_STM_ELOAD(tx, dst, src)			\
  chpl_stm_tx_load(tx, &dst, &src, sizeof(dst), 0, 0)

//    type chpl_stm_macro_tmp = &((stype)(src))->_data;	

//
// equivalent to _ARRAY_GET in chplrt.h
// these macros are different in that the destination is an input
//
#define CHPL_STM_ARRAY_LOAD(tx, dst, src, ind, type, ln, filename)	\
  do {									\
    chpl_stm_tx_load(tx, &dst, &((src)->_data[ind]),			\
		     SPECIFY_SIZE(type), ln, filename);			\
    dst += ind;								\
  } while(0)

#define CHPL_STM_LOAD_TEST_CID(tx, dst, src, cid, ln, filename)	\
  do {									\
    chpl__class_id chpl_stm_tmp;					\
    chpl_stm_load(tx, &chpl_stm_tmp, &src,				\
		  SPECIFY_SIZE(chpl__class_id), ln, filename);	\
    dst = chpl_stm_tmp == cid;						\
  } while(0)

#define CHPL_STM_COMM_WIDE_GET(tx, ldst, rwide, type, ln, filename)	\
  do {									\
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_load(tx, &ldst, (rwide).addr,				\
		       SPECIFY_SIZE(type), ln, filename);		\
    else 								\
      chpl_stm_tx_get(tx, &ldst, (rwide).locale, (rwide).addr,		\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while(0)

#define CHPL_STM_COMM_WIDE_GET_LOCALE(tx, ldst, rwide, ln, filename) \
  do {                                                                  \
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_load(tx, &ldst, &(rwide).addr->locale,		\
		       SPECIFY_SIZE(int32_t), ln, filename);	\
    else                                                                \
      chpl_stm_tx_get(tx, &(ldst), &(rwide).locale, (rwide).addr,	\
		      SPECIFY_SIZE(int32_t), ln, filename);		\
  } while (0)

#define CHPL_STM_COMM_WIDE_GET_FIELD_VALUE(tx, ldst, rwide, stype, sfield, type, ln, filename) \
  do {                                                                  \
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_load(tx, &ldst, &((stype)((rwide).addr))->sfield,	\
		       SPECIFY_SIZE(type), ln, filename);		\
    else								\
      chpl_stm_tx_get(tx, &ldst, (rwide).locale,			\
		      &((stype)((rwide).addr))->sfield,			\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while (0)

#define CHPL_STM_COMM_WIDE_ARRAY_GET(tx, rwide, cls, ind, stype, sfield, etype, ln, filename) \
  do {                                                                  \
    (rwide).locale = (cls).locale;					\
    CHPL_STM_COMM_WIDE_GET_FIELD_VALUE(tx, (rwide).addr, cls, stype,	\
				       sfield, etype, ln, filename); \
    (rwide).addr += ind;						\
  } while (0)

#define CHPL_STM_COMM_WIDE_ARRAY_GET_SVEC(tx, rwide, cls, ind, stype, sfield, etype, ln, filename) \
  CHPL_STM_COMM_WIDE_ARRAY_GET(tx, rwide, cls, ind, stype, sfield, etype, ln, filename)

#define CHPL_STM_COMM_WIDE_CLASS_GET_TEST_CID(tx, ldst, rwide, cid, stype, sfield, ln, filename) \
  do {									\
    chpl__class_id chpl_stm_tmp;					\
    CHPL_STM_COMM_WIDE_GET_FIELD_VALUE(tx, chpl_stm_tmp, rwide, stype,	\
				       sfield, chpl__class_id,		\
				       ln, filename);		\
    ldst = chpl_stm_tmp == cid;						\
  } while(0)

#define CHPL_STM_STORE(tx, dst, src, type, ln, filename)		\
  do {									\
    type chpl_stm_tmp = src;						\
    chpl_stm_tx_store(tx, &chpl_stm_tmp, &dst,				\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while (0)

#define CHPL_STM_STORE_REF(tx, dst, src, type, ln, filename)	\
  do {									\
    type chpl_stm_tmp = src;						\
    chpl_stm_tx_store(tx, &chpl_stm_tmp, dst,				\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while (0)

#define CHPL_STM_COMM_WIDE_ARRAY_SET_VALUE(tx, rwide_type, cls, ind, stype, sfIeld, etype, val, ln, filename) \

#define CHPL_STM_ARRAY_STORE(tx, dst, ind, type, ln, filename)	\
  do {									\
    chpl_stm_tx_store(tx, &((src)->_data[ind]), &dst, 			\
		      SPECIFY_SIZE(type), ln, filename);		\
    dst += ind;								\
  } while(0)

// FIXME -- no guarantee rwide WILL be wide. Deprecated already ?
#define CHPL_STM_COMM_WIDE_EPUT(tx, rwide, lsrc)			\
  do {									\
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_store(tx, &lsrc, (rwide).addr, sizeof(lsrc), 0, 0);	\
    else 								\
      chpl_stm_tx_put(tx, &lsrc, (rwide).locale, (rwide).addr,		\
		      sizeof(lsrc), 0, 0);				\
  } while(0)

#define CHPL_STM_COMM_WIDE_PUT(tx, rwide, lsrc, type, ln, filename)	\
  do {									\
    type chpl_stm_tmp = lsrc;						\
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_store(tx, &chpl_stm_tmp, (rwide).addr,		\
			SPECIFY_SIZE(type), ln, filename);		\
    else 								\
      chpl_stm_tx_put(tx, &chpl_stm_tmp, (rwide).locale, (rwide).addr,	\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while(0)

#define CHPL_STM_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE(tx, rwide, lsrc, stype, index, type, ln, filename) \
  do {                                                                  \
    type chpl_stm_tmp = lsrc;						\
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_store(tx, &chpl_stm_tmp, &(*(rwide).addr)[index],	\
			SPECIFY_SIZE(type), ln, filename);		\
    else                                                                \
      chpl_stm_tx_put(tx, &chpl_stm_tmp, (rwide).locale,		\
		      &(*(rwide).addr)[index],				\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while (0)

#define CHPL_STM_COMM_WIDE_SET_FIELD_VALUE(tx, rwide, lsrc, stype, sfield, type, ln, filename) \
  do {									\
    type chpl_stm_tmp = lsrc;						\
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_store(tx, &chpl_stm_tmp, &((stype)((rwide).addr))->sfield, \
			SPECIFY_SIZE(type), ln, filename);		\
    else								\
      chpl_stm_tx_put(tx, &chpl_stm_tmp, (rwide).locale,		\
		      &((stype)((rwide).addr))->sfield,			\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while (0)

#define CHPL_STM_COMM_WIDE_SET_FIELD_VALUE_SVEC(tx, rwide, lsrc, stype, sfield, type, ln, filename) \
  do {									\
    if (chpl_localeID == (rwide).locale)				\
      chpl_stm_tx_store(tx, &lsrc, &((stype)((rwide).addr))->sfield,	\
			SPECIFY_SIZE(type), ln, filename);		\
    else								\
      chpl_stm_tx_put(tx, &lsrc, (rwide).locale,			\
		      &((stype)((rwide).addr))->sfield,			\
		      SPECIFY_SIZE(type), ln, filename);		\
  } while (0)

#define CHPL_STM_COMM_WIDE_ARRAY_SET_VALUE(tx, rwide_type, cls, ind, stype, sfIeld, etype, val, ln, filename) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_STM_COMM_WIDE_ARRAY_GET(tx, chpl_macro_tmp, cls, ind, stype,	\
				 sfield, etype, ln, fn);		\
    CHPL_STM_COMM_WIDE_PUT(tx, chpl_macro_tmp, val, etype, ln, fn);	\
  } while (0)

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
jmp_buf *chpl_stm_tx_get_env(chpl_stm_tx_p tx);

//
// similar to the global calls, except when we know all operations inside the
// transaction are local
// 
chpl_stm_tx_p chpl_stm_tx_create_local(void);
void chpl_stm_tx_destroy_local(chpl_stm_tx_p tx);
void chpl_stm_tx_begin_local(chpl_stm_tx_p tx);
void chpl_stm_tx_commit_local(chpl_stm_tx_p tx);
void chpl_stm_tx_abort_local(chpl_stm_tx_p tx);

// FIXME: Basically we need to deal with the size issue. There are two issues here. 1st deal with smaller words. 2nd deal with larger words

// 
// transactional load 'size' bytes of data from 'srcaddr' to addr at 'dstaddr'
// FIXME: size   
//
void chpl_stm_tx_load(chpl_stm_tx_p tx, void* dstaddr, void* srcaddr, size_t size, int ln, chpl_string filename);

// 
// transactional store 'size' bytes of data at 'dstaddr' from data at 
// 'srcaddr'  
// FIXME: size   
//
void chpl_stm_tx_store(chpl_stm_tx_p tx, void* srcaddr, void* dstaddr, size_t size, int ln, chpl_string filename);

// 
// transactional load 'size' bytes of remote data at 'srcaddr' 
// on 'locale' to local addr at 'dstaddr'
// notes:
//   internally check if the operation is local
// FIXME: size   
//
void chpl_stm_tx_get(chpl_stm_tx_p tx, void* dstaddr, proc_t locale, void* srcaddr, size_t size, int ln, chpl_string filename);

// 
// transactional store 'size' bytes of local data at 'srcaddr' to 
// remote data at 'dstaddr' on 'locale' 
// notes:
//   internally check if the operation is local
// FIXME: size
//
void chpl_stm_tx_put(chpl_stm_tx_p tx, void* srcaddr, proc_t locale, void* dstaddr, size_t size, int ln, chpl_string filename);

//
// transactional malloc
//
void* chpl_stm_tx_malloc(chpl_stm_tx_p tx, size_t number, size_t size, chpl_memDescInt_t description, int32_t ln, chpl_string filename);

void chpl_stm_tx_free(chpl_stm_tx_p tx, void* ptr, int32_t ln, chpl_string filename);

#else // LAUNCHER

#define chpl_stm_exit()

#endif // LAUNCHER

#endif
