/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_linefile_support_h_
#define _chpl_linefile_support_h_

#include "chpltypes.h"

#include "fileinfo/chpl-linefile-defs.h"

#ifdef __cplusplus
extern "C" {
#endif

// chpl_saveFilename stores the passed in char*, subsequent calls to
// chpl_lookupFilename(CHPL_FILE_IDX_SAVED_FILENAME) will return that stored
// pointer. CHPL_FILE_IDX_SAVED_FILENAME may only be used while the original
// filename pointer is still valid.  This was added to support reporting errors
// in a user-supplied file for config vars.
void chpl_saveFilename(const char *filename);

c_string chpl_lookupFilename(const int32_t idx);

#ifdef __cplusplus
}
#endif

#endif // _chpl_linefile_support_h_
