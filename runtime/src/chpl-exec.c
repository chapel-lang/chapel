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

#include "chplrt.h"

#include "chpl-exec.h"
#include "chpl-linefile-support.h"
#include "error.h"

#include <sys/wait.h>

#define CHECK_ERROR(status, description, ignore_status) do { \
  if (status == -1) { \
    chpl_error("fork failed", 0, CHPL_FILE_IDX_COMMAND_LINE);\
  } else if (status != 0 && !ignore_status) { \
    chpl_error(description, 0, CHPL_FILE_IDX_COMMAND_LINE); \
  } \
} while(0)

int chpl_invoke_using_system(const char* command,
                             const char* description,
                             int ignore_status) {
  int status = system(command);
  CHECK_ERROR(status, description, ignore_status);
  return status;
}
