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

//
// Formatted output support.
//

#include "chplrt.h"

#include "chpl-format.h"

#include <inttypes.h>
#include <stdio.h>


static const size_t KiB = (size_t) (1UL << 10);
static const size_t MiB = (size_t) (1UL << 20);
static const size_t GiB = (size_t) (1UL << 30);


//
// snprintf() a size as nnn[KMG]
//

int chpl_snprintf_KMG_z(char* buf, int bufSize, size_t val) {
  if (val >= GiB)
    return snprintf(buf, bufSize, "%zdG", val / GiB);
  else if (val >= MiB)
    return snprintf(buf, bufSize, "%zdM", val / MiB);
  else if (val >= KiB)
    return snprintf(buf, bufSize, "%zdK", val / KiB);
  return snprintf(buf, bufSize, "%zd", val);
}


int chpl_snprintf_KMG_f(char* buf, int bufSize, double val) {
  if (val >= GiB)
    return snprintf(buf, bufSize, "%.1fG", val / GiB);
  else if (val >= MiB)
    return snprintf(buf, bufSize, "%.1fM", val / MiB);
  else if (val >= KiB)
    return snprintf(buf, bufSize, "%.1fK", val / KiB);
  return snprintf(buf, bufSize, "%.1f", val);
}
