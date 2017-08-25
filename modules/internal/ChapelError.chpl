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

module ChapelError {
  use ChapelStandard;

  // Base class for errors
  // TODO: should base class Error have a string message at all?
  // TODO: should Error include list pointers for TaskErrors?
  class Error {
    var msg: string;
    pragma "no doc"
    var _next: Error = nil; // managed by lock in record TaskErrorsRecord

    proc Error() {
      _next = nil;
    }

    proc Error(_msg: string) {
      msg = _msg;
      _next = nil;
    }

    proc writeThis(f) {
      //f <~> "Error: " <~> msg;
      f <~> "{msg = " <~> msg <~> "}";
    }
  }

  // Used by the runtime to accumulate errors. This type
  // supports adding errors concurrently but need not support
  // iterating over the errors concurrently. Errors
  // will be read from this after all tasks that can add
  // errors have completed; at that point it no longer needs
  // to be parallel-safe.
  pragma "no doc"
  record chpl_TaskErrors {
    var _head: Error;
    var _errorsLock: atomicbool;
    // this atomic controls:
    //  - _head
    //  - all list elements ->_next

    inline proc _lockErrors() {
      // WARNING: If you are calling this function directly from
      // a remote locale, you should consider wrapping the call in
      // an on clause to avoid excessive remote forks due to the
      // testAndSet()
      while (_errorsLock.testAndSet()) do chpl_task_yield();
    }
    inline proc _unlockErrors() {
      _errorsLock.clear();
    }
    proc append(err: Error) {
      on this {
        _lockErrors();
        var tmp = _head;
        err._next = tmp;
        _head = err;
        _unlockErrors();
      }
    }
    proc empty() {
      return _head == nil;
    }
  }

  // stores multiple errors when they can come up.
  class TaskErrors : Error {
    var _head: Error = nil;

    pragma "no doc"
    proc TaskErrors(ref group:chpl_TaskErrors) {
      _head = group._head;
      group._head = nil;
    }
    proc TaskErrors() {
      _head = nil;
    }

    iter these() {
      var e = _head;
      while e != nil {
        yield e;
        e = e._next;
      }
    }

    proc deinit() {
      var e = _head;
      var todelete: Error;
      while e != nil {
        todelete = e;
        e = e._next;
        delete todelete;
      }
    }

    proc writeThis(f) {
      var n = 0;

      var minMsg:string;
      var maxMsg:string;
      var first:Error;
      var last:Error;

      for e in these() {
        if minMsg == "" || e.msg < minMsg then
          minMsg = e.msg;
        if maxMsg == "" || e.msg > maxMsg then
          maxMsg = e.msg;

        n += 1;
      }

      // Set first and last.
      {
        for e in these() {
          if e.msg == minMsg {
            if first == nil then
              first = e;
            last = e;
          }
        }
        if minMsg != maxMsg {
          for e in these() {
            if e.msg == maxMsg {
              last = e;
            }
          }
        }
      }

      f <~> "TaskErrors with ";
      if n > 1 then
        f <~> n <~> " errors: ";

      if first != last then
        f <~> first <~> " ... " <~> last;
      else
        f <~> first;
    }

    // convenience methods
    iter filter(type t) where t:Error {
      for e in these() {
        var tmp = e:t;
        if tmp then
          yield tmp;
      }
    }
    iter filter(type t) {
      compilerError("Filtered iterator only supports subclasses of Error");
    }
    proc contains(type t) {
      for e in filter(t) {
        return true;
      }
      return false;
    }
  }

  pragma "no doc"
  proc chpl_delete_error(err: Error) {
    if err != nil then delete err;
  }
  pragma "function terminates program"
  proc chpl_uncaught_error(err: Error) {
    var tmpstring = "uncaught error - " + stringify(err);
    __primitive("chpl_error", tmpstring.c_str());
  }
  // This is like the above, but it is only ever added by the
  // compiler. In case of iterator inlining (say), this call
  // should be replaced by goto-error-handling.
  proc chpl_propagate_error(err: Error) {
    chpl_uncaught_error(err);
  }

}
