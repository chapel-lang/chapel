/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _SYS_H_
#define _SYS_H_

#include "sys_basic.h"
#include "qio_error.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#ifndef __CYGWIN__
#include <netinet/tcp.h>
#endif
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// This function just returns errno (needed for LLVM compiles)
static inline int chpl_macro_int_errno(void) { return errno; }

#ifndef LUSTRE_SUPER_MAGIC
// Magic value to be found in the statfs man page
#define LUSTRE_SUPER_MAGIC     0x0BD00BD0
#endif

// TAKZ - In the case where we are unable to include statfs or fstatfs, we need to
// have a struct defined. As well, the Mac and linux statfs structs differ on what
// different field names represent, and this way we have a uniform struct across all
// OSs. Besides this fact, on apple, depending upon whether or not
// _DARWIN_FEATURE_64_BIT_INODE is defined, we get different types for the same
// members of the statfs struct (!!).
typedef struct sys_statfs_s {
  uint64_t    f_type;     /* type of filesystem */
  int64_t     f_bsize;    /* optimal transfer block size */
  uint64_t    f_blocks;   /* total data blocks in file system */
  uint64_t    f_bfree;    /* free blocks in fs */
  uint64_t    f_bavail;   /* free blocks avail to non-superuser */
  uint64_t    f_files;    /* total file nodes in file system */
  uint64_t    f_ffree;    /* free file nodes in fs */
  uint64_t    f_namelen;  /* maximum length of filenames */
} sys_statfs_t;

typedef struct sys_stat_s {
  dev_t       st_dev;         /* Device ID of device containing file */
  ino_t       st_ino;         /* File serial number. */
  mode_t      st_mode;        /* Mode of file (see below). */
  nlink_t     st_nlink;       /* Number of hard links to the file. */
  uid_t       st_uid;         /* User ID of file. */
  gid_t       st_gid;         /* Group ID of file.  */
  dev_t       st_rdev;        /* Device ID (if file is character or block special). */
  off_t       st_size;        /* File Size in bytes */
  struct timespec    st_atim; /* Last data access timestamp.  */
  struct timespec    st_mtim; /* Last data modification timestamp. */
  struct timespec    st_ctim; /* Last file status change timestamp. */
  // blksize_t   st_blksize;     /* A file system-specific preferred I/O block size for this object. */
  // blkcnt_t    st_blocks;      /* Number of blocks allocated for this object. */
} sys_stat_t;

void stat_to_sys_stat(const char* path, sys_stat_t* out_buf, struct stat* in_buf);

typedef int fd_t;

// Do we have getaddrinfo?
#ifdef EAI_NONAME
#define HAS_GETADDRINFO
#endif


typedef struct sockaddr_storage sys_sockaddr_storage_t;


typedef struct sys_sockaddr_s {
  sys_sockaddr_storage_t addr;
  socklen_t len;
} sys_sockaddr_t;

#ifdef HAS_GETADDRINFO
typedef struct addrinfo sys_addrinfo_t;
typedef struct addrinfo* sys_addrinfo_ptr_t;
#endif

//
//typedef struct sys_addrinfo_s //{
//  struct addrinfo addr_info;
//} sys_addrinfo_t;

/* Wrap system calls to return error separately,
 * to run them in a pthread, and to use a fixed-length sys_sockaddr
 * to simplify programming.
 *
 * A qthread blocked on a system call will make no useful progress,
 * and indeed that might lead to deadlock. Therefore, all system calls
 * that could block are decorated with
 * STARTING_SLOW_SYSCALL and
 * DONE_SLOW_SYSCALL
 *
 * The I/O library might also want to move I/O operations that do
 * not block (just might take a while) to other pthreads. In order to do
 * that, it uses STARTING_SLOW_SYSCALL and DONE_SLOW_SYSCALL,
 * possibly redundantly, around the call to sys_something.
 */

// TODO -- define these once we have appropriate qthreads integration.
// These need to handle being run when the thread is already on 
// a system-call running pthread.
#define STARTING_SLOW_SYSCALL { }
#define DONE_SLOW_SYSCALL { }

void sys_init_sys_sockaddr(sys_sockaddr_t* addr);

size_t sys_page_size(void);


err_t sys_fseeko(FILE* stream, off_t offset, int whence);
err_t sys_ftello(FILE* stream, off_t* offset_out);
err_t sys_fopen(const char* path, const char* mode, FILE** file_out);
err_t sys_fdopen(fd_t fd, const char* mode, FILE** file_out);
err_t sys_fclose(FILE* stream);
// like sys_fread, but returns EEOF on eof.
err_t sys_fread(void* ptr, size_t size, size_t nmemb, FILE* stream, size_t* num_read);
err_t sys_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream, size_t* num_written);
err_t sys_fflush(FILE* stream);
err_t sys_feof(FILE* stream, int *iseof);
err_t sys_ferror(FILE* stream);
err_t sys_posix_fadvise(fd_t fd, off_t offset, off_t len, int advice);
err_t sys_posix_madvise(void* addr, size_t len, int advice);

// returns an allocated string in string_out, which must be freed.
err_t sys_strerror(err_t error, const char** string_out);

const char* sys_strerror_syserr_str(syserr error, err_t* err_in_strerror);

// returns an allocated string in string_out, which must be freed.
err_t sys_readlink(const char* path, const char** string_out);
int sys_getenv(const char* name, const char** string_out);

err_t sys_open(const char* path, int flags, mode_t mode, fd_t* fd_out);


err_t sys_close(fd_t fd);

err_t sys_lseek(fd_t fd, off_t offset, int whence, off_t* offset_out);
err_t sys_stat(const char* path, sys_stat_t* buf);
err_t sys_fstat(fd_t fd, struct stat* buf);
err_t sys_lstat(const char* path, struct stat* buf);
err_t sys_fstatfs(fd_t fd, sys_statfs_t* buf);

#ifdef SYS_HAS_LLAPI
err_t sys_lustre_get_stripe_size(fd_t fd, int64_t* size_out);
#endif

err_t sys_mkstemp(char* template_, fd_t* fd_out);

err_t sys_ftruncate(fd_t fd, off_t length);

err_t sys_sysconf(int name, long* val_out);

err_t sys_posix_fallocate(fd_t fd, off_t offset, off_t len);

int64_t sys_iov_total_bytes(const struct iovec* iov, int iovcnt);
err_t sys_mmap(void* addr, size_t length, int prot, int flags, fd_t fd, off_t offset, void** ret);

err_t sys_munmap(void* addr, size_t length);

err_t sys_read(fd_t fd, void* buf, size_t count, ssize_t* num_read_out);
err_t sys_write(fd_t fd, const void* buf, size_t count, ssize_t* num_written_out);

err_t sys_pread(fd_t fd, void* buf, size_t count, off_t offset, ssize_t* num_read_out);
err_t sys_pwrite(fd_t fd, const void* buf, size_t count, off_t offset, ssize_t* num_written_out);

err_t sys_readv(fd_t fd, const struct iovec* iov, int iovcnt, ssize_t* num_read_out);
err_t sys_writev(fd_t fd, const struct iovec* iov, int iovcnt, ssize_t* num_written_out);

err_t sys_preadv(fd_t fd, const struct iovec* iov, int iovcnt, off_t seek_to_offset, ssize_t* num_read_out);
err_t sys_pwritev(fd_t fd, const struct iovec* iov, int iovcnt, off_t seek_to_offset, ssize_t* num_written_out);

err_t sys_fsync(fd_t fd);

err_t sys_fcntl(fd_t fd, int cmd, int* ret);

err_t sys_fcntl_long(fd_t fd, int cmd, long arg, int* ret);


err_t sys_fcntl_ptr(fd_t fd, int cmd, void* arg, int* ret);


err_t sys_dup(fd_t oldfd, fd_t* fd_out);


err_t sys_dup2(fd_t oldfd, fd_t newfd, fd_t* fd_out);


err_t sys_pipe(fd_t* read_fd_out, fd_t* write_fd_out);


err_t sys_accept(fd_t sockfd, sys_sockaddr_t* addr_out, int* fd_out);


err_t sys_bind(fd_t sockfd, const sys_sockaddr_t* addr);


err_t sys_connect(fd_t sockfd, const sys_sockaddr_t* addr);


#ifdef HAS_GETADDRINFO
/* See comment about this being commented out in sys.c -BLC */
//err_t sys_getaddrinfo(const char* node, const char* service, 
//                     const struct addrinfo* hints, struct addrinfo ** res);


int sys_getaddrinfo_flags(sys_addrinfo_ptr_t a);
int sys_getaddrinfo_family(sys_addrinfo_ptr_t a);
int sys_getaddrinfo_socktype(sys_addrinfo_ptr_t a);
int sys_getaddrinfo_protocol(sys_addrinfo_ptr_t a);
sys_sockaddr_t sys_getaddrinfo_addr(sys_addrinfo_ptr_t a);
sys_addrinfo_ptr_t sys_getaddrinfo_next(sys_addrinfo_ptr_t a);

void sys_freeaddr_info(sys_addrinfo_ptr_t* p);


err_t sys_getnameinfo(const sys_sockaddr_t* addr, char** host_out, char** serv_out, int flags);
#endif

err_t sys_getpeername(fd_t sockfd, sys_sockaddr_t* addr);


err_t sys_getsockname(fd_t sockfd, sys_sockaddr_t* addr);


err_t sys_getsockopt(fd_t sockfd, int level, int optname, void* optval, socklen_t* optlen);

err_t sys_listen(fd_t sockfd, int backlog);


err_t sys_recv(fd_t sockfd, void* buf, size_t len, int flags, ssize_t* num_recvd_out);


err_t sys_recvfrom(fd_t sockfd, void* buf, size_t len, int flags, sys_sockaddr_t* src_addr_out, ssize_t* num_recvd_out);


err_t sys_recvmsg(fd_t sockfd, struct msghdr *msg, int flags, ssize_t* num_recvd_out);

err_t sys_send(fd_t sockfd, const void* buf, int64_t len, int flags, ssize_t* num_sent_out);


err_t sys_sendto(fd_t sockfd, const void* buf, int64_t len, int flags, const sys_sockaddr_t* dest_addr, ssize_t* num_sent_out);


err_t sys_sendmsg(fd_t sockfd, const struct msghdr *msg, int flags, ssize_t* num_sent_out);


err_t sys_setsockopt(fd_t sockfd, int level, int optname, void* optval, socklen_t optlen);


err_t sys_shutdown(fd_t sockfd, int how);


err_t sys_socket(int domain, int type, int protocol, fd_t* fd_out);


err_t sys_socketpair(int domain, int type, int protocol, fd_t* fd_out_a, fd_t* fd_out_b);

err_t sys_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout, int* nset);

static inline void sys_fd_clr(int fd, fd_set* set) {
  FD_CLR(fd, set);
}
static inline int sys_fd_isset(int fd, fd_set* set) {
  return FD_ISSET(fd, set);
}
static inline void sys_fd_set(int fd, fd_set* set) {
  FD_SET(fd, set);
}
static inline void sys_fd_zero(fd_set* set) {
  FD_ZERO(set);
}


err_t sys_unlink(const char* path);

// Allocates a string to store the current directory which must be freed.
err_t sys_getcwd(const char** path_out);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
