/*
 * Copyright 2004-2018 Cray Inc.
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
#include "chpl-comm-locales.h"
#include "error.h"

int64_t chpl_comm_default_num_locales(void) {
  return 1;
}

void chpl_comm_verify_num_locales(int64_t proposedNumLocales) {
  if (proposedNumLocales != 1) {
    chpl_error("Only 1 locale may be used for CHPL_COMM layer 'none'\n"
               "To use multiple locales, see $CHPL_HOME/doc/rst/usingchapel/multilocale.rst",
               0, 0);
  }
}
