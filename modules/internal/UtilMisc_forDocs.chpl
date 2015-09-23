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
  proc compilerError(param x:c_string ...?n, param errorDepth:int) {
    __primitive("error", (...x));
  }

  /*
     These two functions are used primarily within system modules to signal
     a compile time error.  The first version accepts a param int as the
     last argument; this value is used to control the depth of an error
     stack trace
  */
  proc compilerError(param x:c_string ...?n) {
    __primitive("error", (...x));
  }

  proc compilerWarning(param x:c_string ...?n, param errorDepth:int) {
    __primitive("warning", (...x));
  }

  /* Similar to compilerError() but generate a warning rather than an error */
  proc compilerWarning(param x:c_string ...?n) {
    __primitive("warning", (...x));
  }



  /*
     This function is used primarily within system modules to test a compile
     time property. An error message is generated if the property is false

     :arg test: the param value to be tested
  */
  proc compilerAssert(param test: bool)
  { if !test then compilerError("assert failed"); }


  /*
     This function is used primarily within system modules to test a compile
     time property. An error message is generated if the property is false.
     The second parameter, which must be integral, is used to control the
     depth of the error stack traceback.

     :arg test: the param value to be tested
  */
  proc compilerAssert(param test: bool, param arg1:integral)
  { if !test then compilerError("assert failed", arg1:int); }

  /*
     This function of two arguments will be used if the second parameter is not
     integral.  If the first parameter is false, then the second parameter is
     included in the error message that is generated

     :arg test: the param value to be tested
  */

  proc compilerAssert(param test: bool, param arg1) where !isIntegralType(arg1.type)
  { if !test then compilerError("assert failed - ", arg1); }


  proc compilerAssert(param test: bool, param arg1, param arg2)
  { if !test then compilerError("assert failed - ", arg1, arg2); }

  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3); }

  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4); }

  /*
     An extended set of functions that provides additional information for the error
     message. A current limitation in the handling of tuples of parameters prevents
     the use of a variable number of arguments

     :arg test: the param value to be tested
  */
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5); }

  /*
     This variation accepts any number of arguments.  The first five additional parameters are
     included if an error message is generated and then there is an indication that there are
     additional parameters.

     :arg test: the param value to be tested
  */

  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5, argrest...)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5, " [...]"); }


  /*
     This variation accepts the value to be tested, precisely 5 values to be included in the
     error message, and finally an integral value that controls the depth of the error stack.

     :arg test: the param value to be tested
  */

  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5, param arg6: integral)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5, arg6:int); }

  /*
     This variation accepts any number of arguments but detects that the final argument is integral.
     In this version the final argument is treated as an errorDepth as for compilerWarning() and
     compilerError()

     :arg test: the param value to be tested
  */
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5, argrest..., param arglast: integral)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5, " [...]", arglast:int); }





  inline proc min(x, y) return if x < y then x else y;

  /* Compute the minimum value of 2 or more arguments */
  inline proc min(x, y, z...?k) return min(min(x, y), (...z));

  inline proc max(x, y) return if x > y then x else y;

  /* Compute the maximum value of 2 or more arguments */
  inline proc max(x, y, z...?k) return max(max(x, y), (...z));

  /*
    Exit the program

    :arg status: The exit code for the program
  */
  inline proc exit(status: int) {
    __primitive("chpl_exit_any", status);
  }
}
