/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
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
#include "mm_to_bcsr.h"

int main(int argc, char *argv[])
{
	unsigned c, r;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [martix-market-filename] [c] [r]\n", argv[0]);
		exit(1);
	}

	c = 64;
	r = 64;

	bcsr_t *bcsr;
	bcsr = mm_file_to_bcsr(argv[1], c, r);

	return 0;
}
