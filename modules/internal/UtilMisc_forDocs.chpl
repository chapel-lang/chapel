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

//
// NOAKES 2015/09/22
//
// This is a dummy chapel that is used to anchor some internal definitions
// during the last day before Doc Freeze for 1.12.  This file is processed
// for docs in modules/Makefile but is not part of the Chapel Module code
// base.
//
// One reason to create this file is that the Chapel compiler, and hence
// chpldoc, fails if it is asked to compile ChapelBase directly.  It is
// speculated that this is because ChapelBase auto-magically included as
// anchor module by the compiler.
//
// Brad imagines that we should be on a path that refactors ChapelBase.chpl


//
// ChapelMisc_forDocs.chpl
//

/*

Additional utilities

*/

module UtilMisc_forDocs {

  /*
     Generate a compile-time error.
     The error text is a concatenation of the arguments.
  */
  proc compilerError(param msg: string ...?n) {
  }

  /*
     Generate a compile-time error.
     The error text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerError(param msg: string ...?n, param errorDepth: int) {
  }


  /*
     Generate a compile-time warning.
     The warning text is a concatenation of the arguments.
  */
  proc compilerWarning(param msg: string ...?n) {
  }

  /*
     Generate a compile-time warning.
     The warning text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerWarning(param msg: string ...?n, param errorDepth: int) {
  }


  //
  // TODO: improve the compilerAssert() functions in ChapelBase.chpl:
  //  * do we want to +1 when passing errorDepth to compilerError() ?
  //

  /* Generate a compile-time error if the `test` argument is false.
  */
  proc compilerAssert(param test: bool)
  { }

  /* Generate a compile-time error if the `test` argument is false.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerAssert(param test: bool, param errorDepth: int)
  { }

  /* Generate a compile-time error if the `test` argument is false.
     The warning text is a concatenation of the string arguments.
  */
  proc compilerAssert(param test: bool, param msg: string ...?n)
  { }

  /* Generate a compile-time error if the `test` argument is false.
     The warning text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  proc compilerAssert(param test: bool, param msg: string ...?n, param errorDepth: int)
  { }


  /* Compute the minimum value of 2 or more arguments
     using the ``<`` operator for comparison. */
  inline proc min(x, y...) return min();  // dummy

  /* Compute the maximum value of 2 or more arguments
     using the ``>`` operator for comparison. */
  inline proc max(x, y...) return max();  // dummy

  /*
    Exit the program

    :arg status: The exit code for the program
  */
  inline proc exit(status: int) {
    __primitive("chpl_exit_any", status);
  }
}
