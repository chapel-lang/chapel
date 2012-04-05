/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
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

#include <starpu.h>
#include "../helper.h"

unsigned token = 0;
starpu_data_handle_t token_handle;

void callback(void *arg __attribute__ ((unused)))
{
	token = 42;
        starpu_data_release(token_handle);
}

int main(int argc, char **argv)
{
	int ret;

        ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_variable_data_register(&token_handle, 0, (uintptr_t)&token, sizeof(unsigned));
        starpu_data_acquire_cb(token_handle, STARPU_RW, callback, NULL);

	starpu_data_unregister(token_handle);

        FPRINTF(stderr, "Token: %u\n", token);
        STARPU_ASSERT(token == 42);

	starpu_shutdown();

	return EXIT_SUCCESS;
}
