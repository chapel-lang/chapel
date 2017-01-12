/*
 * Copyright 2004-2017 Cray Inc.
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

Containers for accessing the results of asynchronous execution.

A :record:`Future` object is a container that can store the result of an
asynchronous operation, which can be retrieved when the result is ready.

Usage
-----

A :record:`Future` object is not created directly. Instead, a future may be
created by calling the :proc:`async()` function, which takes as arguments
the function to be executed and all arguments to that function.

The following example demonstrates a trivial use of futures. Three computations
are executed asynchronously.

.. code-block:: chapel

   use Futures;

   config const X = 42;

   const A = async(lambda(x: int) { return 2 * x; }, X);
   const B = async(lambda(x: int) { return 3 * x; }, X);
   const C = async(lambda(x: int) { return 4 * x; }, X);

   writeln(A.get());
   writeln(B.get());
   writeln(C.get());

Future Chaining
---------------

A continuation to a future (itself a future) can be created via the
:proc:`Future.andThen()` method, which takes as its single argument a function
to be invoked asynchronously (with respect to other tasks) but strictly ordered
in execution after the result of the parent future is ready. The continuation
function takes a single argument, the result of the parent future.

The following examples demonstrate such chaining of futures.

.. code-block:: chapel

   use Futures;

   config const X = 42;

   const F = async(lambda(x: int) { return 2 * x; }, X)
     .andThen(lambda(x: int) { return x + 7; })
     .andThen(lambda(x: int) { return 3 * x; });

   writeln(F.get() == (3 * ((2 * X) + 7))); // prints "true"

.. code-block:: chapel

   use Futures;

   config const X = 1234;

   var F = async(lambda(x:int) { return x:string; }, X)
     .andThen(lambda(x:string) { return(x.length); });

   writeln(F.get()); // prints "4"

 */

module Futures {

  use Reflection;
  use RefCount;

  pragma "no doc"
  class FutureClass: RefCountBase {

    type retType;

    var value: single retType;

    proc FutureClass(type retType) {
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
    var classRef: FutureClass(retType);

    pragma "no doc"
    proc Future(type retType) {
      acquire(new FutureClass(retType));
    }

    pragma "no doc"
    proc ~Future() {
      release();
    }

    /*
      Get the result of a future, blocking until it is available.
     */
    proc get(): retType {
      return classRef.value.readFF();
    }

    /*
      Test whether the result of the future is available.
     */
    proc isReady(): bool {
      return classRef.value.isFull;
    }

    /*
      Asynchronously execute a function as a continuation of the future.

      The function argument `taskFn` must take a single argument of type
      `retType` (i.e., the return type of the parent future) and will be
      executed when the parent future's value is available.

      :arg taskFn: The function to invoke as a continuation.
      :returns: A future of the return type of `taskFn`
     */
    proc andThen(taskFn) {
      /*
      if !canResolveMethod(taskFn, "this", retType) then
        compilerError("Mismatch in async task function and arguments");
      */
      var f: Future(taskFn.retType);
      begin f.classRef.value.writeEF(taskFn(this.get()));
      return f;
    }

    pragma "no doc"
    proc acquire(newRef: FutureClass) {
      classRef = newRef;
      classRef.incRefCount();
    }

    pragma "no doc"
    proc acquire() {
      classRef.incRefCount();
    }

    pragma "no doc"
    proc release() {
      var rc = classRef.decRefCount();
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
  if lhs.classRef != nil then
      lhs.release();
    lhs.acquire(rhs.classRef);
  }

  /*
    Asynchronously execute a function (taking no arguments) and return a
    :record:`Future` that will eventually hold the result of the function call.

    :arg taskFn: A function taking no arguments
    :returns: A future of the return type of `taskFn`
   */
  proc async(taskFn) {
    if !canResolveMethod(taskFn, "this") then
      compilerError("Async task function (expecting arguments) called without arguments");
    var f: Future(taskFn.retType);
    begin f.classRef.value.writeEF(taskFn());
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
      compilerError("Async task function called with mismatching arguments");
    var f: Future(taskFn.retType);
    begin f.classRef.value.writeEF(taskFn((...args)));
    return f;
  }

} // module Futures
