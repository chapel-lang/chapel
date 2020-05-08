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
  This module contains built-in functions.

  .. note:: All Chapel programs automatically ``use`` this module by default.
            An explicit ``use`` statement is not necessary.
*/
module Builtins {

  /*
    Assert that a boolean condition is true.  If it is false, prints
    'assert failed' and halts the program.

    .. note :: In the current implementation, this assert never becomes a no-op.
               That is, using it will always incur execution-time checks.

    :arg test: the boolean condition
    :type test: `bool`
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc assert(test: bool) {
    if !test then
      __primitive("chpl_error", c"assert failed");
  }
    

  /*
    Assert that a boolean condition is true.  If it is false, prints
    'assert failed - ' followed by all subsequent arguments, as though
    printed using :proc:`~ChapelIO.write()`.

    .. note :: In the current implementation, this assert never becomes a no-op.
               That is, using it will always incur execution-time checks.

    :arg test: the boolean condition
    :type test: `bool`

    :arg args: other arguments to print
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc assert(test: bool, args ...?numArgs) {
    use IO only stringify;
    if !test {
      var tmpstring = "assert failed - " + stringify((...args));
      __primitive("chpl_error", tmpstring.c_str());
    }
  }

  //
  // compile-time diagnostics
  //
  // Note: the message printed by "error" and "warning" primitives
  // consists of the formals of the enclosing function, not their own args.
  //

  /*
     Generate a compile-time error.
     The error text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerError(param msg: string ...?n, param errorDepth: int) {
    __primitive("error");
  }

  /*
     Generate a compile-time error.
     The error text is a concatenation of the arguments.
  */
  proc compilerError(param msg: string ...?n) {
    __primitive("error");
  }

  /*
     Generate a compile-time warning.
     The warning text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerWarning(param msg: string ...?n, param errorDepth: int) {
    __primitive("warning");
  }

  /*
     Generate a compile-time warning.
     The warning text is a concatenation of the arguments.
  */
  proc compilerWarning(param msg: string ...?n) {
    __primitive("warning");
  }

  /* 
     Generate a compile-time error if the `test` argument is false.
  */
  proc compilerAssert(param test: bool) { 
    if !test then compilerError("assert failed");
  }

  /* 
     Generate a compile-time error if the `test` argument is false.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerAssert(param test: bool, param errorDepth: int) {
    if !test then compilerError("assert failed", errorDepth + 1);
  }

  /* 
     Generate a compile-time error if the `test` argument is false.
     The warning text is a concatenation of the string arguments.
  */
  proc compilerAssert(param test: bool, param msg: string ...?n) {
    if !test then compilerError("assert failed - ", (...msg));
  }

  /* 
     Generate a compile-time error if the `test` argument is false.
     The warning text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerAssert(param test: bool, param msg: string ...?n,
                      param errorDepth: int) {
    if !test then compilerError("assert failed - ", (...msg), errorDepth + 1);
  }

  //
  // end compile-time diagnostics
  //

  /*
    Exit the program

    :arg status: The exit code for the program
  */
  pragma "function terminates program"
  inline proc exit(status: int=0) {
    __primitive("chpl_exit_any", status);
  }
}
