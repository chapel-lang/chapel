/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _OFI_ATOM_H_
#define _OFI_ATOM_H_

#include "config.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ofi_lock.h>
#include <ofi_osd.h>

#ifdef HAVE_ATOMICS
#  include <stdatomic.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


#if ENABLE_DEBUG
#define ATOMIC_DEF_INIT int is_initialized
#define ATOMIC_IS_INITIALIZED(atomic) assert(atomic->is_initialized)
#define ATOMIC_INIT(atomic) atomic->is_initialized = 1
#else
#define ATOMIC_DEF_INIT
#define ATOMIC_IS_INITIALIZED(atomic)
#define ATOMIC_INIT(atomic)
#endif

#ifdef HAVE_ATOMICS
#ifdef HAVE_ATOMICS_LEAST_TYPES
typedef atomic_int_least32_t	ofi_atomic_int32_t;
typedef atomic_int_least64_t	ofi_atomic_int64_t;
#else
typedef atomic_int	ofi_atomic_int32_t;
typedef atomic_long	ofi_atomic_int64_t;
#endif

#define OFI_ATOMIC_DEFINE(radix)									\
	typedef struct {										\
		ofi_atomic_int##radix##_t val;								\
		ATOMIC_DEF_INIT;									\
	} ofi_atomic##radix##_t;									\
													\
	static inline											\
	int##radix##_t ofi_atomic_inc##radix(ofi_atomic##radix##_t *atomic)				\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return (int##radix##_t)atomic_fetch_add_explicit(&atomic->val, 1,			\
								 memory_order_acq_rel) + 1;		\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_dec##radix(ofi_atomic##radix##_t *atomic)				\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return (int##radix##_t)atomic_fetch_sub_explicit(&atomic->val, 1,			\
								 memory_order_acq_rel) - 1;		\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_set##radix(ofi_atomic##radix##_t *atomic, int##radix##_t value)	\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		atomic_store(&atomic->val, value);							\
		return (int##radix##_t)value;								\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_get##radix(ofi_atomic##radix##_t *atomic)				\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return (int##radix##_t)atomic_load(&atomic->val);					\
	}												\
	static inline											\
	void ofi_atomic_initialize##radix(ofi_atomic##radix##_t *atomic, int##radix##_t value)		\
	{												\
		atomic_init(&atomic->val, value);							\
		ATOMIC_INIT(atomic);									\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_add##radix(ofi_atomic##radix##_t *atomic, int##radix##_t val)		\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return (int##radix##_t)atomic_fetch_add_explicit(&atomic->val, val,			\
								 memory_order_acq_rel) + val;		\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_sub##radix(ofi_atomic##radix##_t *atomic, int##radix##_t val)		\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return (int##radix##_t)atomic_fetch_sub_explicit(&atomic->val, val,			\
								 memory_order_acq_rel) - val;		\
	}												\
	/**												\
	 *  Compare and swap, strong version								\
	 *												\
	 *  @return true if atomic matches expected and the change is done, false			\
	 *   otherwise.											\
	 */												\
	static inline											\
	bool ofi_atomic_cas_bool_strong##radix(ofi_atomic##radix##_t *atomic, 				\
						      int##radix##_t expected, 				\
						      int##radix##_t desired)				\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return atomic_compare_exchange_strong_explicit(&atomic->val, &expected, desired,	\
							       memory_order_acq_rel,			\
							       memory_order_relaxed);			\
	}												\
	/**												\
	 *  Compare and swap, weak version								\
	 *												\
	 *  @return true if atomic matches expected and the change is done, false			\
	 *   otherwise.											\
	 *   This is the weak version and may incorrectly report a failed match.			\
	 *   As a result it is most useful in loops that wait until the check succeeds.			\
	 */												\
	 static inline											\
	 bool ofi_atomic_cas_bool_weak##radix(ofi_atomic##radix##_t *atomic, 				\
					      int##radix##_t expected, 					\
					      int##radix##_t desired)					\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return atomic_compare_exchange_weak_explicit(&atomic->val, 				\
							     &expected, desired,			\
							     memory_order_acq_rel,			\
							     memory_order_relaxed);			\
	}												\
	static inline											\
	bool ofi_atomic_cas_bool##radix(ofi_atomic##radix##_t *atomic, 					\
					int##radix##_t expected, 					\
					int##radix##_t desired)						\
	{												\
		return ofi_atomic_cas_bool_strong##radix(atomic, expected, desired);			\
	}												\
	static inline											\
	bool ofi_atomic_compare_exchange_weak##radix(ofi_atomic##radix##_t *atomic, 			\
					int##radix##_t *expected,					\
					int##radix##_t desired)						\
	{												\
		return atomic_compare_exchange_weak(&atomic->val, expected, desired);			\
	}												\
	static inline											\
	void ofi_atomic_store_explicit##radix(ofi_atomic##radix##_t *atomic,				\
					      int##radix##_t value, int memmodel)			\
	{												\
		atomic_store_explicit(&atomic->val, value, memmodel);					\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_load_explicit##radix(ofi_atomic##radix##_t *atomic,			\
						       int memmodel)					\
	{												\
		return atomic_load_explicit(&atomic->val, memmodel);					\
	}

#elif defined HAVE_BUILTIN_ATOMICS
#  if ENABLE_DEBUG
#    define ATOMIC_T(radix)		\
	struct {			\
		int##radix##_t val;	\
		ATOMIC_DEF_INIT;	\
	}

#    define ofi_atomic_ptr(atomic) (&((atomic)->val))
#  else
#    define ATOMIC_T(radix) int##radix##_t
#    define ofi_atomic_ptr(atomic) (atomic)
#  endif

#define OFI_ATOMIC_DEFINE(radix)									\
	typedef ATOMIC_T(radix) ofi_atomic##radix##_t;							\
													\
	static inline											\
	int##radix##_t ofi_atomic_add##radix(ofi_atomic##radix##_t *atomic, int##radix##_t val)		\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return (int##radix##_t)ofi_atomic_add_and_fetch(radix, ofi_atomic_ptr(atomic), val);	\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_sub##radix(ofi_atomic##radix##_t *atomic, int##radix##_t val)		\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return (int##radix##_t)ofi_atomic_sub_and_fetch(radix, ofi_atomic_ptr(atomic), val);	\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_inc##radix(ofi_atomic##radix##_t *atomic)				\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return ofi_atomic_add##radix(atomic, 1);						\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_dec##radix(ofi_atomic##radix##_t *atomic)				\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return ofi_atomic_sub##radix(atomic, 1);						\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_set##radix(ofi_atomic##radix##_t *atomic, int##radix##_t value)	\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		*(ofi_atomic_ptr(atomic)) = value;							\
		return value;										\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_get##radix(ofi_atomic##radix##_t *atomic)				\
	{												\
		ATOMIC_IS_INITIALIZED(atomic);								\
		return *ofi_atomic_ptr(atomic);								\
	}												\
	static inline											\
	void ofi_atomic_initialize##radix(ofi_atomic##radix##_t *atomic, int##radix##_t value)		\
	{												\
		*(ofi_atomic_ptr(atomic)) = value;							\
		ATOMIC_INIT(atomic);									\
	}												\
	static inline											\
	bool ofi_atomic_cas_bool##radix(ofi_atomic##radix##_t *atomic, 					\
					int##radix##_t expected,					\
					int##radix##_t desired)						\
	{												\
		 ATOMIC_IS_INITIALIZED(atomic);								\
		 return ofi_atomic_cas_bool(radix, ofi_atomic_ptr(atomic), expected, desired);		\
	}												\
	static inline											\
	bool ofi_atomic_cas_bool_strong##radix(ofi_atomic##radix##_t *atomic, 				\
					       int##radix##_t expected,					\
					       int##radix##_t desired)					\
	{												\
		return ofi_atomic_cas_bool##radix(atomic, expected, desired);				\
	}												\
	static inline											\
	bool ofi_atomic_cas_bool_weak##radix(ofi_atomic##radix##_t *atomic, 				\
					     int##radix##_t expected,					\
					     int##radix##_t desired)					\
	{												\
		return ofi_atomic_cas_bool##radix(atomic, expected, desired);				\
	}												\
	static inline											\
	bool ofi_atomic_compare_exchange_weak##radix(ofi_atomic##radix##_t *atomic,			\
					int##radix##_t *expected,					\
					int##radix##_t desired)						\
	{												\
		return ofi_atomic_compare_exchange_weak(radix, ofi_atomic_ptr(atomic), expected,	\
						desired, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);		\
	}												\
	static inline											\
	void ofi_atomic_store_explicit##radix(ofi_atomic##radix##_t *atomic,				\
					      int##radix##_t value, int memmodel)			\
	{												\
		ofi_atomic_store_explicit(radix, ofi_atomic_ptr(atomic), value, memmodel);		\
	}												\
	static inline											\
	int##radix##_t ofi_atomic_load_explicit##radix(ofi_atomic##radix##_t *atomic,			\
						       int memmodel)					\
	{												\
		return ofi_atomic_load_explicit(radix, ofi_atomic_ptr(atomic), memmodel);		\
	}

#else /* HAVE_ATOMICS */

#define OFI_ATOMIC_DEFINE(radix)								\
	typedef	struct {									\
		ofi_spin_t lock;								\
		int##radix##_t val;								\
		ATOMIC_DEF_INIT;								\
	} ofi_atomic##radix##_t;								\
												\
	static inline										\
	int##radix##_t ofi_atomic_inc##radix(ofi_atomic##radix##_t *atomic)			\
	{											\
		int##radix##_t v = 0;								\
		ATOMIC_IS_INITIALIZED(atomic);							\
		ofi_spin_lock(&atomic->lock);						\
		v = ++(atomic->val);								\
		ofi_spin_unlock(&atomic->lock);						\
		return v;									\
	}											\
	static inline										\
	int##radix##_t ofi_atomic_dec##radix(ofi_atomic##radix##_t *atomic)			\
	{											\
		int##radix##_t v = 0;								\
		ATOMIC_IS_INITIALIZED(atomic);							\
		ofi_spin_lock(&atomic->lock);						\
		v = --(atomic->val);								\
		ofi_spin_unlock(&atomic->lock);						\
		return v;									\
	}											\
	static inline										\
	int##radix##_t ofi_atomic_set##radix(ofi_atomic##radix##_t *atomic,			\
					     int##radix##_t value)				\
	{											\
		ATOMIC_IS_INITIALIZED(atomic);							\
		ofi_spin_lock(&atomic->lock);						\
		atomic->val = value;								\
		ofi_spin_unlock(&atomic->lock);						\
		return value;									\
	}											\
	static inline int##radix##_t ofi_atomic_get##radix(ofi_atomic##radix##_t *atomic)	\
	{											\
		ATOMIC_IS_INITIALIZED(atomic);							\
		return atomic->val;								\
	}											\
	static inline										\
	void ofi_atomic_initialize##radix(ofi_atomic##radix##_t *atomic,			\
					  int##radix##_t value)					\
	{											\
		ofi_spin_init(&atomic->lock);							\
		atomic->val = value;								\
		ATOMIC_INIT(atomic);								\
	}											\
	static inline										\
	int##radix##_t ofi_atomic_add##radix(ofi_atomic##radix##_t *atomic,			\
					     int##radix##_t val)				\
	{											\
		int##radix##_t v;								\
		ATOMIC_IS_INITIALIZED(atomic);							\
		ofi_spin_lock(&atomic->lock);						\
		atomic->val += val;								\
		v = atomic->val;								\
		ofi_spin_unlock(&atomic->lock);						\
		return v;									\
	}											\
	static inline										\
	int##radix##_t ofi_atomic_sub##radix(ofi_atomic##radix##_t *atomic,			\
					     int##radix##_t val)				\
	{											\
		int##radix##_t v;								\
		ATOMIC_IS_INITIALIZED(atomic);							\
		ofi_spin_lock(&atomic->lock);						\
		atomic->val -= val;								\
		v = atomic->val;								\
		ofi_spin_unlock(&atomic->lock);						\
		return v;									\
	}											\
	static inline										\
	bool ofi_atomic_cas_bool##radix(ofi_atomic##radix##_t *atomic,				\
					int##radix##_t expected,				\
					int##radix##_t desired)					\
	{											\
		bool ret = false;								\
		ATOMIC_IS_INITIALIZED(atomic);							\
		ofi_spin_lock(&atomic->lock);						\
		if (atomic->val == expected) {							\
			atomic->val = desired;							\
			ret = true;								\
		}										\
		ofi_spin_unlock(&atomic->lock);						\
		return ret;									\
	}											\
	static inline										\
	bool ofi_atomic_cas_bool_strong##radix(ofi_atomic##radix##_t *atomic,			\
							 int##radix##_t expected,		\
							 int##radix##_t desired)		\
	{											\
		return ofi_atomic_cas_bool##radix(atomic, expected, desired);			\
	}											\
	static inline										\
	bool ofi_atomic_cas_bool_weak##radix(ofi_atomic##radix##_t *atomic,			\
							 int##radix##_t expected,		\
							 int##radix##_t desired)		\
	{											\
		return ofi_atomic_cas_bool##radix(atomic, expected, desired);			\
	}											\
	static inline										\
	bool ofi_atomic_compare_exchange_weak##radix(ofi_atomic##radix##_t *atomic,		\
					int##radix##_t *expected,				\
					int##radix##_t desired)					\
	{											\
		return ofi_atomic_cas_bool_weak##radix(atomic, *expected, desired);		\
	}											\
	static inline										\
	void ofi_atomic_store_explicit##radix(ofi_atomic##radix##_t *atomic,			\
					      int##radix##_t value, int memmodel)		\
	{											\
		(void) ofi_atomic_set##radix(atomic, value);						\
	}											\
	static inline										\
	int##radix##_t ofi_atomic_load_explicit##radix(ofi_atomic##radix##_t *atomic,		\
						       int memmodel)				\
	{											\
		return ofi_atomic_get##radix(atomic);						\
	}											\

#endif // HAVE_ATOMICS

OFI_ATOMIC_DEFINE(32)
OFI_ATOMIC_DEFINE(64)

#ifdef __cplusplus
}
#endif

#endif /* _OFI_ATOM_H_ */
