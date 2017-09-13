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

    // These fields save the line/file where the error was thrown.
    pragma "no doc"
    var thrownLine:int;
    pragma "no doc"
    var thrownFileId:int(32);

    proc Error() {
      _next = nil;
    }

    proc Error(_msg: string) {
      msg = _msg;
      _next = nil;
    }

    proc message() {
      return msg;
    }

    proc writeThis(f) {
      var description = chpl_describe_error(this);
      f <~> description;
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
      var cur:Error = group._head;
      group._head = nil;
      _head = nil;

      // Flatten nested TaskErrors

      while cur != nil {
        // remove it from that list
        var curnext = cur._next;
        cur._next = nil;

        // Add head / errors in it to this list
        var asTaskErr:TaskErrors = cur:TaskErrors;
        if asTaskErr == nil {
          append(cur);
        } else {
          // Remove & add errors in the sub-list
          var sub:Error = asTaskErr._head;
          asTaskErr._head = nil;
          while sub != nil {
            // remove it from that list
            var subnext = sub._next;
            sub._next = nil;

            // add it to this list
            append(sub);

            sub = subnext;
          }
          delete asTaskErr;
        }
        cur = curnext;
      }
    }

    // append a single error to the group
    proc append(err:Error) {
      var tmp = _head;
      err._next = tmp;
      _head = err;
    }

    /* Create a TaskErrors containing only the passed error */
    proc TaskErrors(err: Error) {
      _head = err;
    }

    /* Create a TaskErrors not containing any errors */
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

    proc message() : string {
      var n = 0;

      var minMsg:string;
      var maxMsg:string;
      var first:Error;
      var last:Error;

      for e in these() {
        if minMsg == "" || e.message() < minMsg then
          minMsg = e.message();
        if maxMsg == "" || e.message() > maxMsg then
          maxMsg = e.message();

        n += 1;
      }

      // Set first and last.
      {
        for e in these() {
          if e.message() == minMsg {
            if first == nil then
              first = e;
            last = e;
          }
        }
        if minMsg != maxMsg {
          for e in these() {
            if e.message() == maxMsg {
              last = e;
            }
          }
        }
      }

      var ret = n + " errors: ";

      if first != last then
        ret += chpl_describe_error(first) + " ... " + chpl_describe_error(last);
      else
        ret += chpl_describe_error(first);

      return ret;
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
  proc chpl_error_type_name(err: Error) : string {
    var cid =  __primitive("getcid", err);
    var nameC: c_string = __primitive("class name by id", cid);
    var nameS = nameC:string;
    return nameS;
  }
  pragma "no doc"
  proc chpl_describe_error(err: Error) : string {
    var nameS = chpl_error_type_name(err);

    var ret = nameS + ": " + err.message();

    return ret;
  }

  pragma "no doc"
  pragma "insert line file info"
  proc chpl_save_line_in_error(err: Error) {
    const line = __primitive("_get_user_line");
    const fileId = __primitive("_get_user_file");
    err.thrownLine = line;
    err.thrownFileId = fileId;
  }
  pragma "no doc"
  proc chpl_delete_error(err: Error) {
    if err != nil then delete err;
  }
  pragma "no doc"
  pragma "function terminates program"
  pragma "insert line file info"
  proc chpl_uncaught_error(err: Error) {
    extern proc chpl_error_preformatted(c_string);

    const myFileC:c_string = __primitive("chpl_lookupFilename",
                                         __primitive("_get_user_file"));
    const myFileS = myFileC:string;
    const myLine = __primitive("_get_user_line");



    const thrownFileC:c_string = __primitive("chpl_lookupFilename",
                                             err.thrownFileId);
    const thrownFileS = thrownFileC:string;
    const thrownLine = err.thrownLine;

    var s = "uncaught " + chpl_describe_error(err) +
            "\n  " + thrownFileS + ":" + thrownLine + ": thrown here" +
            "\n  " + myFileS + ":" + myLine + ": uncaught here";
    chpl_error_preformatted(s.c_str());
  }
  // This is like the above, but it is only ever added by the
  // compiler. In case of iterator inlining (say), this call
  // should be replaced by goto-error-handling.
  pragma "no doc"
  proc chpl_propagate_error(err: Error) {
    chpl_uncaught_error(err);
  }
  // This function is called to "normalize" the error returned
  // from a forall loop, so that it is always TaskErrors
  // (since the author of the forall loop shouldn't need to know
  //  how many tasks were run in that loop).
  pragma "no doc"
  proc chpl_forall_error(err: Error):Error {
    if err:TaskErrors then
      return err;
    // If err wasn't a taskError, wrap it in one
    return new TaskErrors(err);
  }
}
