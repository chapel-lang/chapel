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
 This module contains built-in functions that are included in every Chapel
 program by default.

*/
module Builtins {

  /*
   Terminates the application.

   :arg status: The exit status
  */
  pragma "function terminates program"
  inline proc exit(status: int=0) {
    __primitive("chpl_exit_any", status);
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
}
