/*
 * Copyright 2004-2020 Cray Inc.
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

.. |---| unicode:: U+2014

Containers for accessing the results of asynchronous execution.

A :record:`Future` object is a container that can store the result of an
asynchronous operation, which can be retrieved when the result is ready.

Usage
-----

A valid :record:`Future` object is not created directly. Instead, a future may
be created by calling the :proc:`async()` function, which takes as arguments
the function to be executed and all arguments to that function.

The following example demonstrates a trivial use of futures. Three computations
are executed asynchronously.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-simple.chpl
   :language: chapel

.. _valid-futures:

Validity of Futures
-------------------

A future that is initialized by a call to :proc:`async()` or
:proc:`Future.andThen()` is created in a valid state.  Otherwise |---| for
example, when a future is declared but not initialized |---| the future is in
an invalid state and method calls other than :proc:`Future.isValid()` on an
invalid future will :proc:`~ChapelIO.halt()`.  If such a future object is subsequently
assigned to by a call to :proc:`async()` or :proc:`Future.andThen()`, then
the future will become valid.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-validity.chpl
   :language: chapel

Task Arguments
--------------

The task argument in a call to :proc:`async()` or :proc:`Future.andThen()`
may be a :ref:`first-class function <readme-firstClassFns>`, a
:ref:`lambda function <readme-lambdaFns>`, or a specially-constructed
class or record.  Such a record must have both a `proc this()` method for the
desired computation and a `proc retType type` method that returns the return
type of the `this()` method.  (The requirement for the `retType` method is
a currently limitation that is intended to be resolved in the future.)
For example:

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-taskfn.chpl
   :language: chapel

Future Chaining
---------------

A continuation to a future (itself a future) can be created via the
:proc:`Future.andThen()` method, which takes as its single argument a function
to be invoked asynchronously (with respect to other tasks) but strictly ordered
in execution after the result of the parent future is ready. The continuation
function takes a single argument, the result of the parent future.

The following examples demonstrate such chaining of futures.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-chaining1.chpl
   :language: chapel

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-chaining2.chpl
   :language: chapel

Future Bundling
---------------

A set of futures can be bundled via :proc:`waitAll`, which takes a
variable number of futures as arguments and returns a new future whose return
type is a tuple of the return types of the arguments.  The returned future is
ready only when all the future arguments are ready.

The following example demonstrate bundling of futures.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-waitall.chpl
   :language: chapel

 */

module Futures {

  private use Reflection;
  private use ExplicitRefCount;

  pragma "no doc"
  class FutureClass: RefCountBase {

    type retType;

    var valid: bool = false;
    var value: retType;
    var state: atomic bool;

    proc init(type retType) {
      this.retType = retType;
      this.complete();
      refcnt.write(0);
      state.clear();
    }

  } // class FutureClass

  /*
    A container that can store the result of an asynchronous operation,
    which can be retrieved when the result is ready.

    A future is not created directly. Instead, one is created by calling the
    :proc:`async()` function or the :proc:`Future.andThen()` method on
    an already-existing future.
   */
  record Future {

    /*
      The return type of the future.
     */
    type retType;

    pragma "no doc"
    var classRef: unmanaged FutureClass(retType)? = nil;

    pragma "no doc"
    proc init(type retType) {
      this.retType = retType;
      this.complete();
      acquire(new unmanaged FutureClass(retType));
    }

    pragma "no doc"
    proc deinit() {
      release();
    }

    /*
      Get the result of a future, blocking until it is available.

      If the future is not valid, this call will :proc:`~ChapelIO.halt()`.
     */
    proc get(): retType {
      if !isValid() then halt("get() called on invalid future");
      classRef!.state.waitFor(true);
      return classRef!.value;
    }

    pragma "no doc"
    proc set(value: retType) {
      if !isValid() then halt("set() called on invalid future");
      classRef!.value = value;
      var oldState = classRef!.state.testAndSet();
      if oldState then halt("set() called more than once on a future");
    }

    /*
      Test whether the result of the future is available.

      If the future is not valid, this call will :proc:`~ChapelIO.halt()`.
     */
    proc isReady(): bool {
      if !isValid() then halt("isReady() called on invalid future");
      return classRef!.state.peek();
    }

    /*
      Test whether the future is valid. For more,
      :ref:`see above <valid-futures>`.
     */
    inline proc isValid(): bool {
      return ((classRef != nil) && classRef!.valid);
    }

    /*
      Asynchronously execute a function as a continuation of the future.

      The function argument `taskFn` must take a single argument of type
      `retType` (i.e., the return type of the parent future) and will be
      executed when the parent future's value is available.

      If the parent future is not valid, this call will :proc:`~ChapelIO.halt()`.

      :arg taskFn: The function to invoke as a continuation.
      :returns: A future of the return type of `taskFn`
     */
    proc andThen(taskFn) {
      /*
      if !canResolveMethod(taskFn, "this", retType) then
        compilerError("andThen() task function arguments are incompatible with parent future return type");
      */
      if !isValid() then halt("andThen() called on invalid future");
      if !canResolveMethod(taskFn, "retType") then
        compilerError("cannot determine return type of andThen() task function");
      var f: Future(taskFn.retType);
      f.classRef!.valid = true;
      begin f.set(taskFn(this.get()));
      return f;
    }

    pragma "no doc"
    proc acquire(newRef: unmanaged FutureClass) {
      if isValid() then halt("acquire(newRef) called on valid future!");
      classRef = newRef;
      newRef.incRefCount();
    }

    pragma "no doc"
    proc acquire() {
      if classRef == nil then halt("acquire() called on nil future");
      classRef!.incRefCount();
    }

    pragma "no doc"
    proc release() {
      if classRef == nil then halt("release() called on nil future");
      var rc = classRef!.decRefCount();
      if rc == 1 {
        delete classRef;
        classRef = nil;
      }
    }

  } // record Future

  pragma "no doc"
  pragma "init copy fn"
  proc chpl__initCopy(x: Future) {
    x.acquire();
    return x;
  }

  pragma "no doc"
  pragma "auto copy fn"
  proc chpl__autoCopy(x: Future) {
    x.acquire();
    return x;
  }

  pragma "no doc"
  proc =(ref lhs: Future, rhs: Future) {
    if lhs.classRef == rhs.classRef then return;
    if lhs.classRef != nil then
      lhs.release();
    lhs.acquire(rhs.classRef!);
  }

  /*
    Asynchronously execute a function (taking no arguments) and return a
    :record:`Future` that will eventually hold the result of the function call.

    :arg taskFn: A function taking no arguments
    :returns: A future of the return type of `taskFn`
   */
  proc async(taskFn) {
    if !canResolveMethod(taskFn, "this") then
      compilerError("async() task function (expecting arguments) provided without arguments");
    if !canResolveMethod(taskFn, "retType") then
      compilerError("cannot determine return type of andThen() task function");
    var f: Future(taskFn.retType);
    f.classRef!.valid = true;
    begin f.set(taskFn());
    return f;
  }

  /*
    Asynchronously execute a function (taking arguments) and return a
    :record:`Future` that will eventually hold the result of the function call.

    :arg taskFn: A function taking arguments with types matching `args...`
    :arg args...: Arguments to `taskFn`
    :returns: A future of the return type of `taskFn`
   */
  proc async(taskFn, args...) {
    if !canResolveMethod(taskFn, "this", (...args)) then
      compilerError("async() task function provided with mismatching arguments");
    if !canResolveMethod(taskFn, "retType") then
      compilerError("cannot determine return type of async() task function");
    var f: Future(taskFn.retType);
    f.classRef!.valid = true;
    begin f.set(taskFn((...args)));
    return f;
  }

  pragma "no doc"
  proc getRetTypes(arg) type {
    return (arg.retType,);
  }

  pragma "no doc"
  proc getRetTypes(arg, args...) type {
    return (arg.retType, (...getRetTypes((...args))));
  }

  /*
    Bundle a set of futures and return a :record:`Future` that will hold a
    tuple of the results of its arguments (themselves futures).

    :arg futures...: A variable-length argument list of futures
    :returns: A future with a return type that is a tuple of the return type of
       the arguments
   */
  proc waitAll(futures...?N) {
    type retTypes = getRetTypes((...futures));
    var f: Future(retTypes);
    f.classRef!.valid = true;
    begin {
      var result: retTypes;
      for param i in 1..N do
        result[i] = futures[i].get();
      f.set(result);
    }
    return f;
  }

} // module Futures
