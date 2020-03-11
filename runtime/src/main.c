/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

int main(int argc, char* argv[]) {

  // Initialize the runtime
  chpl_rt_init(argc, argv);

  // Run the main function for this node.
  chpl_task_callMain(chpl_executable_init); 

  // have everyone exit, returning the value returned by the user written main
  // or 0 if it didn't return anything
  chpl_rt_finalize(chpl_gen_main_arg.return_value);

  return 0; // should never get here
}
