/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#define CHPL_FILE_IDX_INTERNAL 1 // <internal>

#define CHPL_FILE_IDX_COMMAND_LINE -1     // (command-line)
#define CHPL_FILE_IDX_COMMAND_LINE_ARG -2 // <command-line arg>
#define CHPL_FILE_IDX_FORK_LARGE -3       // "fork large"
#define CHPL_FILE_IDX_MAIN_PROGRAM -4     // "main program"
#define CHPL_FILE_IDX_UNKNOWN -5          // "<unknown>"
#define CHPL_FILE_IDX_IDLE_TASK -6        // "|idle|"
#define CHPL_FILE_IDX_COMM_TASK -7        // "<comm thread>"
#define CHPL_FILE_IDX_MAIN_TASK -8        // "<main task>"
#define CHPL_FILE_IDX_ON_BODY_TASK -9     // "on-body task"
#define CHPL_FILE_IDX_SAVED_FILENAME -10  // variable, see below

// chpl_saveFilename stores the passed in char*, subsequent calls to
// chpl_lookupFilename(CHPL_FILE_IDX_SAVED_FILENAME) will return that stored
// pointer. CHPL_FILE_IDX_SAVED_FILENAME may only be used while the original
// filename pointer is still valid.  This was added to support reporting errors
// in a user-supplied file for config vars.
void chpl_saveFilename(const char *filename);

c_string chpl_lookupFilename(const int32_t idx);

#endif // _chpl_linefile_support_h_
