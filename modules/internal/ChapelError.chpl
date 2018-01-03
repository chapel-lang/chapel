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

/*
   :class:`Error` is the parent type for errors in Chapel.  :class:`TaskErrors`
   is the type of errors thrown from parallel constructs where more than one
   error can be simultaneously encountered.

 */
module ChapelError {
  use ChapelStandard;

  // Base class for errors
  // TODO: should Error include list pointers for TaskErrors?
  /* :class:`Error` is the base class for errors */
  class Error {
    pragma "no doc"
    var _next: Error = nil; // managed by lock in record TaskErrorsRecord

    // These fields save the line/file where the error was thrown.
    pragma "no doc"
    var thrownLine:int;
    pragma "no doc"
    var thrownFileId:int(32);

    pragma "no doc"
    proc msg: string {
      compilerError("msg field in Error is no longer available");
      return "";
    }

    pragma "no doc"
    proc init(_msg:string) {
      _next = nil;
      compilerError("Error constructor accepting a string is no longer available");
    }

    /* Construct an Error */
    proc init() {
      _next = nil;
      super.init();
    }

    /* Override this method to provide an error message
       in case the error is printed out or never caught.
     */
    proc message() {
      return "";
    }

    /* Errors can be printed out. In that event, they will
       show information about the error including the result
       of calling :proc:`Error.message`.
     */
    proc writeThis(f) {
      var description = chpl_describe_error(this);
      f <~> description;
    }
  }

  /*
     If a `nil` :class:`Error` is thrown, :class:`NilThrownError`
     will be thrown instead.
   */
  pragma "use default init"
  class NilThrownError : Error {
    pragma "no doc"
    proc message() {
      return "thrown error was nil";
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

  /*
     :class:`TaskErrors` stores multiple errors when they can come up.
     For example, a ``coforall`` loop might throw errors from multiple
     tasks at the same time. These errors will be reported to the parent
     task at the end of the ``coforall`` in the form of
     :class:`TaskErrors`.

     Note that errors thrown within a ``coforall``, ``cobegin``, or
     ``forall`` are represented as elements of ``TaskErrors``. In the
     case of nesting, all errors will be stored in a single ``TaskErrors``.

     Errors thrown in ``begin`` tasks will be reported within a ``TaskErrors``
     to the task that uses ``sync`` to wait for those ``begin`` tasks.

   */
  class TaskErrors : Error {
    pragma "no doc"
    var _head: Error = nil;

    pragma "no doc"
    proc init(ref group:chpl_TaskErrors) {
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

    /*
       append a single error to the group
     */
    proc append(err:Error) {
      var tmp = _head;
      err._next = tmp;
      _head = err;
    }

    /* Create a :class:`TaskErrors` containing only the passed error */
    proc init(err: Error) {
      _head = err;
      super.init();
    }

    /* Create a :class:`TaskErrors` not containing any errors */
    proc init() {
      _head = nil;
      super.init();
    }

    /* Iterate over the errors contained in this :class:`TaskErrors`.
       For example

         .. code-block:: chapel

           var taskErrors:TaskErrors = ...;
           for containedError in taskErrors {
             // Do something with the contained error
           }

     */
    iter these() {
      var e = _head;
      while e != nil {
        yield e;
        e = e._next;
      }
    }

    pragma "no doc"
    proc deinit() {
      var e = _head;
      var todelete: Error;
      while e != nil {
        todelete = e;
        e = e._next;
        delete todelete;
      }
    }

    /*

       Returns a string summarizing the errors contained in this
       :class:`TaskErrors`. The summary is intended to be concise: it will not
       grow arbitrarily long if the :class:`TaskErrors` contains many errors.

     */
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

    /*
       Iterate over those errors contained that are the passed type
       or a subclass of that type.
     */
    iter filter(type t) where t:Error {
      for e in these() {
        var tmp = e:t;
        if tmp then
          yield tmp;
      }
    }
    pragma "no doc"
    iter filter(type t) {
      compilerError("Filtered iterator only supports subclasses of Error");
    }
    /* Returns `true` if this :class:`TaskErrors` contains an error
       of the given type or a subclass of that type. */
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
  proc chpl_fix_thrown_error(err: Error): Error {
    var fixErr: Error = err;
    if fixErr == nil then
      fixErr = new NilThrownError();

    const line = __primitive("_get_user_line");
    const fileId = __primitive("_get_user_file");
    fixErr.thrownLine = line;
    fixErr.thrownFileId = fileId;

    return fixErr;
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
