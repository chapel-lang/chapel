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

/* Provides a collection of useful debugging utilities.

The Debugger module currently only provides a single function,
:proc:`breakpoint`. This function can be used to set a breakpoint in the code
that will be hit when the program is run under a debugger.
*/
@unstable(category="experimental", reason="The Debugger module is unstable due to its experimental behavior")
module Debugger {
  /*
    Sets a breakpoint at this point in execution if compiled with `-g`.

    .. warning::
       If code uses :proc:`breakpoint` and is compiled with `-g`,
       the program will not be runnable outside of a debugger
  */
  inline proc breakpoint {
    __primitive("breakpoint");
  }
}
