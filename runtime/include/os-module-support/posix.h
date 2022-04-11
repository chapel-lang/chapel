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

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
