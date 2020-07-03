/*
 * Copyright (c) 2015,2017 Cray Inc. All rights reserved.
 *
 *  Created on: Apr 16, 2015
 *      Author: jswaro
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdint.h>
#include <pthread.h>
#include <ofi.h>
#include <rdma/fi_errno.h>

#define GNIX_BITMAP_BUCKET_BITS 6
#define GNIX_BITMAP_BUCKET_LENGTH (1ULL << GNIX_BITMAP_BUCKET_BITS)
#define GNIX_BUCKET_INDEX(index) ((index) >> GNIX_BITMAP_BUCKET_BITS)
#define GNIX_BIT_INDEX(index) ((index) % GNIX_BITMAP_BUCKET_LENGTH)
#define GNIX_BIT_VALUE(index) (1ULL << GNIX_BIT_INDEX(index))

#define __PARTIAL_BLOCKS(nbits) (((nbits) % GNIX_BITMAP_BUCKET_LENGTH) ? 1 : 0)
#define __FULL_BLOCKS(nbits) ((nbits) >> GNIX_BITMAP_BUCKET_BITS)
#define GNIX_BITMAP_BLOCKS(nbits) \
	(__FULL_BLOCKS(nbits) + __PARTIAL_BLOCKS(nbits))

typedef uint64_t gnix_bitmap_value_t;

#ifdef HAVE_ATOMICS
#include <stdatomic.h>

typedef atomic_uint_fast64_t gnix_bitmap_block_t;
#else
typedef struct atomic_uint64_t {
	fastlock_t lock;
	gnix_bitmap_value_t val;
} gnix_bitmap_block_t;
#endif

typedef enum gnix_bitmap_state {
	GNIX_BITMAP_STATE_UNINITIALIZED = 0,
	GNIX_BITMAP_STATE_READY,
	GNIX_BITMAP_STATE_FREE,
} gnix_bitmap_state_e;

/**
 * @brief gnix bitmap structure
 *
 * @var state    state of the bitmap
 * @var length   length of bitmap in bits
 * @var arr      bitmap array
 * @var internal_buffer_allocation   flag to denote use of an externally
 *                                   allocated buffer
 */
typedef struct gnix_bitmap {
	gnix_bitmap_state_e state;
	uint32_t length;
	gnix_bitmap_block_t *arr;
	int internal_buffer_allocation;
} gnix_bitmap_t;

/**
 * Tests to see if a bit has been set in the bit.
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   index   index of the bit in the map to test
 * @return  0 if the bit is not set, 1 if the bit is set
 */
int _gnix_test_bit(gnix_bitmap_t *bitmap, uint32_t index);

/**
 * Sets a bit in the bitmap
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   index   index of the bit in the map to set
 */
void _gnix_set_bit(gnix_bitmap_t *bitmap, uint32_t index);

/**
 * Clears a bit in the bitmap
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   index   index of the bit in the map to clear
 */
void _gnix_clear_bit(gnix_bitmap_t *bitmap, uint32_t index);

/**
 * Tests to see if a bit is set, then sets the bit in the bitmap
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   index   index of the bit in the map to test and set
 * @return  0 if the bit was not set, 1 if the bit was already set
 */
int _gnix_test_and_set_bit(gnix_bitmap_t *bitmap, uint32_t index);

/**
 * Tests to see if a bit is set, the clears the bit in the bitmap
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   index   index of the bit in the map to test and set
 * @return  0 if the bit was not set, 1 if the bit was already set
 */
int _gnix_test_and_clear_bit(gnix_bitmap_t *bitmap, uint32_t index);

/**
 * Takes a gnix_bitmap and allocates the internal structures and performs
 *   generic setup based on the number of bits requested
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   nbits   number of bits to request space for
 * @param   addr    if provided, external memory allocation used for internal
					array
 * @return  0       on success
 * @return  -FI_EINVAL if bitmap is already initialized, or 0 is given
 *          as nbits
 * @return  -FI_ENOMEM if there isn't sufficient memory available to
 *          create bitmap
 * @note    If addr parameter is provided, realloc_bitmap will not work
 */
int _gnix_alloc_bitmap(gnix_bitmap_t *bitmap, uint32_t nbits, void *addr);

/**
 * Takes a gnix_bitmap and reallocates the internal structures to the requested
 *   size given in bits
 *
 * @note    On return of a ENOMEM error code, the bitmap will not be
 *          resized and will still be a valid and operable bitmap.
 *          The ENOMEM error only serves to indication that resources
 *          are	limited.
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   nbits   number of bits to resize the bitmap to
 * @return  0       on success
 * @return  -FI_EINVAL if the bitmap hasn't been allocated yet or nbits == 0
 * @return  -FI_ENOMEM if there wasn't sufficient memory to expand the bitmap.
 */
int _gnix_realloc_bitmap(gnix_bitmap_t *bitmap, uint32_t nbits);

/**
 * Frees the internal structures of gnix_bitmap
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @return  0       on success
 * @return  -FI_EINVAL if the internal resources are uninitialized or already free
 */
int _gnix_free_bitmap(gnix_bitmap_t *bitmap);

/**
 * Sets every bit in the bitmap with (value != 0)
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @param   value   an integer value to be compared with 0 to set bits to
 */
void _gnix_fill_bitmap(gnix_bitmap_t *bitmap, uint64_t value);

/**
 * Finds the bit index of the first zero bit in the bitmap
 *
 * @param   bitmap	a gnix_bitmap pointer to the bitmap struct
 * @return  index	on success, returns an index s.t.
 *                    0 <= index < bitmap->length
 * @return  -FI_EAGAIN on failure to find a zero bit
 */
int _gnix_find_first_zero_bit(gnix_bitmap_t *bitmap);

/**
 * Finds the bit index of the first set bit in the bitmap
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @return  index   on success, returns a index s.t.
 *                    0 <= index < bitmap->length
 * @return  -FI_EAGAIN on failure to find a set bit
 */
int _gnix_find_first_set_bit(gnix_bitmap_t *bitmap);

/**
 * Tests to verify that the bitmap is full
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @return  0 if the bitmap has cleared bits, 1 if the bitmap is fully set
 */
int _gnix_bitmap_full(gnix_bitmap_t *bitmap);

/**
 * Tests to verify that the bitmap is empty
 *
 * @param   bitmap  a gnix_bitmap pointer to the bitmap struct
 * @return  0 if the bitmap has set bits, 1 if the bitmap is fully cleared
 */
int _gnix_bitmap_empty(gnix_bitmap_t *bitmap);

/**
 * Helper function for determining the size of array needed to support
 * 'x' number of bits for an externally provided buffer address 
 * @param   nbits  number of bits requested for the bitmap
 */
__attribute__((unused))
static inline uint32_t _gnix_bitmap_get_buffer_size(int nbits)
{
	return GNIX_BITMAP_BLOCKS(nbits) * sizeof(gnix_bitmap_block_t);
}

#endif /* BITMAP_H_ */
