/*
* Copyright (c) 2017 Intel Corporation.  All rights reserved.
*
* This software is available to you under the BSD license below:
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

#ifndef _WINDOWS_OSD_H_
#define _WINDOWS_OSD_H_

#include <winsock2.h>
#include <ws2def.h>
#include <windows.h>
#include <assert.h>
#include <inttypes.h>

#include <time.h>

struct iovec
{
	void *iov_base; /* Pointer to data.  */
	size_t iov_len; /* Length of data.  */
};

#define strdup _strdup
#define strncasecmp _strnicmp
#define SHUT_RDWR SD_BOTH
#define CLOCK_MONOTONIC	1

#ifndef EAI_SYSTEM
# define EAI_SYSTEM	-11
#endif

typedef int pid_t;

/*
 * The FILETIME structure records time in the form of
 * 100-nanosecond intervals since January 1, 1601
 */
#define file2unix_time	10000000i64		/* 1E+7 */
#define win2unix_epoch	116444736000000000i64	/* 1 Jan 1601 to 1 Jan 1970 */

static inline
int clock_gettime(int which_clock, struct timespec *spec)
{
	__int64 wintime;

	GetSystemTimeAsFileTime((FILETIME*)&wintime);
	wintime -= win2unix_epoch;

	spec->tv_sec = wintime / file2unix_time;
	spec->tv_nsec = wintime % file2unix_time * 100;

	return 0;
}

static inline int ft_close_fd(int fd)
{
	return closesocket(fd);
}

static inline int poll(struct pollfd *fds, int nfds, int timeout)
{
	return WSAPoll(fds, nfds, timeout);
}

static inline char* strndup(const char* str, size_t n)
{
	char* res = strdup(str);
	if (strlen(res) > n)
		res[n] = '\0';
	return res;
}

static inline char* strsep(char **stringp, const char *delim)
{
	char* ptr = *stringp;
	char* p;

	p = ptr ? strpbrk(ptr, delim) : NULL;

	if(!p)
		*stringp = NULL;
	else
	{
		*p = 0;
		*stringp = p + 1;
	}

	return ptr;
}

#define _SC_PAGESIZE	30

static long int sysconf(int name)
{
	switch (name) {
	case _SC_PAGESIZE:
		SYSTEM_INFO info;
		GetNativeSystemInfo(&info);
		return (long int)info.dwPageSize;
	default:
		assert(0);
	}
	errno = EINVAL;
	return -1;
}

#define AF_LOCAL AF_UNIX

int socketpair(int af, int type, int protocol, int socks[2]);

static inline int ft_fd_nonblock(int fd)
{
	u_long argp = 1;
	return ioctlsocket(fd, FIONBIO, &argp) ? -WSAGetLastError() : 0;
}

/* Bits in the fourth argument to `waitid'.  */
#define WSTOPPED	2	/* Report stopped child (same as WUNTRACED). */
#define WEXITED		4	/* Report dead child. */
#define WCONTINUED	8	/* Report continued child. */
#define WNOWAIT		0x01000000	/* Don't reap, just poll status. */

static pid_t waitpid(pid_t pid, int *status, int options)
{
	assert(0);
	return 0;
}

static const char* gai_strerror(int code)
{
	return "Unknown error";
}

static pid_t fork(void)
{
	assert(0);
	return -1;
}

static int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	*memptr = _aligned_malloc(size, alignment);
	return (*memptr) ? 0 : ENOMEM;
}

static inline int ft_startup(void)
{
	int ret = 0;
	WSADATA data;

	ret = WSAStartup(MAKEWORD(2, 2), &data);
	if (ret)
		return HRESULT_FROM_WIN32(ret);
	return ret;
}


/* complex operations implementation */
#define OFI_COMPLEX(name) ofi_##name##_complex
#define OFI_COMPLEX_BASE(name) OFI_COMPLEX(name)##_base
#define OFI_COMPLEX_OP(name, op) ofi_complex_##name##_##op
#define OFI_COMPLEX_TYPE_DECL(name, type)	\
typedef type OFI_COMPLEX_BASE(name);		\
typedef struct {				\
	OFI_COMPLEX_BASE(name) re;		\
	OFI_COMPLEX_BASE(name) im;		\
} OFI_COMPLEX(name);

OFI_COMPLEX_TYPE_DECL(float, float)
OFI_COMPLEX_TYPE_DECL(double, double)
OFI_COMPLEX_TYPE_DECL(long_double, long double)

#define OFI_COMPLEX_OPS(name)								\
static inline OFI_COMPLEX_BASE(name) OFI_COMPLEX_OP(name, real)(OFI_COMPLEX(name) v)	\
{											\
	return v.re;									\
} 											\
static inline OFI_COMPLEX_BASE(name) OFI_COMPLEX_OP(name, imag)(OFI_COMPLEX(name) v)	\
{											\
	return v.im;									\
}											\
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, sum)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2) \
{											\
	OFI_COMPLEX(name) ret = {.re = v1.re + v2.re, .im = v1.im + v2.im};		\
	return ret;									\
}											\
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, mul)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2) \
{											\
	OFI_COMPLEX(name) ret = {.re = (v1.re * v2.re) - (v1.im * v2.im),		\
			      .im = (v1.re * v2.im) + (v1.im * v2.re)};			\
	return ret;									\
}											\
static inline int OFI_COMPLEX_OP(name, equ)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2)	\
{											\
	return v1.re == v2.re && v1.im == v2.im;					\
}											\
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, land)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2) \
{											\
	OFI_COMPLEX(name) zero = {.re = 0, .im = 0};					\
	int equ = !OFI_COMPLEX_OP(name, equ)(v1, zero) && !OFI_COMPLEX_OP(name, equ)(v2, zero); \
	OFI_COMPLEX(name) ret = {.re = equ ? 1.f : 0, .im = 0};				\
	return ret;									\
}											\
static inline OFI_COMPLEX(name) OFI_COMPLEX_OP(name, lor)(OFI_COMPLEX(name) v1, OFI_COMPLEX(name) v2) \
{											\
	OFI_COMPLEX(name) zero = {.re = 0, .im = 0};					\
	int equ = !OFI_COMPLEX_OP(name, equ)(v1, zero) || !OFI_COMPLEX_OP(name, equ)(v2, zero); \
	OFI_COMPLEX(name) ret = {.re = equ ? 1.f : 0, .im = 0};				\
	return ret;									\
}

OFI_COMPLEX_OPS(float)
OFI_COMPLEX_OPS(double)
OFI_COMPLEX_OPS(long_double)

#endif /* _WINDOWS_OSD_H_ */
