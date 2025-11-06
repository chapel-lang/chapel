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

#ifndef _chpl_linefile_defs_h_
#define _chpl_linefile_defs_h_

#ifdef __cplusplus
extern "C" {
#endif

enum chpl_file_idx_t {
  CHPL_FILE_IDX_INTERNAL = 1, // <internal>
  CHPL_FILE_IDX_COMMAND_LINE = -1,     // (command-line)
  CHPL_FILE_IDX_COMMAND_LINE_ARG = -2, // <command-line arg>
  CHPL_FILE_IDX_FORK_LARGE = -3,       // "fork large"
  CHPL_FILE_IDX_MAIN_PROGRAM = -4,     // "main program"
  CHPL_FILE_IDX_UNKNOWN = -5,          // "<unknown>"
  CHPL_FILE_IDX_IDLE_TASK = -6,        // "|idle|"
  CHPL_FILE_IDX_COMM_TASK = -7,        // "<comm thread>"
  CHPL_FILE_IDX_MAIN_TASK = -8,        // "<main task>"
  CHPL_FILE_IDX_ON_BODY_TASK = -9,     // "on-body task"
  CHPL_FILE_IDX_SAVED_FILENAME = -10,  // variable, see chpl-linefile-support.h
};

#ifdef __cplusplus
}
#endif

#endif // _chpl_linefile_defs_h_
