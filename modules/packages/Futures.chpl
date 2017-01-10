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

A :class:`Future` object is a read-only container that can store the result of
an asynchronous operation, which can be retrieved when the result is ready.

Examples
--------

.. code-block:: chapel

   use Futures;

   config const X = 42;

   const A = new Future(lambda(x: int) { return 2 * x; }, X);
   const B = new Future(lambda(x: int) { return x + 7; }, A.get(),
                        ExecMode.Async);
   const C = new Future(lambda(x: int) { return 3 * x; }, B.get(),
                        ExecMode.Defer);

   writeln(C.get() == (3 * ((2 * X) + 7)));

 */

module Futures {

  use Reflection;

  /*
    The execution mode for a :class:`Future`.

    * :enum:`ExecMode.Async` specifies that the task associated with this
      future should be executed asynchronously; i.e., by an implicit `begin`
      statement.

    * :enum:`ExecMode.Defer` specifies that the task associated with this
      future should defer execution until either :proc:`Future.get()` or
      :proc:`Future.trigger()` is called.
   */
  enum ExecMode {
    Async,
    Defer
  };

  /*
   */
  class Future {

    pragma "no doc"
    var taskFn;

    pragma "no doc"
    var args;

    pragma "no doc"
    type retType = taskFn.retType;

    pragma "no doc"
    const execMode: ExecMode;

    pragma "no doc"
    var retval: sync retType;

    pragma "no doc"
    var launched: atomic bool;

    pragma "no doc"
    proc Future(taskFn, args...) {
      param mode = ExecMode.Async;
      if !canResolveMethod(taskFn, "this", (...args)) then
        compilerError("Type mismatch in function and arguments");
      execMode = mode;
      if mode == ExecMode.Defer then
        launched.write(false);
      else {
        launched.write(true);
        begin retval.writeEF(taskFn((...args)));
      }
    }

    /*
      Create a :class:`Future` object.

      :arg taskFn: The function to invoke
      :arg args...: The arguments to pass to task function `taskFn`
      :arg mode: An optional execution mode
      :type mode: :enum:`ExecMode`
     */
    proc Future(taskFn, args..., mode: ExecMode = ExecMode.Async) {
      if !canResolveMethod(taskFn, "this", (...args)) then
        compilerError("Type mismatch in function and arguments");
      execMode = mode;
      if mode == ExecMode.Defer then
        launched.write(false);
      else {
        launched.write(true);
        begin retval.writeEF(taskFn((...args)));
      }
    }

    /*
      Get the result of a future, blocking until it is available.
     */
    proc get(): retType {
      wait();
      return retval.readFF();
    }

    /*
      Start execution of the asynchronous task associated with this future.
      (This only has an effect on futures specified with
      :enum:`ExecMode.Defer`.)
     */
    proc trigger() {
      if execMode == ExecMode.Defer && !launched.testAndSet() then
        begin retval.writeEF(taskFn((...args)));
    }

    /*
      Wait for the result of the future to become available.
     */
    proc wait() {
      if !launched.testAndSet() then
        retval.writeEF(taskFn((...args)));
    }

    /*
      Test whether the result of the future is available.
     */
    proc ready(): bool {
      return retval.isFull;
    }

  } // class Future

} // module Futures
