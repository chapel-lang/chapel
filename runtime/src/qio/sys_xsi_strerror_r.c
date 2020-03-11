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

/*
 * Since GNU environments like to define their own
 * non-portable strerror_r, we need a separate
 * source file that does not define _GNU_SOURCE
 * in order to get the portable one.
 */

// squelch warning on Mac OS X
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 600
#undef _GNU_SOURCE

#include <string.h>
#include <errno.h>

#define HAS_STRERROR_R

int sys_xsi_strerror_r(int errnum, char* buf, size_t buflen);


int sys_xsi_strerror_r(int errnum, char* buf, size_t buflen)
{
#ifdef HAS_STRERROR_R
  return strerror_r(errnum, buf, buflen);
#else
  char* msg = strerror(errnum);
  if( strlen(msg) + 1 < buflen ) {
   strcpy(buf, msg);
   return 0;
  }
  return ERANGE;
#endif
}

#if 0
// Squelch the warning about not including the chplrt; not necessary here
#include "chplrt.h"
#endif

