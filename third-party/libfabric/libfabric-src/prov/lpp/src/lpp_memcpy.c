/*
 * Copyright (c) 2014, NVIDIA CORPORATION. All rights reserved.
 * Copyright (c) 2018-2024 GigaIO Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sys/param.h>
#include <immintrin.h>

#include "lpp.h"

lpp_memcpy lpp_io_memcpy = memcpy;

// Derived from gdrcopy.
static void * __attribute__((__target__("avx")))
lpp_memcpy_avx256(void *dest, const void *src, size_t n_bytes)
{
    char *d = (char*)dest;
    uintptr_t d_int = (uintptr_t)d;
    const char *s = (const char *)src;
    uintptr_t s_int = (uintptr_t)s;
    size_t n = n_bytes;

    // align dest to 256-bits
    if (d_int & 0x1f) {
        size_t nh = MIN(0x20 - (d_int & 0x1f), n);
        memcpy(d, s, nh);
        d += nh; d_int += nh;
        s += nh; s_int += nh;
        n -= nh;
    }

    if (s_int & 0x1f) { // src is not aligned to 256-bits
        __m256d r0,r1,r2,r3;
        // unroll 4
        while (n >= 4*sizeof(__m256d)) {
            r0 = _mm256_loadu_pd((double *)(s+0*sizeof(__m256d)));
            r1 = _mm256_loadu_pd((double *)(s+1*sizeof(__m256d)));
            r2 = _mm256_loadu_pd((double *)(s+2*sizeof(__m256d)));
            r3 = _mm256_loadu_pd((double *)(s+3*sizeof(__m256d)));
            _mm256_stream_pd((double *)(d+0*sizeof(__m256d)), r0);
            _mm256_stream_pd((double *)(d+1*sizeof(__m256d)), r1);
            _mm256_stream_pd((double *)(d+2*sizeof(__m256d)), r2);
            _mm256_stream_pd((double *)(d+3*sizeof(__m256d)), r3);
            s += 4*sizeof(__m256d);
            d += 4*sizeof(__m256d);
            n -= 4*sizeof(__m256d);
        }
        while (n >= sizeof(__m256d)) {
            r0 = _mm256_loadu_pd((double *)(s));
            _mm256_stream_pd((double *)(d), r0);
            s += sizeof(__m256d);
            d += sizeof(__m256d);
            n -= sizeof(__m256d);
        }
    } else { // or it IS aligned
        __m256d r0,r1,r2,r3,r4,r5,r6,r7;
        // unroll 8
        while (n >= 8*sizeof(__m256d)) {
            r0 = _mm256_load_pd((double *)(s+0*sizeof(__m256d)));
            r1 = _mm256_load_pd((double *)(s+1*sizeof(__m256d)));
            r2 = _mm256_load_pd((double *)(s+2*sizeof(__m256d)));
            r3 = _mm256_load_pd((double *)(s+3*sizeof(__m256d)));
            r4 = _mm256_load_pd((double *)(s+4*sizeof(__m256d)));
            r5 = _mm256_load_pd((double *)(s+5*sizeof(__m256d)));
            r6 = _mm256_load_pd((double *)(s+6*sizeof(__m256d)));
            r7 = _mm256_load_pd((double *)(s+7*sizeof(__m256d)));
            _mm256_stream_pd((double *)(d+0*sizeof(__m256d)), r0);
            _mm256_stream_pd((double *)(d+1*sizeof(__m256d)), r1);
            _mm256_stream_pd((double *)(d+2*sizeof(__m256d)), r2);
            _mm256_stream_pd((double *)(d+3*sizeof(__m256d)), r3);
            _mm256_stream_pd((double *)(d+4*sizeof(__m256d)), r4);
            _mm256_stream_pd((double *)(d+5*sizeof(__m256d)), r5);
            _mm256_stream_pd((double *)(d+6*sizeof(__m256d)), r6);
            _mm256_stream_pd((double *)(d+7*sizeof(__m256d)), r7);
            s += 8*sizeof(__m256d);
            d += 8*sizeof(__m256d);
            n -= 8*sizeof(__m256d);
        }
        while (n >= sizeof(__m256d)) {
            r0 = _mm256_load_pd((double *)(s));
            _mm256_stream_pd((double *)(d), r0);
            s += sizeof(__m256d);
            d += sizeof(__m256d);
            n -= sizeof(__m256d);
        }
    }

    if (n)
        memcpy(d, s, n);

    return dest;
}

// Derived from gdrcopy.
static void * __attribute__((__target__("avx512f")))
lpp_memcpy_avx512(void *dest, const void *src, size_t n_bytes)
{
    char *d = (char*)dest;
    uintptr_t d_int = (uintptr_t)d;
    const char *s = (const char *)src;
    uintptr_t s_int = (uintptr_t)s;
    size_t n = n_bytes;

    // align dest to 512-bits
    if (d_int & 0x3f) {
        size_t nh = MIN(0x40 - (d_int & 0x3f), n);
        memcpy(d, s, nh);
        d += nh; d_int += nh;
        s += nh; s_int += nh;
        n -= nh;
    }

    if (s_int & 0x3f) { // src is not aligned to 512-bits
        __m512d r0,r1,r2,r3;
        // unroll 4
        while (n >= 4*sizeof(__m512d)) {
            r0 = _mm512_loadu_pd((double *)(s+0*sizeof(__m512d)));
            r1 = _mm512_loadu_pd((double *)(s+1*sizeof(__m512d)));
            r2 = _mm512_loadu_pd((double *)(s+2*sizeof(__m512d)));
            r3 = _mm512_loadu_pd((double *)(s+3*sizeof(__m512d)));
            _mm512_stream_pd((double *)(d+0*sizeof(__m512d)), r0);
            _mm512_stream_pd((double *)(d+1*sizeof(__m512d)), r1);
            _mm512_stream_pd((double *)(d+2*sizeof(__m512d)), r2);
            _mm512_stream_pd((double *)(d+3*sizeof(__m512d)), r3);
            s += 4*sizeof(__m512d);
            d += 4*sizeof(__m512d);
            n -= 4*sizeof(__m512d);
        }
        while (n >= sizeof(__m512d)) {
            r0 = _mm512_loadu_pd((double *)(s));
            _mm512_stream_pd((double *)(d), r0);
            s += sizeof(__m512d);
            d += sizeof(__m512d);
            n -= sizeof(__m512d);
        }
    } else { // or it IS aligned
        __m512d r0,r1,r2,r3,r4,r5,r6,r7;
        // unroll 8
        while (n >= 8*sizeof(__m512d)) {
            r0 = _mm512_load_pd((double *)(s+0*sizeof(__m512d)));
            r1 = _mm512_load_pd((double *)(s+1*sizeof(__m512d)));
            r2 = _mm512_load_pd((double *)(s+2*sizeof(__m512d)));
            r3 = _mm512_load_pd((double *)(s+3*sizeof(__m512d)));
            r4 = _mm512_load_pd((double *)(s+4*sizeof(__m512d)));
            r5 = _mm512_load_pd((double *)(s+5*sizeof(__m512d)));
            r6 = _mm512_load_pd((double *)(s+6*sizeof(__m512d)));
            r7 = _mm512_load_pd((double *)(s+7*sizeof(__m512d)));
            _mm512_stream_pd((double *)(d+0*sizeof(__m512d)), r0);
            _mm512_stream_pd((double *)(d+1*sizeof(__m512d)), r1);
            _mm512_stream_pd((double *)(d+2*sizeof(__m512d)), r2);
            _mm512_stream_pd((double *)(d+3*sizeof(__m512d)), r3);
            _mm512_stream_pd((double *)(d+4*sizeof(__m512d)), r4);
            _mm512_stream_pd((double *)(d+5*sizeof(__m512d)), r5);
            _mm512_stream_pd((double *)(d+6*sizeof(__m512d)), r6);
            _mm512_stream_pd((double *)(d+7*sizeof(__m512d)), r7);
            s += 8*sizeof(__m512d);
            d += 8*sizeof(__m512d);
            n -= 8*sizeof(__m512d);
        }
        while (n >= sizeof(__m512d)) {
            r0 = _mm512_load_pd((double *)(s));
            _mm512_stream_pd((double *)(d), r0);
            s += sizeof(__m512d);
            d += sizeof(__m512d);
            n -= sizeof(__m512d);
        }
    }

    if (n)
        memcpy(d, s, n);

    return dest;
}

void lpp_init_memcpy()
{
    if (lpp_system_memcpy) {
        FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "using system memcpy\n");
        lpp_io_memcpy = memcpy;
    } else if (__builtin_cpu_supports("avx512f")) {
        FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "using avx512f memcpy\n");
        lpp_io_memcpy = lpp_memcpy_avx512;
    } else if (__builtin_cpu_supports("avx")) {
        FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "using avx256 memcpy\n");
        lpp_io_memcpy = lpp_memcpy_avx256;
    } else {
        FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "using system memcpy\n");
        lpp_io_memcpy = memcpy;
    }
}
