/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include <cuComplex.h>
#include <starpu_cuda.h>
_externC void STARPUFFT(cuda_twist1_1d_host)(const _cuComplex *in, _cuComplex *twisted1, unsigned i, unsigned n1, unsigned n2);
_externC void STARPUFFT(cuda_twiddle_1d_host)(_cuComplex *out, const _cuComplex *roots, unsigned n, unsigned i);
_externC void STARPUFFT(cuda_twist1_2d_host)(const _cuComplex *in, _cuComplex *twisted1, unsigned i, unsigned j, unsigned n1, unsigned n2, unsigned m1, unsigned m2);
_externC void STARPUFFT(cuda_twiddle_2d_host)(_cuComplex *out, const _cuComplex *roots0, const _cuComplex *roots1, unsigned n2, unsigned m2, unsigned i, unsigned j);
