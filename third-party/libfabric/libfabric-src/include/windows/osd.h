/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef _FI_WIN_OSD_H_
#define _FI_WIN_OSD_H_

#include "config.h"

#include <WinSock2.h>
#include <Mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <io.h>
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <complex.h>
#include "pthread.h"

#include <sys/uio.h>
#include <time.h>

#include <rdma/fi_errno.h>
#include <rdma/fabric.h>

#include <ofi_osd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OFI_MAX_SOCKET_BUF_SIZE	INT_MAX

/*
 * The following defines redefine the Windows Socket
 * errors as BSD errors.
 */
#ifndef ENOTEMPTY
# define ENOTEMPTY		41	/* Directory not empty */
#endif
#ifndef EREMOTE
# define EREMOTE		66	/* The object is remote */
#endif
#ifndef EPFNOSUPPORT
# define EPFNOSUPPORT		96	/* Protocol family not supported */
#endif
#ifndef EADDRINUSE
# define EADDRINUSE		100	/* Address already in use */
#endif
#ifndef EADDRNOTAVAIL
# define EADDRNOTAVAIL		101	/* Can't assign requested address */
#endif
#ifndef EAFNOSUPPORT
# define EAFNOSUPPORT		102	/* Address family not supported */
#endif
#ifndef EALREADY
# define EALREADY		103	/* Operation already in progress */
#endif
#ifndef EBADMSG
# define EBADMSG		104	/* Not a data message */
#endif
#ifndef ECANCELED
# define ECANCELED		105	/* Canceled */
#endif
#ifndef ECONNABORTED
# define ECONNABORTED		106	/* Software caused connection abort */
#endif
#ifndef ECONNREFUSED
# define ECONNREFUSED		107	/* Connection refused */
#endif
#ifndef ECONNRESET
# define ECONNRESET		108	/* Connection reset by peer */
#endif
#ifndef EDESTADDRREQ
# define EDESTADDRREQ		109	/* Destination address required */
#endif
#ifndef EHOSTUNREACH
# define EHOSTUNREACH		110	/* No route to host */
#endif
#ifndef EIDRM
# define EIDRM			111	/* Identifier removed */
#endif
#ifndef EINPROGRESS
# define EINPROGRESS		112	/* Operation now in progress */
#endif
#ifndef EISCONN
# define EISCONN		113	/* Socket is already connected */
#endif
#ifndef ELOOP
# define ELOOP			114	/* Symbolic link loop */
#endif
#ifndef EMSGSIZE
# define EMSGSIZE		115	/* Message too long */
#endif
#ifndef ENETDOWN
# define ENETDOWN		116	/* Network is down */
#endif
#ifndef ENETRESET
# define ENETRESET		117	/* Network dropped connection on reset */
#endif
#ifndef ENETUNREACH
# define ENETUNREACH		118	/* Network is unreachable */
#endif
#ifndef ENOBUFS
# define ENOBUFS		119	/* No buffer space available */
#endif
#ifndef ENODATA
# define ENODATA		120	/* No data available */
#endif
#ifndef ENOLINK
# define ENOLINK		121	/* Link has be severed */
#endif
#ifndef ENOMSG
# define ENOMSG			122	/* No message of desired type */
#endif
#ifndef ENOPROTOOPT
# define ENOPROTOOPT		123	/* Protocol not available */
#endif
#ifndef ENOSR
# define ENOSR			124	/* Out of stream resources */
#endif
#ifndef ENOSTR
# define ENOSTR			125	/* Not a stream device */
#endif
#ifndef ENOTCONN
# define ENOTCONN		126	/* Socket is not connected */
#endif
#ifndef ENOTRECOVERABLE
# define ENOTRECOVERABLE	127	/* Not recoverable */
#endif
#ifndef ENOTSOCK
# define ENOTSOCK		128	/* Socket operation on non-socket */
#endif
#ifndef ENOTSUP
# define ENOTSUP		129	/* Operation not supported */
#endif
#ifndef EOPNOTSUPP
# define EOPNOTSUPP		130	/* Operation not supported on socket */
#endif
#ifndef EOTHER
# define EOTHER			131	/* Other error */
#endif
#ifndef EOVERFLOW
# define EOVERFLOW		132	/* File too big */
#endif
#ifndef EOWNERDEAD
# define EOWNERDEAD		133	/* Owner dead */
#endif
#ifndef EPROTO
# define EPROTO			134	/* Protocol error */
#endif
#ifndef EPROTONOSUPPORT
# define EPROTONOSUPPORT	135	/* Protocol not supported */
#endif
#ifndef EPROTOTYPE
# define EPROTOTYPE		136	/* Protocol wrong type for socket */
#endif
#ifndef ETIME
# define ETIME			137	/* Timer expired */
#endif
#ifndef ETIMEDOUT
# define ETIMEDOUT		138	/* Connection timed out */
#endif
#ifndef ETXTBSY
# define ETXTBSY		139	/* Text file or pseudo-device busy */
#endif
#ifndef EWOULDBLOCK
# define EWOULDBLOCK		140	/* Operation would block */
#endif

/* Visual Studio doesn't have these, so just choose some high numbers */
#ifndef ESOCKTNOSUPPORT
# define ESOCKTNOSUPPORT	240	/* Socket type not supported */
#endif
#ifndef ESHUTDOWN
# define ESHUTDOWN		241	/* Can't send after socket shutdown */
#endif
#ifndef ETOOMANYREFS
# define ETOOMANYREFS		242	/* Too many references: can't splice */
#endif
#ifndef EHOSTDOWN
# define EHOSTDOWN		243	/* Host is down */
#endif
#ifndef EUSERS
# define EUSERS			244	/* Too many users (for UFS) */
#endif
#ifndef EDQUOT
# define EDQUOT			245	/* Disc quota exceeded */
#endif
#ifndef ESTALE
# define ESTALE			246	/* Stale NFS file handle */
#endif

/* MSG_NOSIGNAL doesn't exist on Windows */
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL	0
#endif

#ifndef SHUT_RDWR
#define SHUT_RDWR	SD_BOTH
#endif


#define FI_DESTRUCTOR(func) void func

#define LITTLE_ENDIAN 5678
#define BIG_ENDIAN 8765
#define BYTE_ORDER LITTLE_ENDIAN

#define OFI_SOCK_TRY_SND_RCV_AGAIN(err)		\
	(((err) == ETIMEDOUT)		||	\
	 ((err) == EWOULDBLOCK)		||	\
	 ((err) == EAGAIN))

#define OFI_SOCK_TRY_ACCEPT_AGAIN(err)		\
	(((err) == EAGAIN)		||	\
	 ((err) == EWOULDBLOCK))

#define OFI_SOCK_TRY_CONN_AGAIN(err)		\
	(((err) == EWOULDBLOCK)		||	\
	 ((err) == EINPROGRESS))

struct util_shm
{ /* this is dummy structure to provide compilation on Windows platform. */
  /* will be updated on real Windows implementation */
	HANDLE		shared_fd;
	void		*ptr;
	const char	*name;
	size_t		size;
};

#define FI_FFSL(val)	 			\
do						\
{						\
	int i = 0;				\
	while(val)				\
	{					\
		if((val) & 1)			\
		{				\
			return i + 1; 		\
		}				\
		else				\
		{				\
			i++;			\
			(val) = (val) >> 1;	\
		}				\
	}					\
} while(0)

#define strdup _strdup
#define strcasecmp _stricmp
#define snprintf _snprintf
#define sleep(x) Sleep(x * 1000)
#define strtok_r strtok_s

#define __PRI64_PREFIX "ll"

#define HOST_NAME_MAX 256

#define MIN min
#define MAX max
#define OFI_UNUSED UNREFERENCED_PARAMETER

#define htonll _byteswap_uint64
#define ntohll _byteswap_uint64
#define strncasecmp _strnicmp

typedef int pid_t;
#define getpid (int)GetCurrentProcessId

int fd_set_nonblock(int fd);

int socketpair(int af, int type, int protocol, int socks[2]);
void sock_get_ip_addr_table(struct slist *addr_list);
int ofi_getsockname(SOCKET fd, struct sockaddr *addr, socklen_t *len);
int ofi_getpeername(SOCKET fd, struct sockaddr *addr, socklen_t *len);

/*
 * Win32 error code should be passed as a parameter.
 * This routine converts a Win32 error into an errno value.
 */
static unsigned char winerr2bsderr(DWORD win_errcode) /* do NOT use directly */
{
	/*
	* The following table contains the mapping from Win32 errors to errno errors.
	*/
	static const unsigned char error_table[] = {
		0,
		EINVAL,		/* ERROR_INVALID_FUNCTION		1*/
		ENOENT,		/* ERROR_FILE_NOT_FOUND			2 */
		ENOENT,		/* ERROR_PATH_NOT_FOUND			3 */
		EMFILE,		/* ERROR_TOO_MANY_OPEN_FILES		4 */
		EACCES,		/* ERROR_ACCESS_DENIED			5 */
		EBADF,		/* ERROR_INVALID_HANDLE			6 */
		ENOMEM,		/* ERROR_ARENA_TRASHED			7 */
		ENOMEM,		/* ERROR_NOT_ENOUGH_MEMORY		8 */
		ENOMEM,		/* ERROR_INVALID_BLOCK			9 */
		E2BIG,		/* ERROR_BAD_ENVIRONMENT		10 */
		ENOEXEC,	/* ERROR_BAD_FORMAT			11 */
		EACCES,		/* ERROR_INVALID_ACCESS			12 */
		EINVAL,		/* ERROR_INVALID_DATA			13 */
		EFAULT,		/* ERROR_OUT_OF_MEMORY			14 */
		ENOENT,		/* ERROR_INVALID_DRIVE			15 */
		EACCES,		/* ERROR_CURRENT_DIRECTORY		16 */
		EXDEV,		/* ERROR_NOT_SAME_DEVICE		17 */
		ENOENT,		/* ERROR_NO_MORE_FILES			18 */
		EROFS,		/* ERROR_WRITE_PROTECT			19 */
		ENXIO,		/* ERROR_BAD_UNIT			20 */
		EBUSY,		/* ERROR_NOT_READY			21 */
		EIO,		/* ERROR_BAD_COMMAND			22 */
		EIO,		/* ERROR_CRC				23 */
		EIO,		/* ERROR_BAD_LENGTH			24 */
		EIO,		/* ERROR_SEEK				25 */
		EIO,		/* ERROR_NOT_DOS_DISK			26 */
		ENXIO,		/* ERROR_SECTOR_NOT_FOUND		27 */
		EBUSY,		/* ERROR_OUT_OF_PAPER			28 */
		EIO,		/* ERROR_WRITE_FAULT			29 */
		EIO,		/* ERROR_READ_FAULT			30 */
		EIO,		/* ERROR_GEN_FAILURE			31 */
		EACCES,		/* ERROR_SHARING_VIOLATION		32 */
		EACCES,		/* ERROR_LOCK_VIOLATION			33 */
		ENXIO,		/* ERROR_WRONG_DISK			34 */
		ENFILE,		/* ERROR_FCB_UNAVAILABLE		35 */
		ENFILE,		/* ERROR_SHARING_BUFFER_EXCEEDED	36 */
		EINVAL,		/*					37 */
		EINVAL,		/*					38 */
		ENOSPC,		/* ERROR_HANDLE_DISK_FULL		39 */
		EINVAL,		/*					40 */
		EINVAL,		/*					41 */
		EINVAL,		/*					42 */
		EINVAL,		/*					43 */
		EINVAL,		/*					44 */
		EINVAL,		/*					45 */
		EINVAL,		/*					46 */
		EINVAL,		/*					47 */
		EINVAL,		/*					48 */
		EINVAL,		/*					49 */
		ENODEV,		/* ERROR_NOT_SUPPORTED			50 */
		EBUSY,		/* ERROR_REM_NOT_LIST			51 */
		EEXIST,		/* ERROR_DUP_NAME			52 */
		ENOENT,		/* ERROR_BAD_NETPATH			53 */
		EBUSY,		/* ERROR_NETWORK_BUSY			54 */
		ENODEV,		/* ERROR_DEV_NOT_EXIST			55 */
		EAGAIN,		/* ERROR_TOO_MANY_CMDS			56 */
		EIO,		/* ERROR_ADAP_HDW_ERR			57 */
		EIO,		/* ERROR_BAD_NET_RESP			58 */
		EIO,		/* ERROR_UNEXP_NET_ERR			59 */
		EINVAL,		/* ERROR_BAD_REM_ADAP			60 */
		EFBIG,		/* ERROR_PRINTQ_FULL			61 */
		ENOSPC,		/* ERROR_NO_SPOOL_SPACE			62 */
		ENOENT,		/* ERROR_PRINT_CANCELLED		63 */
		ENOENT,		/* ERROR_NETNAME_DELETED		64 */
		EACCES,		/* ERROR_NETWORK_ACCESS_DENIED		65 */
		ENODEV,		/* ERROR_BAD_DEV_TYPE			66 */
		ENOENT,		/* ERROR_BAD_NET_NAME			67 */
		ENFILE,		/* ERROR_TOO_MANY_NAMES			68 */
		EIO,		/* ERROR_TOO_MANY_SESS			69 */
		EAGAIN,		/* ERROR_SHARING_PAUSED			70 */
		EINVAL,		/* ERROR_REQ_NOT_ACCEP			71 */
		EAGAIN,		/* ERROR_REDIR_PAUSED			72 */
		EINVAL,		/*					73 */
		EINVAL,		/*					74 */
		EINVAL,		/*					75 */
		EINVAL,		/*					76 */
		EINVAL,		/*					77 */
		EINVAL,		/*					78 */
		EINVAL,		/*					79 */
		EEXIST,		/* ERROR_FILE_EXISTS			80 */
		EINVAL,		/*					81 */
		ENOSPC,		/* ERROR_CANNOT_MAKE			82 */
		EIO,		/* ERROR_FAIL_I24			83 */
		ENFILE,		/* ERROR_OUT_OF_STRUCTURES		84 */
		EEXIST,		/* ERROR_ALREADY_ASSIGNED		85 */
		EPERM,		/* ERROR_INVALID_PASSWORD		86 */
		EINVAL,		/* ERROR_INVALID_PARAMETER		87 */
		EIO,		/* ERROR_NET_WRITE_FAULT		88 */
		EAGAIN,		/* ERROR_NO_PROC_SLOTS			89 */
		EINVAL,		/*					90 */
		EINVAL,		/*					91 */
		EINVAL,		/*					92 */
		EINVAL,		/*					93 */
		EINVAL,		/*					94 */
		EINVAL,		/*					95 */
		EINVAL,		/*					96 */
		EINVAL,		/*					97 */
		EINVAL,		/*					98 */
		EINVAL,		/*					99 */
		EINVAL,		/*					100 */
		EINVAL,		/*					101 */
		EINVAL,		/*					102 */
		EINVAL,		/*					103 */
		EINVAL,		/*					104 */
		EINVAL,		/*					105 */
		EINVAL,		/*					106 */
		EXDEV,		/* ERROR_DISK_CHANGE			107 */
		EAGAIN,		/* ERROR_DRIVE_LOCKED			108 */
		EPIPE,		/* ERROR_BROKEN_PIPE			109 */
		ENOENT,		/* ERROR_OPEN_FAILED			110 */
		EINVAL,		/* ERROR_BUFFER_OVERFLOW		111 */
		ENOSPC,		/* ERROR_DISK_FULL			112 */
		EMFILE,		/* ERROR_NO_MORE_SEARCH_HANDLES		113 */
		EBADF,		/* ERROR_INVALID_TARGET_HANDLE		114 */
		EFAULT,		/* ERROR_PROTECTION_VIOLATION		115 */
		EINVAL,		/*					116 */
		EINVAL,		/*					117 */
		EINVAL,		/*					118 */
		EINVAL,		/*					119 */
		EINVAL,		/*					120 */
		EINVAL,		/*					121 */
		EINVAL,		/*					122 */
		ENOENT,		/* ERROR_INVALID_NAME			123 */
		EINVAL,		/*					124 */
		EINVAL,		/*					125 */
		EINVAL,		/*					126 */
		EINVAL,		/* ERROR_PROC_NOT_FOUND			127 */
		ECHILD,		/* ERROR_WAIT_NO_CHILDREN		128 */
		ECHILD,		/* ERROR_CHILD_NOT_COMPLETE		129 */
		EBADF,		/* ERROR_DIRECT_ACCESS_HANDLE		130 */
		EINVAL,		/* ERROR_NEGATIVE_SEEK			131 */
		ESPIPE,		/* ERROR_SEEK_ON_DEVICE			132 */
		EINVAL,		/*					133 */
		EINVAL,		/*					134 */
		EINVAL,		/*					135 */
		EINVAL,		/*					136 */
		EINVAL,		/*					137 */
		EINVAL,		/*					138 */
		EINVAL,		/*					139 */
		EINVAL,		/*					140 */
		EINVAL,		/*					141 */
		EAGAIN,		/* ERROR_BUSY_DRIVE			142 */
		EINVAL,		/*					143 */
		EINVAL,		/*					144 */
		EEXIST,		/* ERROR_DIR_NOT_EMPTY			145 */
		EINVAL,		/*					146 */
		EINVAL,		/*					147 */
		EINVAL,		/*					148 */
		EINVAL,		/*					149 */
		EINVAL,		/*					150 */
		EINVAL,		/*					151 */
		EINVAL,		/*					152 */
		EINVAL,		/*					153 */
		EINVAL,		/*					154 */
		EINVAL,		/*					155 */
		EINVAL,		/*					156 */
		EINVAL,		/*					157 */
		EACCES,		/* ERROR_NOT_LOCKED			158 */
		EINVAL,		/*					159 */
		EINVAL,		/*					160 */
		ENOENT,		/* ERROR_BAD_PATHNAME			161 */
		EINVAL,		/*					162 */
		EINVAL,		/*					163 */
		EINVAL,		/*					164 */
		EINVAL,		/*					165 */
		EINVAL,		/*					166 */
		EACCES,		/* ERROR_LOCK_FAILED			167 */
		EINVAL,		/*					168 */
		EINVAL,		/*					169 */
		EINVAL,		/*					170 */
		EINVAL,		/*					171 */
		EINVAL,		/*					172 */
		EINVAL,		/*					173 */
		EINVAL,		/*					174 */
		EINVAL,		/*					175 */
		EINVAL,		/*					176 */
		EINVAL,		/*					177 */
		EINVAL,		/*					178 */
		EINVAL,		/*					179 */
		EINVAL,		/*					180 */
		EINVAL,		/*					181 */
		EINVAL,		/*					182 */
		EEXIST,		/* ERROR_ALREADY_EXISTS			183 */
		ECHILD,		/* ERROR_NO_CHILD_PROCESS		184 */
		EINVAL,		/*					185 */
		EINVAL,		/*					186 */
		EINVAL,		/*					187 */
		EINVAL,		/*					188 */
		EINVAL,		/*					189 */
		EINVAL,		/*					190 */
		EINVAL,		/*					191 */
		EINVAL,		/*					192 */
		EINVAL,		/*					193 */
		EINVAL,		/*					194 */
		EINVAL,		/*					195 */
		EINVAL,		/*					196 */
		EINVAL,		/*					197 */
		EINVAL,		/*					198 */
		EINVAL,		/*					199 */
		EINVAL,		/*					200 */
		EINVAL,		/*					201 */
		EINVAL,		/*					202 */
		EINVAL,		/*					203 */
		EINVAL,		/*					204 */
		EINVAL,		/*					205 */
		ENAMETOOLONG,	/* ERROR_FILENAME_EXCED_RANGE		206 */
		EINVAL,		/*					207 */
		EINVAL,		/*					208 */
		EINVAL,		/*					209 */
		EINVAL,		/*					210 */
		EINVAL,		/*					211 */
		EINVAL,		/*					212 */
		EINVAL,		/*					213 */
		EINVAL,		/*					214 */
		EINVAL,		/*					215 */
		EINVAL,		/*					216 */
		EINVAL,		/*					217 */
		EINVAL,		/*					218 */
		EINVAL,		/*					219 */
		EINVAL,		/*					220 */
		EINVAL,		/*					221 */
		EINVAL,		/*					222 */
		EINVAL,		/*					223 */
		EINVAL,		/*					224 */
		EINVAL,		/*					225 */
		EINVAL,		/*					226 */
		EINVAL,		/*					227 */
		EINVAL,		/*					228 */
		EINVAL,		/*					229 */
		EPIPE,		/* ERROR_BAD_PIPE			230 */
		EAGAIN,		/* ERROR_PIPE_BUSY			231 */
		EPIPE,		/* ERROR_NO_DATA			232 */
		EPIPE,		/* ERROR_PIPE_NOT_CONNECTED		233 */
		EINVAL,		/*					234 */
		EINVAL,		/*					235 */
		EINVAL,		/*					236 */
		EINVAL,		/*					237 */
		EINVAL,		/*					238 */
		EINVAL,		/*					239 */
		EINVAL,		/*					240 */
		EINVAL,		/*					241 */
		EINVAL,		/*					242 */
		EINVAL,		/*					243 */
		EINVAL,		/*					244 */
		EINVAL,		/*					245 */
		EINVAL,		/*					246 */
		EINVAL,		/*					247 */
		EINVAL,		/*					248 */
		EINVAL,		/*					249 */
		EINVAL,		/*					250 */
		EINVAL,		/*					251 */
		EINVAL,		/*					252 */
		EINVAL,		/*					253 */
		EINVAL,		/*					254 */
		EINVAL,		/*					255 */
		EINVAL,		/*					256 */
		EINVAL,		/*					257 */
		EINVAL,		/*					258 */
		EINVAL,		/*					259 */
		EINVAL,		/*					260 */
		EINVAL,		/*					261 */
		EINVAL,		/*					262 */
		EINVAL,		/*					263 */
		EINVAL,		/*					264 */
		EINVAL,		/*					265 */
		EINVAL,		/*					266 */
		ENOTDIR		/* ERROR_DIRECTORY			267 */
	};

	/*
	* The following table contains the mapping from WinSock errors to
	* errno errors.
	*/
	static const unsigned char wsa_error_table[] = {
		EWOULDBLOCK,		/* WSAEWOULDBLOCK */
		EINPROGRESS,		/* WSAEINPROGRESS */
		EALREADY,		/* WSAEALREADY */
		ENOTSOCK,		/* WSAENOTSOCK */
		EDESTADDRREQ,		/* WSAEDESTADDRREQ */
		EMSGSIZE,		/* WSAEMSGSIZE */
		EPROTOTYPE,		/* WSAEPROTOTYPE */
		ENOPROTOOPT,		/* WSAENOPROTOOPT */
		EPROTONOSUPPORT,	/* WSAEPROTONOSUPPORT */
		ESOCKTNOSUPPORT,	/* WSAESOCKTNOSUPPORT */
		EOPNOTSUPP,		/* WSAEOPNOTSUPP */
		EPFNOSUPPORT,		/* WSAEPFNOSUPPORT */
		EAFNOSUPPORT,		/* WSAEAFNOSUPPORT */
		EADDRINUSE,		/* WSAEADDRINUSE */
		EADDRNOTAVAIL,		/* WSAEADDRNOTAVAIL */
		ENETDOWN,		/* WSAENETDOWN */
		ENETUNREACH,		/* WSAENETUNREACH */
		ENETRESET,		/* WSAENETRESET */
		ECONNABORTED,		/* WSAECONNABORTED */
		ECONNRESET,		/* WSAECONNRESET */
		ENOBUFS,		/* WSAENOBUFS */
		EISCONN,		/* WSAEISCONN */
		ENOTCONN,		/* WSAENOTCONN */
		ESHUTDOWN,		/* WSAESHUTDOWN */
		ETOOMANYREFS,		/* WSAETOOMANYREFS */
		ETIMEDOUT,		/* WSAETIMEDOUT */
		ECONNREFUSED,		/* WSAECONNREFUSED */
		ELOOP,			/* WSAELOOP */
		ENAMETOOLONG,		/* WSAENAMETOOLONG */
		EHOSTDOWN,		/* WSAEHOSTDOWN */
		EHOSTUNREACH,		/* WSAEHOSTUNREACH */
		ENOTEMPTY,		/* WSAENOTEMPTY */
		EAGAIN,			/* WSAEPROCLIM */
		EUSERS,			/* WSAEUSERS */
		EDQUOT,			/* WSAEDQUOT */
		ESTALE,			/* WSAESTALE */
		EREMOTE			/* WSAEREMOTE */
	};

	if (win_errcode >= sizeof(error_table) / sizeof(error_table[0])) {
		win_errcode -= WSAEWOULDBLOCK;
		if (win_errcode >= (sizeof(wsa_error_table) / sizeof(wsa_error_table[0]))) {
			return error_table[1];
		}
		else {
			return wsa_error_table[win_errcode];
		}
	}
	else {
		return error_table[win_errcode];
	}
}

static inline int ffsl(long val)
{
	unsigned long v = (unsigned long)val;
	FI_FFSL(v);
	return 0;
}

static inline int ffsll(long long val)
{
	unsigned long long v = (unsigned long long)val;
	FI_FFSL(v);
	return 0;
}

static inline int vasprintf(char **ptr, const char *format, va_list args)
{
	int len = vsnprintf(0, 0, format, args);
	*ptr = (char *)malloc(len + 1);
	vsnprintf(*ptr, len + 1, format, args);
	(*ptr)[len] = 0; /* to be sure that string is enclosed */
	return len;
}

static inline int asprintf(char **ptr, const char *format, ...)
{
	va_list args;
	int len;

	va_start(args, format);
	len = vasprintf(ptr, format, args);
	va_end(args);

	return len;
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

#define __attribute__(x)

static inline int ofi_memalign(void **memptr, size_t alignment, size_t size)
{
	*memptr = _aligned_malloc(size, alignment);
	return *memptr == 0;
}

static inline void ofi_freealign(void *memptr)
{
	_aligned_free(memptr);
}

static inline void ofi_osd_init(void)
{
	WORD wsa_version;
	WSADATA data;
	int err;

	wsa_version = MAKEWORD(2, 2);

	err = WSAStartup(wsa_version, &data);
}

static inline void ofi_osd_fini(void)
{
	WSACleanup();
}

static inline SOCKET ofi_socket(int domain, int type, int protocol)
{
	return socket(domain, type, protocol);
}

/*
 * The windows API limits socket send/recv transfers to INT_MAX.
 * For nonblocking, stream sockets, we limit send/recv calls to that
 * size, since the sockets aren't guaranteed to send the full amount
 * requested.  For datagram sockets, we don't expect any transfers to
 * be larger than a few KB.
 * We do not handle blocking sockets that attempt to transfer more
 * than INT_MAX data at a time.
 */
static inline ssize_t
ofi_recv_socket(SOCKET fd, void *buf, size_t count, int flags)
{
	int len = count > INT_MAX ? INT_MAX : (int) count;
	return (ssize_t) recv(fd, (char *) buf, len, flags);
}

static inline ssize_t
ofi_send_socket(SOCKET fd, const void *buf, size_t count, int flags)
{
	int len = count > INT_MAX ? INT_MAX : (int) count;
	return (ssize_t) send(fd, (const char*) buf, len, flags);
}

static inline ssize_t ofi_read_socket(SOCKET fd, void *buf, size_t count)
{
	return ofi_recv_socket(fd, buf, count, 0);
}

static inline ssize_t ofi_write_socket(SOCKET fd, const void *buf, size_t count)
{
	return ofi_send_socket(fd, buf, count, 0);
}

static inline ssize_t
ofi_recvfrom_socket(SOCKET fd, void *buf, size_t count, int flags,
		    struct sockaddr *from, socklen_t *fromlen)
{
	int len = count > INT_MAX ? INT_MAX : (int) count;
	return recvfrom(fd, (char*) buf, len, flags, from, (int *) fromlen);
}

static inline ssize_t
ofi_sendto_socket(SOCKET fd, const void *buf, size_t count, int flags,
		  const struct sockaddr *to, socklen_t tolen)
{
	int len = count > INT_MAX ? INT_MAX : (int) count;
	return sendto(fd, (const char*) buf, len, flags, to, (int) tolen);
}

ssize_t ofi_writev_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt);
ssize_t ofi_readv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt);
ssize_t ofi_sendmsg_tcp(SOCKET fd, const struct msghdr *msg, int flags);
ssize_t ofi_recvmsg_tcp(SOCKET fd, struct msghdr *msg, int flags);

static inline ssize_t
ofi_sendmsg_udp(SOCKET fd, const struct msghdr *msg, int flags)
{
	DWORD bytes;
	int ret;

	ret = WSASendMsg(fd, msg, flags, &bytes, NULL, NULL);
	return ret ? ret : bytes;
}

ssize_t ofi_recvmsg_udp(SOCKET fd, struct msghdr *msg, int flags);

static inline int ofi_shutdown(SOCKET socket, int how)
{
	return shutdown(socket, how);
}

static inline int ofi_close_socket(SOCKET socket)
{
	return closesocket(socket);
}

static inline int fi_fd_nonblock(SOCKET fd)
{
	u_long argp = 1;
	return ioctlsocket(fd, FIONBIO, &argp) ? -WSAGetLastError() : 0;
}

static inline int fi_fd_block(SOCKET fd)
{
	u_long argp = 0;
	return ioctlsocket(fd, FIONBIO, &argp) ? -WSAGetLastError() : 0;
}

/* Note: Use static variable `errno` for libc routines
 * (such as fopen, lseek and etc)
 * If you need to define which function/variable is needed
 * to get correct `errno`, cosult with MSDN pages */
/*
 * Use only for OFI wrappers that use Windows Socket API (WSA):
 * Socket routines, poll and etc
 */
static inline int ofi_sockerr(void)
{
	return winerr2bsderr(WSAGetLastError());
}

/*
 * Use only for OFI wrappers that use Windows API (WinAPI):
 * SHM routines, epoll and etc
 */
static inline int ofi_syserr(void)
{
	return winerr2bsderr(GetLastError());
}

static inline int fi_wait_cond(pthread_cond_t *cond, pthread_mutex_t *mut, int timeout_ms)
{
	return !SleepConditionVariableCS(cond, mut, (DWORD)timeout_ms);
}

int ofi_shm_map(struct util_shm *shm, const char *name, size_t size,
				int readonly, void **mapped);

static inline int ofi_shm_remap(struct util_shm *shm, size_t newsize, void **mapped)
{
	OFI_UNUSED(shm);
	OFI_UNUSED(newsize);
	OFI_UNUSED(mapped);

	return -FI_ENOENT;
}

static inline char * strndup(char const *src, size_t n)
{
	size_t len = strnlen(src, n);
	char *dst = (char *)malloc(len + 1);

	if (dst) {
		memcpy(dst, src, len);
		dst[len] = 0;
	}
	return dst;
}

char *strcasestr(const char *haystack, const char *needle);

#ifndef _SC_PAGESIZE
#define _SC_PAGESIZE	0
#endif

#ifndef _SC_NPROCESSORS_ONLN
#define _SC_NPROCESSORS_ONLN 1
#endif

#ifndef _SC_PHYS_PAGES
#define _SC_PHYS_PAGES 2
#endif

static inline long ofi_sysconf(int name)
{
	SYSTEM_INFO si;
	ULONGLONG mem_size = 0;

	GetSystemInfo(&si);

	switch (name) {
	case _SC_PAGESIZE:
		return si.dwPageSize;
	case _SC_NPROCESSORS_ONLN:
		return si.dwNumberOfProcessors;
	case _SC_PHYS_PAGES:
		GetPhysicallyInstalledSystemMemory(&mem_size);
		return mem_size / si.dwPageSize;
	default:
		errno = EINVAL;
		return -1;
	}
}

int ofi_shm_unmap(struct util_shm *shm);

static inline ssize_t ofi_get_hugepage_size(void)
{
	return -FI_ENOSYS;
}

static inline int ofi_alloc_hugepage_buf(void **memptr, size_t size)
{
	return -FI_ENOSYS;
}

static inline int ofi_free_hugepage_buf(void *memptr, size_t size)
{
	return -FI_ENOSYS;
}

static inline int ofi_hugepage_enabled(void)
{
	return 0;
}

static inline int ofi_is_loopback_addr(struct sockaddr *addr) {
	return (addr->sa_family == AF_INET &&
		((struct sockaddr_in *)addr)->sin_addr.s_addr == htonl(INADDR_LOOPBACK)) ||
		(addr->sa_family == AF_INET6 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[0] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[1] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[2] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[3] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[4] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[5] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[6] == 0 &&
		((struct sockaddr_in6 *)addr)->sin6_addr.u.Word[7] == htons(1));
}

size_t ofi_ifaddr_get_speed(struct ifaddrs *ifa);

#define file2unix_time	10000000i64
#define win2unix_epoch	116444736000000000i64
#define CLOCK_MONOTONIC 1

/* Own implementation of clock_gettime*/
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

/* complex operations implementation */

#define OFI_DEF_COMPLEX(type)					\
typedef struct {						\
	type real;						\
	type imag;						\
} ofi_complex_## type;						\
static inline int ofi_complex_eq_## type			\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	return a.real == b.real && a.imag == b.imag;		\
}								\
static inline ofi_complex_## type ofi_complex_sum_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	ofi_complex_## type res;				\
	res.real = a.real + b.real;				\
	res.imag = a.imag + b.imag;				\
	return res;						\
}								\
static inline ofi_complex_## type ofi_complex_prod_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	ofi_complex_## type res;				\
	res.real = a.real * b.real - a.imag * b.imag;		\
	res.imag = a.real * b.imag + a.imag * b.real;		\
	return res;						\
}								\
static inline ofi_complex_## type ofi_complex_land_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	ofi_complex_## type res;				\
	res.real = (type)(((a.real != 0) || (a.imag != 0)) &&	\
		((b.real != 0) || (b.imag != 0)));		\
	res.imag = 0;						\
	return res;						\
}								\
static inline ofi_complex_## type ofi_complex_lor_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	ofi_complex_## type res;				\
	res.real = (type)(((a.real != 0) || (a.imag != 0)) &&	\
		((b.real != 0) || (b.imag != 0)));		\
	res.imag = 0;						\
	return res;						\
}								\
static inline ofi_complex_## type ofi_complex_lxor_## type	\
	(ofi_complex_## type a, ofi_complex_## type b)		\
{								\
	ofi_complex_## type res;				\
	res.real = (type)((((a.real != 0) || (a.imag != 0)) &&	\
		    !((b.real != 0) || (b.imag != 0))) ||	\
		   (!((a.real != 0) || (a.imag != 0)) &&	\
		    ((b.real != 0) || (b.imag != 0))));		\
	res.imag = 0;						\
	return res;						\
}

OFI_DEF_COMPLEX(float)
OFI_DEF_COMPLEX(double)
OFI_DEF_COMPLEX(long_double)


/* atomics primitives */
#ifdef HAVE_BUILTIN_ATOMICS
#define InterlockedAdd32 InterlockedAdd
#define InterlockedCompareExchange32 InterlockedCompareExchange
typedef LONG ofi_atomic_int_32_t;
typedef LONGLONG ofi_atomic_int_64_t;

#define ofi_atomic_add_and_fetch(radix, ptr, val) InterlockedAdd##radix((ofi_atomic_int_##radix##_t *)(ptr), (ofi_atomic_int_##radix##_t)(val))
#define ofi_atomic_sub_and_fetch(radix, ptr, val) InterlockedAdd##radix((ofi_atomic_int_##radix##_t *)(ptr), -(ofi_atomic_int_##radix##_t)(val))
#define ofi_atomic_cas_bool(radix, ptr, expected, desired)					\
	(InterlockedCompareExchange##radix(ptr, desired, expected) == expected)

#endif /* HAVE_BUILTIN_ATOMICS */

static inline int ofi_set_thread_affinity(const char *s)
{
	OFI_UNUSED(s);
	return -FI_ENOSYS;
}


#if defined(_M_X64) || defined(_M_AMD64)

#include <intrin.h>

static inline void
ofi_cpuid(unsigned func, unsigned subfunc, unsigned cpuinfo[4])
{
	__cpuidex(cpuinfo, func, subfunc);
}

#define ofi_clwb(addr) do { _mm_clflush(addr); _mm_sfence(); } while (0)
#define ofi_clflushopt(addr) do { _mm_clflush(addr); _mm_sfence(); } while (0)
#define ofi_clflush(addr) _mm_clflush(addr)
#define ofi_sfence() _mm_sfence()

#else /* defined(_M_X64) || defined(_M_AMD64) */

#define ofi_cpuid(func, subfunc, cpuinfo)
#define ofi_clwb(addr)
#define ofi_clflushopt(addr)
#define ofi_clflush(addr)
#define ofi_sfence()

#endif /* defined(_M_X64) || defined(_M_AMD64) */


#ifdef __cplusplus
}
#endif

#endif /* _FI_WIN_OSD_H_ */
