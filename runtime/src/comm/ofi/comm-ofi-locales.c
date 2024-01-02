/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
#include "arg.h"
#include "chpl-comm-locales.h"
#include "error.h"

#ifndef LAUNCHER
#include "chpl-comm.h"
#include "comm-ofi-internal.h"
#endif

int64_t chpl_comm_default_num_locales(void) {
  return chpl_specify_locales_error();
}


void chpl_comm_verify_num_locales(int64_t proposedNumLocales) {
#ifndef LAUNCHER
  if (proposedNumLocales != chpl_numNodes) {
    char msg[100];
    snprintf(msg, sizeof(msg),
             "Number of locales from command-line doesn't match "
             "number from OOB %s (%" PRId64 " != %d)",
             chpl_comm_oob, proposedNumLocales, chpl_numNodes);
    chpl_error(msg,0,0);
  }
#endif
}


void chpl_comm_verify_supports_colocales(int64_t numColocales) { }

