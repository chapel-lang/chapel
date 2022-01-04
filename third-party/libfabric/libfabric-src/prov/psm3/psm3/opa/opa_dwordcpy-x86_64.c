/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include <stdint.h>
#include <immintrin.h>
#include "opa_intf.h"
#include "psm_user.h"

#if defined(__x86_64__) && defined(HAVE_PSM3_DWORD_FAST)
#define hfi_dwordcpy hfi_dwordcpy_safe
#define hfi_qwordcpy hfi_qwordcpy_safe
#endif

void hfi_dwordcpy(volatile uint32_t *dest, const uint32_t *src, uint32_t ndwords)
{
	uint_fast32_t ndw = ndwords;
	const uint64_t *src64[4];
	volatile uint64_t *dst64[4];
	src64[0] = (const uint64_t *) src;
	dst64[0] = (volatile uint64_t *) dest;

	while (ndw >= 8) {
		*dst64[0] = *src64[0];
		src64[1] = src64[0] + 1;
		src64[2] = src64[0] + 2;
		src64[3] = src64[0] + 3;
		ndw -= 8;
		dst64[1] = dst64[0] + 1;
		dst64[2] = dst64[0] + 2;
		dst64[3] = dst64[0] + 3;
		*dst64[1] = *src64[1];
		*dst64[2] = *src64[2];
		*dst64[3] = *src64[3];
		src64[0] += 4;
		dst64[0] += 4;
	}
	if (ndw) {
		src = (const uint32_t *) src64[0];
		dest = (volatile uint32_t *) dst64[0];

		switch (ndw) {
		case 7: *dest++ = *src++;
		/* fall through */
		case 6: *dest++ = *src++;
		/* fall through */
		case 5: *dest++ = *src++;
		/* fall through */
		case 4: *dest++ = *src++;
		/* fall through */
		case 3: *dest++ = *src++;
		/* fall through */
		case 2:	*dest++ = *src++;
		/* fall through */
		case 1: *dest++ = *src++;
		}

	}
}

void hfi_qwordcpy(volatile uint64_t *dest, const uint64_t *src, uint32_t nqwords)
{
	uint_fast32_t nqw = nqwords;
	const uint64_t *src64[4];
	volatile uint64_t *dst64[4];
	src64[0] = src;
	dst64[0] = dest;

	while (nqw >= 8) {
		*dst64[0] = *src64[0];
		src64[1] = src64[0] + 1;
		src64[2] = src64[0] + 2;
		src64[3] = src64[0] + 3;
		dst64[1] = dst64[0] + 1;
		dst64[2] = dst64[0] + 2;
		dst64[3] = dst64[0] + 3;
		*dst64[1] = *src64[1];
		*dst64[2] = *src64[2];
		*dst64[3] = *src64[3];
		src64[0] += 4;
		dst64[0] += 4;

		*dst64[0] = *src64[0];
		src64[1] = src64[0] + 1;
		src64[2] = src64[0] + 2;
		src64[3] = src64[0] + 3;
		dst64[1] = dst64[0] + 1;
		dst64[2] = dst64[0] + 2;
		dst64[3] = dst64[0] + 3;
		*dst64[1] = *src64[1];
		*dst64[2] = *src64[2];
		*dst64[3] = *src64[3];
		src64[0] += 4;
		dst64[0] += 4;

		nqw -= 8;
	}
	if (nqw) {
		switch (nqw) {
		case 7: *(dst64[0])++ = *(src64[0])++;
		/* fall through */
		case 6: *(dst64[0])++ = *(src64[0])++;
		/* fall through */
		case 5: *(dst64[0])++ = *(src64[0])++;
		/* fall through */
		case 4: *(dst64[0])++ = *(src64[0])++;
		/* fall through */
		case 3: *(dst64[0])++ = *(src64[0])++;
		/* fall through */
		case 2: *(dst64[0])++ = *(src64[0])++;
		/* fall through */
		case 1: *(dst64[0])++ = *(src64[0])++;
		}
	}
}

#ifdef PSM_AVX512
void hfi_pio_blockcpy_512(volatile uint64_t *dest, const uint64_t *src, uint32_t nblock)
{
	volatile __m512i *dp = (volatile __m512i *) dest;
	const __m512i *sp = (const __m512i *) src;

	psmi_assert((dp != NULL) && (sp != NULL));
	psmi_assert((((uintptr_t) dp) & 0x3f) == 0x0);

	if ((((uintptr_t) sp) & 0x3f) == 0x0) {
		/* source and destination are both 64 byte aligned */
		do {
			__m512i tmp0 = _mm512_load_si512(sp);
			_mm512_store_si512((__m512i *)dp, tmp0);
		} while ((--nblock) && (++dp) && (++sp));
	} else {
		/* only destination is 64 byte aligned - use unaligned loads */
		do {
			__m512i tmp0 = _mm512_loadu_si512(sp);
			_mm512_store_si512((__m512i *)dp, tmp0);
		} while ((--nblock) && (++dp) && (++sp));
	}
}
#endif

void hfi_pio_blockcpy_256(volatile uint64_t *dest, const uint64_t *src, uint32_t nblock)
{
	volatile __m256i *dp = (volatile __m256i *) dest;
	const __m256i *sp = (const __m256i *) src;

	psmi_assert((dp != NULL) && (sp != NULL));
	psmi_assert((((uintptr_t) dp) & 0x3f) == 0x0);

	if ((((uintptr_t) sp) & 0x1f) == 0x0) {
		/* source and destination are both 32 byte aligned */
		do {
			__m256i tmp0 = _mm256_load_si256(sp);
			__m256i tmp1 = _mm256_load_si256(sp + 1);
			_mm256_store_si256((__m256i *)dp, tmp0);
			_mm256_store_si256((__m256i *)(dp + 1), tmp1);
		} while ((--nblock) && (dp = dp+2) && (sp = sp+2));
	} else {
		/* only destination is 32 byte aligned - use unaligned loads */
		do {
			__m256i tmp0 = _mm256_loadu_si256(sp);
			__m256i tmp1 = _mm256_loadu_si256(sp + 1);
			_mm256_store_si256((__m256i *)dp, tmp0);
			_mm256_store_si256((__m256i *)(dp + 1), tmp1);
		} while ((--nblock) && (dp = dp+2) && (sp = sp+2));
	}
}

void hfi_pio_blockcpy_128(volatile uint64_t *dest, const uint64_t *src, uint32_t nblock)
{
	volatile __m128i *dp = (volatile __m128i *) dest;
	const __m128i *sp = (const __m128i *) src;

	psmi_assert((dp != NULL) && (sp != NULL));
	psmi_assert((((uintptr_t) dp) & 0x3f) == 0x0);

	if ((((uintptr_t) sp) & 0xf) == 0x0) {
		/* source and destination are both 16 byte aligned */
		do {
			__m128i tmp0 = _mm_load_si128(sp);
			__m128i tmp1 = _mm_load_si128(sp + 1);
			__m128i tmp2 = _mm_load_si128(sp + 2);
			__m128i tmp3 = _mm_load_si128(sp + 3);
			_mm_store_si128((__m128i *)dp, tmp0);
			_mm_store_si128((__m128i *)(dp + 1), tmp1);
			_mm_store_si128((__m128i *)(dp + 2), tmp2);
			_mm_store_si128((__m128i *)(dp + 3), tmp3);
		} while ((--nblock) && (dp = dp+4) && (sp = sp+4));
	} else {
		/* only destination is 16 byte aligned - use unaligned loads */
		do {
			__m128i tmp0 = _mm_loadu_si128(sp);
			__m128i tmp1 = _mm_loadu_si128(sp + 1);
			__m128i tmp2 = _mm_loadu_si128(sp + 2);
			__m128i tmp3 = _mm_loadu_si128(sp + 3);
			_mm_store_si128((__m128i *)dp, tmp0);
			_mm_store_si128((__m128i *)(dp + 1), tmp1);
			_mm_store_si128((__m128i *)(dp + 2), tmp2);
			_mm_store_si128((__m128i *)(dp + 3), tmp3);
		} while ((--nblock) && (dp = dp+4) && (sp = sp+4));
	}
}

void hfi_pio_blockcpy_64(volatile uint64_t *dest, const uint64_t *src, uint32_t nblock)
{
	const uint64_t *src64[4];
	volatile uint64_t *dst64[4];
	src64[0] = src;
	dst64[0] = dest;

	psmi_assert((dst64[0] != NULL) && (src64[0] != NULL));
	psmi_assert((((uintptr_t) dest) & 0x3f) == 0x0);

	do {
		*dst64[0] = *src64[0];
		src64[1] = src64[0] + 1;
		src64[2] = src64[0] + 2;
		src64[3] = src64[0] + 3;
		dst64[1] = dst64[0] + 1;
		dst64[2] = dst64[0] + 2;
		dst64[3] = dst64[0] + 3;
		*dst64[1] = *src64[1];
		*dst64[2] = *src64[2];
		*dst64[3] = *src64[3];
		src64[0] += 4;
		dst64[0] += 4;

		*dst64[0] = *src64[0];
		src64[1] = src64[0] + 1;
		src64[2] = src64[0] + 2;
		src64[3] = src64[0] + 3;
		dst64[1] = dst64[0] + 1;
		dst64[2] = dst64[0] + 2;
		dst64[3] = dst64[0] + 3;
		*dst64[1] = *src64[1];
		*dst64[2] = *src64[2];
		*dst64[3] = *src64[3];
		src64[0] += 4;
		dst64[0] += 4;
	} while (--nblock);
}

void MOCKABLE(psmi_mq_mtucpy)(void *vdest, const void *vsrc, uint32_t nchars)
{

#ifdef PSM_CUDA
	if (PSMI_IS_CUDA_ENABLED && (PSMI_IS_CUDA_MEM(vdest) || PSMI_IS_CUDA_MEM((void *) vsrc))) {
		PSMI_CUDA_CALL(cuMemcpy,
			       (CUdeviceptr)vdest, (CUdeviceptr)vsrc, nchars);
		return;
	}
#endif
	memcpy(vdest, vsrc, nchars);
	return;


}
MOCK_DEF_EPILOGUE(psmi_mq_mtucpy);

void psmi_mq_mtucpy_host_mem(void *vdest, const void *vsrc, uint32_t nchars)
{
	memcpy(vdest, vsrc, nchars);
	return;
}
