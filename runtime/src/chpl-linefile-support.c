/*
 * Copyright 2004-2015 Cray Inc.
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
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "chplrt.h"
#include "chplcgfns.h"

#include "chpl-linefile-support.h"

// chpl_filenameTable is defined in chplcgfns.h

static int compare(const void *a, const void *b) {
  return strcmp((c_string)a, *(c_string *)b);
}

uint64_t chpl_findFilenameIdx(c_string name) {
  c_string *result;

  if (name == NULL) {
    // We will occasionally get a NULL passed in as the filename, just use
    // <internal> instead in that case
    name = "<internal>";
  }

  // binary search chpl_filenameTable for our answer, we will output this table
  // in sorted order with all possible values so result will never be NULL
  result = (c_string *)bsearch(name, chpl_filenameTable,
                               (size_t)chpl_filenameTableSize, sizeof(c_string),
                               compare);
  assert(result != NULL);
  return (uint64_t)(result - chpl_filenameTable);
}

c_string chpl_lookupFilename(const uint64_t idx) {
  assert(idx < chpl_filenameTableSize);

  return chpl_filenameTable[idx];
}

