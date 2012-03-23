/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

/* Wrapper to avoid msys' tendency to turn / into \ and : into ;  */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *prog, *arch, *def, *name, *lib;
	char s[1024];

	if (argc != 6)
	{
		fprintf(stderr,"bad number of arguments");
		exit(EXIT_FAILURE);
	}

	prog = argv[1];
	arch = argv[2];
	def = argv[3];
	name = argv[4];
	lib = argv[5];

	snprintf(s, sizeof(s), "\"%s\" /machine:%s /def:%s /name:%s /out:%s",
		 prog, arch, def, name, lib);
	if (system(s))
	{
		fprintf(stderr, "%s failed\n", s);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
