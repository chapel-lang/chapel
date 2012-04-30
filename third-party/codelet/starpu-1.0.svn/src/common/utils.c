/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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
#include <common/config.h>
#include <common/utils.h>
#include <libgen.h>
#include <errno.h>

#ifdef __MINGW32__
#include <io.h>
#define mkdir(path, mode) mkdir(path)
#endif

/* Function with behaviour like `mkdir -p'. This function was adapted from
 * http://niallohiggins.com/2009/01/08/mkpath-mkdir-p-alike-in-c-for-unix/ */

int _starpu_mkpath(const char *s, mode_t mode)
{
	int olderrno;
	char *q, *r = NULL, *path = NULL, *up = NULL;
	int rv;

	rv = -1;
	if (strcmp(s, ".") == 0 || strcmp(s, "/") == 0
#ifdef __MINGW32__
		/* C:/ or C:\ */
		|| (s[0] && s[1] == ':' && (s[2] == '/' || s[2] == '\\') && !s[3])
#endif
		)
		return 0;

	if ((path = strdup(s)) == NULL)
		STARPU_ABORT();

	if ((q = strdup(s)) == NULL)
		STARPU_ABORT();

	if ((r = dirname(q)) == NULL)
		goto out;

	if ((up = strdup(r)) == NULL)
		STARPU_ABORT();

	if ((_starpu_mkpath(up, mode) == -1) && (errno != EEXIST))
		goto out;

	if ((mkdir(path, mode) == -1) && (errno != EEXIST))
		rv = -1;
	else
		rv = 0;

out:
	olderrno = errno;
	if (up)
		free(up);

	free(q);
	free(path);
	errno = olderrno;
	return rv;
}

int _starpu_check_mutex_deadlock(pthread_mutex_t *mutex)
{
	int ret;
	ret = pthread_mutex_trylock(mutex);
	if (!ret)
	{
		_STARPU_PTHREAD_MUTEX_UNLOCK(mutex);
		return 0;
	}

	if (ret == EBUSY)
		return 0;

	STARPU_ASSERT (ret != EDEADLK);

	return 1;
}
