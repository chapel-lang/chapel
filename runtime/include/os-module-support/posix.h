/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _OS_POSIX_SUPPORT_H_
#define _OS_POSIX_SUPPORT_H_

//
// Ensure all the POSIX .h files the OS.POSIX module depends upon are
// actually present.  Some of these are needed by other parts of the
// runtime and so are already included elsewhere, but here we make
// sure we have them.
//
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// errno.h
//
static inline
int chpl_os_posix_errno_val(void) {
  return errno;
}


//
// fcntl.h
//

// This is needed because POSIX declares: open(const char*, int, ...)
// and Chapel varargs functions are actually generic, thus mismatch.
static inline
int chpl_os_posix_open(const char* path, int oflag, mode_t mode) {
  return open(path, oflag, mode);
}

//
// sys/select.h
//
static inline
void chpl_os_posix_FD_CLR(int fd, fd_set* fdset) {
  FD_CLR(fd, fdset);
}

static inline
int chpl_os_posix_FD_ISSET(int fd, fd_set* fdset) {
  return FD_ISSET(fd, fdset);
}

static inline
void chpl_os_posix_FD_SET(int fd, fd_set* fdset) {
  FD_SET(fd, fdset);
}

static inline
void chpl_os_posix_FD_ZERO(fd_set* fdset) {
  FD_ZERO(fdset);
}


//
// sys/stat.h
//

// The POSIX spec and Darwin disagree on the names of the timestamp
// members with type struct timespec. Here we define a common type
// the module code can use, and a stat(3p) shim that converts from
// the OS's struct and our common ("POSIX") one.
struct chpl_os_posix_struct_stat {
  dev_t st_dev;
  ino_t st_ino;
  mode_t st_mode;
  nlink_t st_nlink;
  uid_t st_uid;
  gid_t st_gid;
  dev_t st_rdev;
  off_t st_size;
  struct timespec st_atim;
  struct timespec st_mtim;
  struct timespec st_ctim;
  blksize_t st_blksize;
  blkcnt_t st_blocks;
};

static inline
int chpl_os_posix_stat(const char *restrict path,
                       struct chpl_os_posix_struct_stat *restrict buf) {
  struct stat myBuf;
  int ret = stat(path, &myBuf);
  buf->st_dev = myBuf.st_dev;
  buf->st_ino = myBuf.st_ino;
  buf->st_mode = myBuf.st_mode;
  buf->st_nlink = myBuf.st_nlink;
  buf->st_uid = myBuf.st_uid;
  buf->st_gid = myBuf.st_gid;
  buf->st_rdev = myBuf.st_rdev;
  buf->st_size = myBuf.st_size;
#if defined(__linux__)
  buf->st_atim = myBuf.st_atim;
  buf->st_mtim = myBuf.st_mtim;
  buf->st_ctim = myBuf.st_ctim;
#elif defined(__APPLE__)
  buf->st_atim = myBuf.st_atimespec;
  buf->st_mtim = myBuf.st_mtimespec;
  buf->st_ctim = myBuf.st_ctimespec;
#else
#error Unsupported OS.
#endif
  buf->st_blksize = myBuf.st_blksize;
  buf->st_blocks = myBuf.st_blocks;
  return ret;
}

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
