/*
 * Copyright 2004-2020 Cray Inc.
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
#include <inttypes.h>

#include "chplrt.h"
#include "chplcgfns.h"
#include "chpl-thread-local-storage.h"

#include "chpl-linefile-support.h"

static const char *savedFilename;

typedef char fileBuff[48];
CHPL_TLS_DECL(fileBuff, unknownFileBuffer);

// See note in chpl-linefile-support.h
void chpl_saveFilename(const char *filename) { savedFilename = filename; }

// chpl_filenameTable is defined in chplcgfns.h
c_string chpl_lookupFilename(const int32_t idx) {
  if (idx < 0) {
    switch (idx) {
    case CHPL_FILE_IDX_COMMAND_LINE:
      return "(command-line)";
    case CHPL_FILE_IDX_COMMAND_LINE_ARG:
      return "<command-line arg>";
    case CHPL_FILE_IDX_FORK_LARGE:
      return "fork large";
    case CHPL_FILE_IDX_MAIN_PROGRAM:
      return "main program";
    case CHPL_FILE_IDX_UNKNOWN:
      return "<unknown>";
    case CHPL_FILE_IDX_IDLE_TASK:
      return "|idle|";
    case CHPL_FILE_IDX_COMM_TASK:
      return "<comm task>";
    case CHPL_FILE_IDX_MAIN_TASK:
      return "<main task>";
    case CHPL_FILE_IDX_ON_BODY_TASK:
      return "on-body task";
    case CHPL_FILE_IDX_SAVED_FILENAME:
      return savedFilename;
    default: {
      snprintf(CHPL_TLS_GET(unknownFileBuffer), 48,
               "<unknown file idx %" PRId32 ">", idx);
      // There is a possibility of unknownFileBuffer changing before it is
      // used, not a very good one though.
      return CHPL_TLS_GET(unknownFileBuffer);
    }
    }
  } else {
    if (idx < chpl_filenameTableSize) {
      return chpl_filenameTable[idx];
    } else {
      snprintf(CHPL_TLS_GET(unknownFileBuffer), 48,
               "<unknown file idx %" PRId32 ">", idx);
      return CHPL_TLS_GET(unknownFileBuffer);
    }
  }
}

