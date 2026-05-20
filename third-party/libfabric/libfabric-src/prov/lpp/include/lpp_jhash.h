/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
 * Copyright (C) 2006. Bob Jenkins (bob_jenkins@burtleburtle.net)
 *
 * Adapted from lookup3.c, by Bob Jenkins, May 2006, Public Domain.
 * https://burtleburtle.net/bob/c/lookup3.c
 *
 * Function name changed to match Linux kernel.
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

#ifndef _LPP_JHASH_H_
#define _LPP_JHASH_H_
#include "lpp.h"

#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define mix(a, b, c)				\
{						\
	a -= c;  a ^= rot(c, 4);  c += b;	\
	b -= a;  b ^= rot(a, 6);  a += c;	\
	c -= b;  c ^= rot(b, 8);  b += a;	\
	a -= c;  a ^= rot(c, 16); c += b;	\
	b -= a;  b ^= rot(a, 19); a += c;	\
	c -= b;  c ^= rot(b, 4);  b += a;	\
}

#define mix_final(a, b, c)			\
{						\
	c ^= b; c -= rot(b, 14);		\
	a ^= c; a -= rot(c, 11);		\
	b ^= a; b -= rot(a, 25);		\
	c ^= b; c -= rot(b, 16);		\
	a ^= c; a -= rot(c, 4);			\
	b ^= a; b -= rot(a, 14);		\
	c ^= b; c -= rot(b, 24);		\
}

#define JHASH_INITVAL 0xdeadbeef

static inline uint32_t jhash(const void *key, uint32_t length, uint32_t initval)
{
	uint32_t a, b, c;
	const uint8_t *k = key;

	/* Set up the internal state */
	a = b = c = JHASH_INITVAL + length + initval;

	/* All but the last block: affect some 32 bits of (a,b,c) */
	while (length > 12) {
		a += *((uint32_t *)k);
		b += *((uint32_t *)(k + 4));
		c += *((uint32_t *)(k + 8));
		mix(a, b, c);
		length -= 12;
		k += 12;
	}
	/* Last block: affect all 32 bits of (c) */
	switch (length) {
	case 12: c += (uint32_t)k[11]<<24;
		 __attribute__ ((fallthrough));
	case 11: c += (uint32_t)k[10]<<16;
		 __attribute__ ((fallthrough));
	case 10: c += (uint32_t)k[9]<<8;
		 __attribute__ ((fallthrough));
	case 9:  c += k[8];
		 __attribute__ ((fallthrough));
	case 8:  b += (uint32_t)k[7]<<24;
		 __attribute__ ((fallthrough));
	case 7:  b += (uint32_t)k[6]<<16;
		 __attribute__ ((fallthrough));
	case 6:  b += (uint32_t)k[5]<<8;
		 __attribute__ ((fallthrough));
	case 5:  b += k[4];
		 __attribute__ ((fallthrough));
	case 4:  a += (uint32_t)k[3]<<24;
		 __attribute__ ((fallthrough));
	case 3:  a += (uint32_t)k[2]<<16;
		 __attribute__ ((fallthrough));
	case 2:  a += (uint32_t)k[1]<<8;
		 __attribute__ ((fallthrough));
	case 1:  a += k[0];
		 mix_final(a, b, c);
		 break;
	case 0:
		break;
	}
	return c;
}
#endif // _LPP_JHASH_H_
