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

// ChapelBase.chpl
//

module ChapelBase
{
  //
  // Assignment operators
  //
  inline proc =  (ref a : bool, b : bool) : void { __primitive("=", a, b); }
  inline proc =  (ref a : int,  b : int ) : void { __primitive("=", a, b); }
  inline proc =  (ref a : real, b : real) : void { __primitive("=", a, b); }

  //
  // Basic unary arithmetic operators
  //

  inline proc + (a : int ) : int  return a;
  inline proc + (a : real) : real return a;

  inline proc - (a : int ) : int  return __primitive("u-", a);
  inline proc - (a : real) : real return __primitive("u-", a);

  //
  // Basic binary arithmetic operators
  //
  inline proc +  (    a : int,  b : int ) : int  return __primitive("+",  a, b);
  inline proc -  (    a : int,  b : int ) : int  return __primitive("-",  a, b);
  inline proc *  (    a : int,  b : int ) : int  return __primitive("*",  a, b);
  inline proc /  (    a : int,  b : int ) : int  return __primitive("/",  a, b);

  inline proc +  (    a : real, b : real) : real return __primitive("+",  a, b);
  inline proc -  (    a : real, b : real) : real return __primitive("-",  a, b);
  inline proc *  (    a : real, b : real) : real return __primitive("*",  a, b);
  inline proc /  (    a : real, b : real) : real return __primitive("/",  a, b);

  //
  // Equality comparison on primitive types
  //
  inline proc == (    a : bool, b : bool) : bool return __primitive("==", a, b);
  inline proc == (    a : int,  b : int ) : bool return __primitive("==", a, b);
  inline proc == (    a : real, b : real) : bool return __primitive("==", a, b);

  inline proc != (    a : bool, b : bool) : bool return __primitive("!=", a, b);
  inline proc != (    a : int,  b : int ) : bool return __primitive("!=", a, b);
  inline proc != (    a : real, b : real) : bool return __primitive("!=", a, b);

  //
  // Ordered comparison on primitive types
  //
  inline proc <  (    a : int,  b : int ) : bool return __primitive("<",  a, b);
  inline proc <  (    a : real, b : real) : bool return __primitive("<",  a, b);

  inline proc >  (    a : int,  b : int ) : bool return __primitive(">",  a, b);
  inline proc >  (    a : real, b : real) : bool return __primitive(">",  a, b);

  inline proc <= (    a : int,  b : int ) : bool return __primitive("<=", a, b);
  inline proc <= (    a : real, b : real) : bool return __primitive("<=", a, b);

  inline proc >= (    a : int,  b : int ) : bool return __primitive(">=", a, b);
  inline proc >= (    a : real, b : real) : bool return __primitive(">=", a, b);

  //
  // Absolute value
  //

  proc abs(a : int) : int
  {
    var retval : int  = a;

    if (a < 0) then
      retval = 0 - a;

    return retval;
  }

  proc abs(a : real) : real
  {
    var retval : real = a;

    if (a < 0.0) then
      retval = 0.0 - a;

    return retval;
  }

  //
  // Support for implicit boolean conversion for conditional expressions
  //
  inline proc _cond_test(x : bool) : bool return x;
  inline proc _cond_test(x : int ) : bool return x != 0;


  //
  // These are implemented within the interpreter
  //

  extern proc print(x : bool)                      : void;
  extern proc print(x : int)                       : void;
  extern proc print(x : real)                      : void;
  extern proc print(x : c_string)                  : void;

  extern proc writeln(format : c_string)           : void;

  extern proc writeln(format : c_string, x : bool) : void;
  extern proc writeln(format : c_string, x : int)  : void;
  extern proc writeln(format : c_string, x : real) : void;

  extern proc quit()                               : void;
}
