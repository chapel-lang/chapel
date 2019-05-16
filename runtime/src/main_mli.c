/*
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
#include "chpl_rt_utils_static.h"
#include "chpl-init.h"
#include "chplexit.h"
#include "config.h"

// Declare the server listen loop (to be linked in later).
void chpl_mli_smain(char* setupConn);

int main(int argc, char** argv) {
  // No need to pass in the connection information, which will be the last
  // two arguments (one for indicating its presence, one for its value).
  chpl_library_init(argc - 2, argv);

  // TODO: Pass argc/argv into listen loop?

  // Drop down into the server listen loop, passing in the last argument, which
  // will be the socket connection information for communicating where the
  // server will be listening.
  chpl_mli_smain(argv[argc-1]);

  //
  // This will be called when the server exits the listen loop, which will
  // only happen when the client calls its version of finalize.
  //
  chpl_library_finalize();

  // Should never reach here.
  return 0;
}
