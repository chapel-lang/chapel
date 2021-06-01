/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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


#include <stddef.h>
#include <stdlib.h>

#include "my_aligned_alloc.h" // requires size_t already defined

void *my_aligned_alloc(size_t alignment, size_t size) {
  // This is a workaround for aligned_alloc() seemingly not being
  // available on Mac OS X < 10.15, despite it claiming to support
  // C11.
#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 101500
  void* retval = NULL;
  int status = posix_memalign(&retval, alignment, size);
  if (status != 0) {
    retval = NULL;
  }
  return retval;
#else
  return aligned_alloc(alignment, size);
#endif
}
