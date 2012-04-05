/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2011  Université de Bordeaux 1
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

#ifndef __HASH_H__
#define __HASH_H__

#include <stdint.h>
#include <stddef.h>

/* Compute the CRC of a byte buffer seeded by the inputcrc "current state".
 * The return value should be considered as the new "current state" for future
 * CRC computation. */
uint32_t starpu_crc32_be_n(void *input, size_t n, uint32_t inputcrc);

/* Compute the CRC of a 32bit number seeded by the inputcrc "current state".
 * The return value should be considered as the new "current state" for future
 * CRC computation. */
uint32_t starpu_crc32_be(uint32_t input, uint32_t inputcrc);

/* Compute the CRC of a string seeded by the inputcrc "current state".  The
 * return value should be considered as the new "current state" for future CRC
 * computation. */
uint32_t starpu_crc32_string(char *str, uint32_t inputcrc);

#endif // __HASH_H__
