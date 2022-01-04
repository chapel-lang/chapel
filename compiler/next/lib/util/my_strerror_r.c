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

// make sure to get the XSI/POSIX.1-2001 strerror_r
#define _POSIX_C_SOURCE 200112L
#undef _GNU_SOURCE

#include <string.h>

#include "my_strerror_r.h"

int my_strerror_r(int errnum, char* strerrbuf, size_t buflen) {
  return strerror_r(errnum, strerrbuf, buflen);
}
