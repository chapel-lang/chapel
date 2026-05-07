/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
#include "chpl-prginfo.h"

// Defined in our module code. We will link against it.
extern chpl_rt_prginfo* chpl_prepareProgramInfoHere(void);

int main(int argc, char* argv[]) {
  // Run the code that initializes our program information.
  chpl_rt_prginfo* our_root_prg = chpl_prepareProgramInfoHere();
  chpl_rt_prginfo* got_root_prg = NULL;

  // Initialize the runtime, giving it our program info.
  chpl_rt_init(our_root_prg, argc, argv);

  // Have the runtime fetch the root program.
  got_root_prg = CHPL_RT_PRGINFO_ROOT;

  // It should match what we gave 'chpl_rt_init'.
  if (got_root_prg && our_root_prg != got_root_prg) {
    chpl_internal_error("a Chapel program tried to initialize the Chapel "
                        "runtime, but the root program was already bound");

  } else if (!got_root_prg) {
    chpl_internal_error("the Chapel runtime has not bound a root program");
  }

  // Run the main function for this node.
  chpl_task_callMain(chpl_executable_init);

  // have everyone exit, returning the value returned by the user written main
  // or 0 if it didn't return anything
  chpl_exit_all(chpl_gen_main_arg.return_value);

  return 0; // should never get here
}
