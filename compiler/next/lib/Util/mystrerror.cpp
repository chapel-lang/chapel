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
#include "./sys_basic.h"

#include "mystrerror.h"

#include <string>

#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace chpl {

std::string my_strerror(int errno_) {
  char errbuf[256];
  int rc;
  errbuf[0] = '\0';
  rc = strerror_r(errno_, errbuf, sizeof(errbuf));
  if (rc != 0)
    strncpy(errbuf, "<unknown error>", sizeof(errbuf));
  return std::string(errbuf);
}

} // end namespace chpl
