/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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

#include "../common/helper.h"

static int
return_77(void)
{
	return 77; /* Leave this statement alone ! */
}

int
main(void)
{
	if (foo)
	{
		return 77; /* => return STARPU_TEST_SKIPPED; */
	}
	return 77; /* => return STARPU_TEST_SKIPPED; */
}

int
main(void)
{
	if (bar)
		return 0; /* => return EXIT_SUCCESS; */

	/* XXX : This works, but the output is ugly :
	 *
	 * + return STARPU_TEST_SKIPPED; return STARPU_TEST_SKIPPED;
	 */
	if (foo)
		return 77; /* => return STARPU_TEST_SKIPPED; */

	return 77; /* => return STARPU_TEST_SKIPPED; */
}

