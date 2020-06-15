/*
 * Copyright (c) 2013-2016 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#ifdef __FreeBSD__
#include <pthread_np.h>
#endif
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "ofi.h"
#include "ofi_osd.h"
#include "ofi_file.h"
#include "ofi_util.h"

#include "rdma/fi_errno.h"
#include "rdma/providers/fi_log.h"

#ifdef __FreeBSD__
typedef cpuset_t ofi_cpu_set_t;
#elif defined __linux__
typedef cpu_set_t ofi_cpu_set_t;
#endif

#if !HAVE_CLOCK_GETTIME
int clock_gettime(clockid_t clk_id, struct timespec *tp) {
	int retval;
	struct timeval tv;

	retval = gettimeofday(&tv, NULL);

	tp->tv_sec = tv.tv_sec;
	tp->tv_nsec = tv.tv_usec * 1000;

	return retval;
}
#endif /* !HAVE_CLOCK_GETTIME */

int fi_fd_nonblock(int fd)
{
	long flags = 0;

	flags = fcntl(fd, F_GETFL);
	if (flags < 0) {
		return -errno;
	}

	if(fcntl(fd, F_SETFL, flags | O_NONBLOCK))
		return -errno;

	return 0;
}

int fi_wait_cond(pthread_cond_t *cond, pthread_mutex_t *mut, int timeout_ms)
{
	uint64_t t;
	struct timespec ts;

	if (timeout_ms < 0)
		return pthread_cond_wait(cond, mut);

	t = ofi_gettime_ms() + timeout_ms;
	ts.tv_sec = t / 1000;
	ts.tv_nsec = (t % 1000) * 1000000;
	return pthread_cond_timedwait(cond, mut, &ts);
}

int ofi_shm_map(struct util_shm *shm, const char *name, size_t size,
		int readonly, void **mapped)
{
	char *fname = 0;
	int i, ret = FI_SUCCESS;
	int flags = O_RDWR | (readonly ? 0 : O_CREAT);
	struct stat mapstat;

	*mapped = MAP_FAILED;
	memset(shm, 0, sizeof(*shm));

	fname = calloc(1, strlen(name) + 2); /* '/' + %s + trailing 0 */
	if (!fname)
		return -FI_ENOMEM;

	strcpy(fname, "/");
	strcat(fname, name);
	shm->name = fname;

	for (i = 0; i < strlen(fname); i++) {
		if (fname[i] == ' ')
			fname[i] = '_';
	}

	FI_DBG(&core_prov, FI_LOG_CORE,
		"Creating shm segment :%s (size: %lu)\n", fname, size);

	shm->shared_fd = shm_open(fname, flags, S_IRUSR | S_IWUSR);
	if (shm->shared_fd < 0) {
		FI_WARN(&core_prov, FI_LOG_CORE, "shm_open failed\n");
		ret = -FI_EINVAL;
		goto failed;
	}

	if (fstat(shm->shared_fd, &mapstat)) {
		FI_WARN(&core_prov, FI_LOG_CORE, "failed to do fstat: %s\n",
			strerror(errno));
		ret = -FI_EINVAL;
		goto failed;
	}

	if (mapstat.st_size == 0) {
		if (ftruncate(shm->shared_fd, size)) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"ftruncate failed: %s\n", strerror(errno));
			ret = -FI_EINVAL;
			goto failed;
		}
	} else if (mapstat.st_size < size) {
		FI_WARN(&core_prov, FI_LOG_CORE, "shm file too small\n");
		ret = -FI_EINVAL;
		goto failed;
	}

	shm->ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
				MAP_SHARED, shm->shared_fd, 0);
	if (shm->ptr == MAP_FAILED) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"mmap failed: %s\n", strerror(errno));
		ret = -FI_EINVAL;
		goto failed;
	}

	*mapped = shm->ptr;
	shm->size = size;
	return ret;

failed:
	if (shm->shared_fd >= 0) {
		close(shm->shared_fd);
		shm_unlink(fname);
	}
	if (fname)
		free(fname);
	memset(shm, 0, sizeof(*shm));
	return ret;
}

int ofi_shm_unmap(struct util_shm* shm)
{
	if (shm->ptr && shm->ptr != MAP_FAILED) {
		if (munmap(shm->ptr, shm->size)) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"munmap failed: %s\n", strerror(errno));
		}
	}

	if (shm->shared_fd)
		close(shm->shared_fd);
	if (shm->name) {
		shm_unlink(shm->name);
		free((void*)shm->name);
	}
	memset(shm, 0, sizeof(*shm));
	return FI_SUCCESS;
}

int fi_read_file(const char *dir, const char *file, char *buf, size_t size)
{
	char *path;
	int fd, len;

	if (asprintf(&path, "%s/%s", dir, file) < 0)
		return -1;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		free(path);
		return -1;
	}

	len = read(fd, buf, size);
	close(fd);
	free(path);

	if (len > 0 && buf[len - 1] == '\n')
		buf[--len] = '\0';

	return len;
}

int ofi_set_thread_affinity(const char *s)
{
#ifndef __APPLE__
	char *saveptra = NULL, *saveptrb = NULL, *saveptrc = NULL;
	char *dup_s, *a, *b, *c;
	int j, first, last, stride, ret = FI_SUCCESS;
	ofi_cpu_set_t mycpuset;
	pthread_t mythread;

	mythread = pthread_self();
	CPU_ZERO(&mycpuset);

	dup_s = strdup(s);
	if (dup_s == NULL)
		return -FI_ENOMEM;

	a = strtok_r(dup_s, ",", &saveptra);
	while (a) {
		last = -1;
		stride = 1;
		b = strtok_r(a, "-", &saveptrb);
		assert(b);
		first = atoi(b);
		/* Check for range delimiter */
		b = strtok_r(NULL, "-", &saveptrb);
		if (b) {
			c = strtok_r(b, ":", &saveptrc);
			assert(c);
			last = atoi(c);
			/* Check for stride */
			c = strtok_r(NULL, ":", &saveptrc);
			if (c)
				stride = atoi(c);
		}

		if (last == -1)
			last = first;

		for (j = first; j <= last; j += stride)
			CPU_SET(j, &mycpuset);
		a =  strtok_r(NULL, ",", &saveptra);
	}

	ret = pthread_setaffinity_np(mythread, sizeof(mycpuset), &mycpuset);
	if (ret)
		ret = -errno;

	free(dup_s);
	return ret;
#else
	OFI_UNUSED(s);
	return -FI_ENOSYS;
#endif
}
