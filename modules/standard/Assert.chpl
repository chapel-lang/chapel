/*
 * Copyright 2004-2015 Cray Inc.
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

  .. note:: 
            In the current implementation, these asserts never become
            no-ops.  That is, using them will always incur
            execution-time checks.
*/
module Assert {


/*
  Assert that a boolean condition is true.  If it is false, prints
  'assert failed' and halts the program.

  :arg test: the boolean condition
  :type test: `bool`
*/
proc assert(test: bool) {
  if !test then
    __primitive("chpl_error", "assert failed");
}
  

/*
  Assert that a boolean condition is true.  If it is false, prints
  'assert failed - ' followed by all subsequent arguments, as though
  printed using :proc:`~IO.write()`.

  :arg test: the boolean condition
  :type test: `bool`

  :arg args: other arguments to print
*/
proc assert(test: bool, args ...?numArgs) {
  if !test {
    var tmpstring: string;
    tmpstring.write((...args));
    __primitive("chpl_error", "assert failed - " + tmpstring.c_str());
  }
}

}
