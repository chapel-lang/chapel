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


/*
  Support for simple assert() routines.

  .. warning:: This module has been deprecated - please use :mod:`Builtins` instead.

  .. note:: All Chapel programs automatically ``use`` this module by default.
            An explicit ``use`` statement is not necessary.

  In the current implementation, these asserts never become no-ops.  That is,
  using them will always incur execution-time checks.

*/
module Assert {
  compilerWarning("Assert module is deprecated. 'assert' functions are now in ",
                  "the Builtins module, which is 'use'd by default");

/*
  Assert that a boolean condition is true.  If it is false, prints
  'assert failed' and halts the program.

  :arg test: the boolean condition
  :type test: `bool`
*/
pragma "insert line file info"
pragma "always propagate line file info"
pragma "last resort"
proc assert(test: bool) {
  // last resort avoids recursion
  assert(test);
}
  

/*
  Assert that a boolean condition is true.  If it is false, prints
  'assert failed - ' followed by all subsequent arguments, as though
  printed using :proc:`~ChapelIO.write()`.

  :arg test: the boolean condition
  :type test: `bool`

  :arg args: other arguments to print
*/
pragma "insert line file info"
pragma "always propagate line file info"
pragma "last resort"
proc assert(test: bool, args ...?numArgs) {
  // last resort avoids recursion
  assert(test, (...args));
}
}
