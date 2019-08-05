/*
 * Copyright 2004-2019 Cray Inc.
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
    var _next: unmanaged Error? = nil; // managed by lock in record TaskErrorsRecord

    // These fields save the line/file where the error was thrown.
    pragma "no doc"
    var thrownLine:int;
    pragma "no doc"
    var thrownFileId:int(32);

    /* Construct an Error */
    proc init() {
      _next = nil;
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
    override proc writeThis(f) {
      var description = chpl_describe_error(this);
      f <~> description;
    }
  }

  /*
     If a `nil` :class:`Error` is thrown, :class:`NilThrownError`
     will be thrown instead.
   */
  class NilThrownError : Error {
    pragma "no doc"
    override proc message() {
      return "thrown error was nil";
    }
  }

  class NilClassError : Error {
    pragma "no doc"
    override proc message() {
      return "cannot convert nil class to non nilable type";
    }
  }

  class ClassCastError : Error {
    pragma "no doc"
    override proc message() {
      return "cannot cast class to type - runtime types not compatible";
    }
  }

  class IllegalArgumentError : Error {
    var formal: string;
    var info: string;

    proc init() {
    }

    proc init(info: string) {
      this.info = info;
    }

    proc init(formal: string, info: string) {
      this.formal = formal;
      this.info   = info;
    }

    override proc message() {
      if formal.isEmpty() then
        return info;
      else
        return "illegal argument '" + formal + "': " + info;
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
    var _head: unmanaged Error? = nil;
    var _errorsLock: chpl_LocalSpinlock;

    proc append(err: unmanaged Error) {
      on this {
        _errorsLock.lock();
        var tmp = _head;
        err._next = tmp;
        _head = err;
        _errorsLock.unlock();
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

    // This could use a regular array but uses a c_ptr
    // instead to work around order-of-resolution issues
    // with the parallel array initialization code.
    pragma "no doc"
    var nErrors: int;
    pragma "no doc"
    var errorsArray: c_ptr(owned Error);

    pragma "no doc"
    proc init(ref group:chpl_TaskErrors) {
      var head: unmanaged Error? = group._head;
      group._head = nil;
      this.complete();

      var cur: unmanaged Error?;

      // Count the number of errors, including from nested errors
      var n = 0;
      cur = head;
      while cur != nil {
        var curnext = cur!._next;
        var asTaskErr: unmanaged TaskErrors? = cur: unmanaged TaskErrors?;
        if asTaskErr == nil {
          n += 1;
        } else {
          for e in asTaskErr! {
            if e != nil then
              n += 1;
          }
        }
        cur = curnext;
      }

      // Allocate the array to the appropriate size
      // (Note, this assumes that owned Error can be zero'd
      //  and that is valid initialization)
      nErrors = n;
      errorsArray = c_calloc(owned Error, n);

      // Gather the errors into errorsArray starting at index idx
      var idx = 0;
      cur = head;
      while cur != nil {
        var curnext = cur!._next;
        cur!._next = nil; // remove from any lists
        var asTaskErr: unmanaged TaskErrors? = cur: unmanaged TaskErrors?;
        if asTaskErr == nil {
          errorsArray[idx].retain(cur!);
          idx += 1;
        } else {
          for e in asTaskErr! {
            // e is an owned error
            if e != nil {
              errorsArray[idx] = e;
              idx += 1;
            }
          }
          delete asTaskErr;
        }
        cur = curnext;
      }
    }

    /* Create a :class:`TaskErrors` containing only the passed error */
    proc init(err: unmanaged Error) {
      nErrors = 1;
      errorsArray = c_calloc(owned Error, 1);
      this.complete();
      err._next = nil;
      errorsArray[0].retain(err);
    }

    /* Create a :class:`TaskErrors` not containing any errors */
    proc init() {
      nErrors = 0;
      errorsArray = nil;
    }

    proc deinit() {
      if errorsArray {
        for i in 0..#nErrors {
          errorsArray[i].clear();
        }
        c_free(errorsArray);
      }
    }

    /* Iterate over the errors contained in this :class:`TaskErrors`.
       For example

         .. code-block:: chapel

           var taskErrors:TaskErrors = ...;
           for containedError in taskErrors {
             // Do something with the contained error
           }

     */
    iter these() ref : owned Error {
      for i in 0..#nErrors do
        yield errorsArray[i];
    }

    /* Returns the first non-nil error contained in this TaskErrors group */
    proc first() ref : owned Error {
      var first = 0;
      for i in 0..#nErrors {
        if errorsArray[i] != nil {
          first = i;
          break;
        }
      }
      return errorsArray[first];
    }

    /*

       Returns a string summarizing the errors contained in this
       :class:`TaskErrors`. The summary is intended to be concise: it will not
       grow arbitrarily long if the :class:`TaskErrors` contains many errors.

     */
    override proc message() : string {
      var n = 0;

      var minMsg:string;
      var maxMsg:string;
      var first:borrowed Error?;
      var last:borrowed Error?;

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

      if first != nil && last != nil && first != last then
        ret += chpl_describe_error(first!) + " ... " + chpl_describe_error(last!);
      else if first != nil then
        ret += chpl_describe_error(first!);

      return ret;
    }

    /*
       Iterate over those errors contained that are the passed type
       or a subclass of that type.
     */
    iter filter(type t) where isSubtype(_to_borrowed(t), borrowed Error) {
      for e in these() {
        var tmp = _to_unmanaged(e):_to_nilable(_to_unmanaged(t));
        if tmp then
          yield _to_nonnil(tmp);
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
  proc chpl_error_type_name(err: borrowed Error) : string {
    var cid =  __primitive("getcid", err);
    var nameC: c_string = __primitive("class name by id", cid);
    var nameS = nameC:string;
    return nameS;
  }
  pragma "no doc"
  proc chpl_describe_error(err: borrowed Error) : string {
    var nameS = chpl_error_type_name(err);

    var ret = nameS + ": " + err.message();

    return ret;
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "always propagate line file info"
  pragma "last resort"
  proc chpl_fix_thrown_error(err: borrowed Error?): unmanaged Error {
    compilerError("Throwing borrowed error - please throw owned", 1);

    return chpl_do_fix_thrown_error(_to_unmanaged(err));
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "last resort"
  proc chpl_fix_thrown_error(type errType) {
    compilerError("Cannot throw a type: '", errType:string, "'. Did you forget the keyword 'new'?");
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_do_fix_thrown_error(err: unmanaged Error?): unmanaged Error {

    var fixErr: unmanaged Error? = err;
    if fixErr == nil then
      fixErr = new unmanaged NilThrownError();

    const line = __primitive("_get_user_line");
    const fileId = __primitive("_get_user_file");
    fixErr!.thrownLine = line;
    fixErr!.thrownFileId = fileId;

    return _to_nonnil(fixErr);
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_fix_thrown_error(err: unmanaged Error?): unmanaged Error {
    // TODO: This should be an error in the future,
    // for now the compiler already adds a warning in this case.
    //compilerWarning("Throwing unmanaged error - please throw owned", 1);

    return chpl_do_fix_thrown_error(err);
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_fix_thrown_error(in err: owned Error?): unmanaged Error {
    return chpl_do_fix_thrown_error(err.release());
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_fix_thrown_error(in err: owned Error): unmanaged Error {
    return chpl_do_fix_thrown_error(err.release());
  }

  pragma "no doc"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_fix_thrown_error(err: _nilType) {
    return chpl_do_fix_thrown_error(nil);
  }

  pragma "no doc"
  pragma "last resort"
  proc chpl_fix_thrown_error(err: ?t) where isRecordType(t) &&
                                            !isClassType(t) {
    compilerError("Cannot throw an instance of type \'", t: string,
                  "\', not a subtype of Error");
  }

  pragma "no doc"
  pragma "last resort"
  proc chpl_fix_thrown_error(err: ?t) where isClassType(t) &&
                                            !isSubtype(t:borrowed, Error) {
    compilerError("Cannot throw an instance of type \'", (t: borrowed): string,
                  "\', not a subtype of Error");
  }

  pragma "no doc"
  pragma "last resort"
  proc chpl_fix_thrown_error(err: ?t) where !isRecordType(t) &&
                                            !isClassType(t) {
    compilerError("Cannot throw an instance of type \'", t: string,
                  "\', not a subtype of Error");
  }

  pragma "no doc"
  proc chpl_delete_error(err: unmanaged Error?) {
    if err != nil then delete err;
  }
  pragma "no doc"
  pragma "function terminates program"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_uncaught_error(err: unmanaged Error) {
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
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_propagate_error(err: unmanaged Error) {
    chpl_uncaught_error(err);
  }
  // This function is called to "normalize" the error returned
  // from a forall loop, so that it is always TaskErrors
  // (since the author of the forall loop shouldn't need to know
  //  how many tasks were run in that loop).
  pragma "no doc"
  proc chpl_forall_error(err: unmanaged Error) : unmanaged Error {
    if err:unmanaged TaskErrors? then
      return err;
    // If err wasn't a taskError, wrap it in one
    return new unmanaged TaskErrors(err);
  }

  // The compiler generates functions to cast from strings to enums. This
  // function helps the compiler throw errors from those generated casts.
  pragma "no doc"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_enum_cast_error(casted: string, enumName: string) throws {
    if casted.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty string to " + enumName);
    else
      throw new owned IllegalArgumentError("bad cast from string '" + casted + "' to " + enumName);
  }
}
