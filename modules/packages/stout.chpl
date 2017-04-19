/*
 * Copyright 2004-2016 Cray Inc.
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

Structures to improve error handling in Chapel programs

General purpose data structures to manage and handle errors that could occur
in an end-user application or program.

This module provides:

1. An "Error" record. 

2. An "Option" record type capturing two states "None" or "Some". This is a
   safe alternative to null/nil testing.

3. The "Try" record type which can either return a value or an "Error" without
   the need for runtime exception handling.

4. "Result" record type - a combination of "Try" and "Option".

Mesos's C++ stout API:

1. https://mesos.apache.org/api/latest/c++/md_3rdparty_libprocess_3rdparty_stout_README.html

2. https://github.com/3rdparty/stout 

Example 1
---------

.. code-block:: chapel

  use stout;

  var os = Option(int).some(10);
  writeln(os.isSome());

  var on = Option(int).none(int);
  writeln(os.isNone());

Example 2
---------

.. code-block:: chapel

  use stout;

   var x : Try(int);

   x = Try(int).error(int, "boo");
   writeln((true, x.isError()));

   var e = new Error(int, "lame");
   writeln((true, x.isError()));

   x = e;
   writeln((true, x.isError()));
   writeln((false, x.isSome()));

   x = Try(int).some(5);
   writeln((false, x.isError()));
   writeln((true, x.isSome()));

stout Types and Functions
--------------------------------

 */
module stout {

   record Error {
      type eltype;
      var msg:string;

  /* Create an Error

     :arg t: the type associated with the Error 
     :arg message: the message associated with the Error 
   */
      proc Error(type eltype, const message:string) {
         msg = message;
      }

      proc this(type eltype) : Try(eltype) {
         return Try(eltype).error(eltype, msg);
      }

      proc this(type eltype) : Result(eltype) {
         return Result(eltype).error(eltype, msg);
      }
   }

   proc =(ref lval:Try(?eltype), rval:Error(eltype)) {
      lval.e = Option(Error(eltype)).some(rval);
   }

   proc =(ref lval:Result(?eltype), rval:Error(eltype)) {
      lval.t = new Result(Error(eltype), rval);
   }

   // capture the concept of void
   // record Nothing {}
 
   enum OptionState { SOME, NONE }

   record Option {
      type option_type;
      var state = OptionState.NONE;
      var t:option_type;      

  /* Create an Option that is a "None" 

     :arg option_type: the type associated with the Option 
   */
      proc Option(type option_type) {
         state = OptionState.NONE;
      }

  /* Create an Option that is a "Some"

     :arg option_type: the type associated with the Option
     :arg val: the inital value
   */
      proc Option(type option_type, const val:option_type) {
         state = OptionState.SOME;
         t = val;
      }

  /* Create an Option from an Option

     :arg option_type: the type associated with the Option
     :arg that: the option to clone 
   */
      proc Option(type option_type, const that:Option(option_type)) {
         state = that.state;
         if that.state != OptionState.NONE { t = that.t; }
      }

  /* Does the Option have value? 
   */
      proc isSome() { return state == OptionState.SOME; }

  /* Does the Option not have value?
   */
      proc isNone() { return state == OptionState.NONE; }

  /* Fetch the Option's value
   */
      proc get() : option_type { assert(state == OptionState.SOME, "Option is not a SOME"); return t; }

  /* Fetch the Option's value 

     :arg val: if the Option is a "None" then assign the Option "val" and return "val".
   */
      proc get(const val:option_type) : option_type {  return if state == OptionState.NONE then val else t; }

  /* Fetch the Option's value 
   */
      proc this() { return get(); }

   }

  /* Create a "None" Option
     :arg option_type: assign the Option "option_type" type
   */
   proc type Option.none(type option_type) {
      return new Option(option_type);
   }

  /* Create a "Some" Option
     :arg val: assign the Option "val"
   */
   proc type Option.some(const val:?option_type) {
      return new Option(option_type, val);
   }

   proc ==(lval:Option(?option_type), rval:Option(option_type)) {
      return (lval.state == OptionState.NONE && rval.state == OptionState.NONE) || (lval.state == OptionState.SOME && rval.state == OptionState.SOME && lval.t == rval.t);
   }

   record Result {
      type result_type;
      var t:Try(Option(result_type)) = Try(Option(result_type)).some(Option(result_type).none(result_type));

      proc Result(type result_type) {
         t = Try(Option(result_type)).some(Option(result_type).none(result_type));
      }

      proc Result(type result_type, const val:result_type) {
         if result_type == string {
            t = Try(Option(result_type)).error(Option(result_type), val);
         }
         else {
            t = Try(Option(result_type)).some(Option(result_type).some(val));
         }
      }

      proc Result(type result_type, const that:Result(result_type)) {
         if that.isSome() {
            t = that.t;
         }
      }

      proc isSome() { return t.isSome() && t.get().isSome(); }
      proc isNone() { return t.isSome() && t.get().isNone(); }
      proc isError() { return t.isError(); }

      proc error() { return t.error(); }

      proc get() {
         if isSome() {
            const tt = t.get();
            if tt.isSome() {
              const ttt = tt.get();
              return ttt;
            }
         }

         assert(false, "error Result.get()");
         return t.get().get();
      }

      proc this() { return get(); }

   }

   proc type Result.none(type result_type) {
      return new Result(result_type);
   }

   proc type Result.some(const val:?T) {
      return new Result(T, val);
   }

   proc type Result.error(type T, const message:string) {
      return new Result(T, message);
   }

   record Try {
      type try_type;
      var t:Option(try_type) = Option(try_type).none(try_type);
      var e:Option(Error(try_type)) = Option(Error(try_type)).none(Error(try_type)); 

      proc Try(type try_type, val:try_type) {
         t = Option(try_type).some(val);
      }

      proc Try(type try_type, const message:string) {
         e = Option(Error(try_type)).some(new Error(try_type, message));
      }

      proc Try(type try_type, const err:Error) {
         e = Option(Error(try_type)).some(err);
      }

      proc Try(type try_type, const that:Try(try_type)) {
         if that.t.isSome() {
            t = that.t;
         }
         else {
            e = Option(Error(try_type)).some(new Error(try_type, that.message));
         }
      }

      proc error() : Error(try_type) {
         return e.get();
      }

      proc isSome() { return t.isSome(); }
      proc isError() { return e.isSome(); }

      proc get() {
         assert(isSome(), "Try is not some");
         return t.get();
      }

      proc this() {
         return get();
      }
   }
   
   proc ==(lval:Try(?T), rval:Try(T)) {
      if lval.isSome() && rval.isSome() {
         return (lval.get() == rval.get());
      }

      return false;
   }

   proc !=(lval:Try(?T), rval:Try(T)) {
      return !(lval == rval);
   }

   proc type Try.some(const val:?T) {
      return new Try(T, val);
   }

   proc type Try.error(type try_type, const message:string) {
      return new Try(try_type, message);
   }

} // end module 

