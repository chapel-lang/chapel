/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

/* Support for error conditions and error-handling.

   This module contains several features related to error conditions and error
   handling:

     * :class:`Error` - the parent type for errors in Chapel that can be
       thrown and caught - as well as a few subclasses
     * runtime functions :proc:`halt`, :proc:`warning`, :proc:`assert`, and
       :proc:`exit`
     * compilation diagnostic functions :proc:`compilerError`,
       :proc:`compilerWarning`, and :proc:`compilerAssert`
 */
pragma "module included by default"
module Errors {
  private use ChapelStandard;
  private use ChapelLocks;
  private use CTypes;

  // Base class for errors
  // TODO: should Error include list pointers for TaskErrors?
  /* :class:`Error` is the base class for errors */
  class Error {
    @chpldoc.nodoc
    var _next: unmanaged Error? = nil; // managed by lock in record TaskErrorsRecord

    // These fields save the line/file where the error was thrown.
    @chpldoc.nodoc
    var thrownLine:int;
    @chpldoc.nodoc
    var thrownFileId:int(32);

    @chpldoc.nodoc
    var _msg: string;

    @chpldoc.nodoc
    var _hasThrowInfo: bool = false;

    /* Construct an Error */
    proc init() {
      _next = nil;
    }

    /* Construct an :class:`Error` with a message. */
    proc init(msg: string) {
      this._msg = msg;
    }

    /* Override this method to provide an error message
       of type string in case the error is printed out or never caught.

       :rtype: string
    */
    proc message():string {
      return _msg;
    }
  }

  /*
     If a `nil` :class:`Error` is thrown, :class:`NilThrownError`
     will be thrown instead.
   */
  class NilThrownError : Error {
    @chpldoc.nodoc
    override proc message() {
      return "thrown error was nil";
    }
  }

  /*
    A `NilClassError` is thrown if a cast from a `nil` class is made.
  */
  class NilClassError : Error {
    @chpldoc.nodoc
    override proc message() {
      return "cannot convert nil class to non nilable type";
    }
  }

  /*
    A `ClassCastError` is thrown if a cast between class types fails and the
    destination is not nilable.
  */
  class ClassCastError : Error {
    @chpldoc.nodoc
    override proc message() {
      return "cannot cast class to type - runtime types not compatible";
    }
  }

  /*
   A `DecodeError` is thrown if an attempt to create a string with non-UTF8 byte
   sequences is made at runtime. This includes calling the
   `bytes.decode(decodePolicy.strict)` method on a ``bytes`` with non-UTF8 byte
   sequences.
   */
  class DecodeError: Error {

    @chpldoc.nodoc
    override proc message() {
      return "Invalid UTF-8 character encountered.";
    }
  }

  /*
    An `IllegalArgumentError` is thrown if bad arguments are passed as arguments
    to procedures.
  */
  class IllegalArgumentError : Error {
    @chpldoc.nodoc
    proc init() {}

    @chpldoc.nodoc
    proc init(msg: string) {
      super.init(msg);
    }
  }


  /*
    A ``CodepointSplitError`` is thrown when slicing a string with
    byteIndex-based ranges where the range boundaries do not align
    with codepoint boundaries.
  */
  class CodepointSplitError: Error {
    @chpldoc.nodoc
    proc init(info: string) {
      super.init(info);
    }

    @chpldoc.nodoc
    override proc message() {
      return "Attempting to split a multi-byte codepoint. " + _msg;
    }
  }

  @unstable("`ArrayOomError` is unstable; expect this error to change in the future.")
  class ArrayOomError: Error {
    @chpldoc.nodoc
    override proc message() {
      return "out of memory allocating array elements";
    }
  }

  @deprecated(notes=":class:`CodepointSplittingError` is deprecated; please use :class:`CodepointSplitError` instead")
  type CodepointSplittingError = CodepointSplitError;

  // Used by the runtime to accumulate errors. This type
  // supports adding errors concurrently but need not support
  // iterating over the errors concurrently. Errors
  // will be read from this after all tasks that can add
  // errors have completed; at that point it no longer needs
  // to be parallel-safe.
  record chpl_TaskErrors {
    var _head: unmanaged Error? = nil;
    var _errorsLock: chpl_LocalSpinlock;

    proc ref append(err: unmanaged Error) {
      on this {
        _errorsLock.lock();
        err._next = _head;
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
    // The array stores nilable errors so that users can transfer
    // out of them (e.g. throw one of them).
    @chpldoc.nodoc
    var nErrors: int;
    @chpldoc.nodoc
    var errorsArray: c_ptr(owned Error?);

    @chpldoc.nodoc
    proc init(ref group:chpl_TaskErrors) {
      var head: unmanaged Error? = group._head;
      group._head = nil;
      init this;

      // Count the number of errors, including from nested errors
      var n = 0;
      var cur = head;
      while const curr = cur {
        var curnext = curr._next;
        if const asTaskErr = curr: unmanaged TaskErrors? {
          on asTaskErr do
            for asTaskErr do
              n += 1;
        } else {
          n += 1;
        }
        cur = curnext;
      }

      // Allocate the array to the appropriate size
      // (Note, this assumes that owned Error can be zero'd
      //  and that is valid initialization)
      nErrors = n;
      errorsArray = allocate(owned Error?, n:c_size_t, clear=true);

      // Gather the errors into errorsArray starting at index idx
      var idx = 0;
      cur = head;
      while const curr = cur {
        var curnext = curr._next;
        curr._next = nil; // remove from any lists
        if const asTaskErr = curr: unmanaged TaskErrors? {
          const origLoc = here, EA = errorsArray, idxPtr = c_ptrTo(idx); //RVF
          // For performance we want to bring over asTaskError.errorsArrays
          // in one comm. [Ensure asTaskError.deinit happens properly.]
          // Ideally, use the same copy in the first while-loop as well.
          on asTaskErr do
            for e in asTaskErr do
              // e is an owned error
              on origLoc {
                EA[idxPtr.deref()] = e;
                idxPtr.deref() += 1;
              }
          delete asTaskErr;
        } else {
          errorsArray[idx] = owned.adopt(curr);
          idx += 1;
        }
        cur = curnext;
      }
    }

    /* Create a :class:`TaskErrors` containing only the passed error */
    proc init(err: unmanaged Error) {
      nErrors = 1;
      errorsArray = allocate(owned Error?, 1, clear=true);
      init this;
      err._next = nil;
      errorsArray[0] = owned.adopt(err);
    }

    /* Create a :class:`TaskErrors` not containing any errors */
    proc init() {
      nErrors = 0;
      errorsArray = nil;
    }

    @chpldoc.nodoc
    proc deinit() {
      if errorsArray then on this {
        for i in 0..#nErrors {
          errorsArray[i] = nil;
        }
        deallocate(errorsArray);
      }
    }

    /* Iterate over the errors contained in this :class:`TaskErrors`.
       For example

         .. code-block:: chapel

           var taskErrors:TaskErrors = ...;
           for containedError in taskErrors {
             // Do something with the contained error
           }

       Yields references to ``owned Error?`` so that one of the
       yielded errors might be re-thrown. Only yields values
       that are not storing ``nil`` at the time of the call.
     */
    iter these() ref : owned Error? {
      if boundsChecking then assert(this.locale.id == here.id,
        "iterating over a TaskErrors object allocated on locale ",
        this.locale.id, " while being on locale ", here.id);
      foreach i in 0..#nErrors {
        if errorsArray[i] != nil {
          yield errorsArray[i];
        }
      }
    }

    /* Returns the first non-nil error contained in this TaskErrors group */
    @unstable("`TaskErrors.first` is unstable; expect this method to change in the future.")
    proc first() ref : owned Error? {
      if boundsChecking then assert(this.locale.id == here.id,
        "querying first() of a TaskErrors object allocated on locale ",
        this.locale.id, " while being on locale ", here.id);
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

      for err in these() {
        var e = err!;
        if minMsg == "" || e.message() < minMsg then
          minMsg = e.message();
        if maxMsg == "" || e.message() > maxMsg then
          maxMsg = e.message();

        n += 1;
      }

      // Set first and last.
      {
        for err in these() {
          var e = err!;
          if e.message() == minMsg {
            if first == nil then
              first = e;
            last = e;
          }
        }
        if minMsg != maxMsg {
          for err in these() {
            var e = err!;
            if e.message() == maxMsg {
              last = e;
            }
          }
        }
      }

      var ret = n:string + " errors: ";

      if first != nil && last != nil && first != last then
        ret += chpl_describe_error(first!) + " ... " + chpl_describe_error(last!);
      else if first != nil then
        ret += chpl_describe_error(first!);

      return ret;
    }

    /*
       Iterate over those errors contained that are the passed type
       or a subclass of that type.

       Note that this iterator yields values of type ``owned Error?``
       but only those that are non-nil and have dynamic type ``t``.
     */
    iter filter(type t) ref : owned Error?
      where isSubtype(t:borrowed class, borrowed Error) {

      foreach e in these() {
        var tmp = (e.borrow():borrowed class?):(t:borrowed class?);
        if tmp then
          yield e;
      }
    }
    @chpldoc.nodoc
    iter filter(type t) {
      compilerError("Filtered iterator only supports subclasses of Error");
    }
    /* Returns `true` if this :class:`TaskErrors` contains an error
       of the given type or a subclass of that type. */
    proc contains(type t) {
      for filter(t) {
        return true;
      }
      return false;
    }
  }

  proc chpl_error_type_name(err: borrowed Error) : string {
    var cid =  __primitive("getcid", err);
    var nameC = __primitive("class name by id", cid);
    var nameS: string;
    try! {
      nameS = string.createCopyingBuffer(nameC:c_ptrConst(c_char));
    }
    return nameS;
  }
  proc chpl_describe_error(err: borrowed Error) : string {
    var nameS = chpl_error_type_name(err);

    var ret = nameS + ": " + err.message();

    return ret;
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_do_fix_thrown_error(err: unmanaged Error?): unmanaged Error {

    var fixErr: unmanaged Error? = err;
    if fixErr == nil then
      fixErr = new unmanaged NilThrownError();

    const line = __primitive("_get_user_line");
    const fileId = __primitive("_get_user_file");

    //
    // TODO: Adjust/remove calls to this routine that are present in catch
    // blocks rather than doing extra work at runtime?
    //
    if !fixErr!._hasThrowInfo {
      fixErr!._hasThrowInfo = true;
      fixErr!.thrownLine = line;
      fixErr!.thrownFileId = fileId;
    }

    return _to_nonnil(fixErr);
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  pragma "ignore in global analysis"
  proc chpl_fix_thrown_error(in err: owned Error?): unmanaged Error {
    return chpl_do_fix_thrown_error(owned.release(err));
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  pragma "ignore transfer errors"
  pragma "ignore in global analysis"
  proc chpl_fix_thrown_error(in err: owned Error): unmanaged Error {
    return chpl_do_fix_thrown_error(owned.release(err));
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_fix_thrown_error(err: _nilType) {
    return chpl_do_fix_thrown_error(nil);
  }

  pragma "last resort"
  proc chpl_fix_thrown_error(err) {
    type t = err.type;
    if isCoercible(t, borrowed Error?) {
      compilerError("Cannot throw an instance of type \'", t:string,
                    "\' - please throw owned", 1);
    } else if isClassType(t) {
      compilerError("Cannot throw an instance of type \'",
                    (t: borrowed): string,
                    "\', not a subtype of Error");
    } else {
      compilerError("Cannot throw an instance of type \'", t: string,
                    "\', not a subtype of Error");
    }
  }

  pragma "last resort"
  proc chpl_fix_thrown_error(type errType) {
    compilerError("Cannot throw a type: '", errType:string, "'. Did you forget the keyword 'new'?");
  }

  proc chpl_delete_error(err: unmanaged Error?) {
    if err != nil then delete err;
  }
  pragma "function terminates program"
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_uncaught_error(err: unmanaged Error) {
    extern proc chpl_error_preformatted(ptr:c_ptrConst(c_char));

    const myFileC = __primitive("chpl_lookupFilename",
                                         __primitive("_get_user_file"));
    var myFileS: string;
    try! {
      myFileS = string.createCopyingBuffer(myFileC:c_ptrConst(c_char));
    }
    const myLine = __primitive("_get_user_line");

    const thrownFileC = __primitive("chpl_lookupFilename",
                                             err.thrownFileId);
    var thrownFileS: string;
    try! {
      thrownFileS = string.createCopyingBuffer(thrownFileC:c_ptrConst(c_char));
    }
    const thrownLine = err.thrownLine;

    var s = "uncaught " + chpl_describe_error(err) +
            "\n  " + thrownFileS + ":" + thrownLine:string + ": thrown here" +
            "\n  " + myFileS + ":" + myLine:string + ": uncaught here";
    chpl_error_preformatted(s.c_str());
  }
  // This is like the above, but it is only ever added by the
  // compiler. In case of iterator inlining (say), this call
  // should be replaced by goto-error-handling.
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_propagate_error(err: unmanaged Error) {
    chpl_uncaught_error(err);
  }
  // This function is called to "normalize" the error returned
  // from a forall loop, so that it is always TaskErrors
  // (since the author of the forall loop shouldn't need to know
  //  how many tasks were run in that loop).
  proc chpl_forall_error(err: unmanaged Error) : unmanaged Error {
    if err:unmanaged TaskErrors? then
      return err;
    // If err wasn't a taskError, wrap it in one
    return new unmanaged TaskErrors(err);
  }

  // The compiler generates functions to cast from strings to enums. This
  // function helps the compiler throw errors from those generated casts.
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_enum_cast_error(casted: string, enumName: string) throws {
    if casted.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty string to enum '" + enumName + "'");
    else
      throw new owned IllegalArgumentError("bad cast from string '" + casted + "' to enum '" + enumName + "'");
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_enum_cast_error(casted: integral, enumName: string) throws {
    throw new owned IllegalArgumentError("bad cast from int '" + casted:string + "' to enum '" + enumName + "'");
  }

  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_enum_cast_error_no_int(enumName: string, constName: string): int throws {
    throw new owned IllegalArgumentError("bad cast: enum '" + enumName + "." +
                                          constName + "' has no integer value");
  }


  // The compiler generates functions to cast from bytes to enums. This
  // function helps the compiler throw errors from those generated casts.
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc chpl_enum_cast_error(casted: bytes, enumName: string) throws {
    if casted.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty bytes to enum '" + enumName + "'");
    else
      throw new owned IllegalArgumentError("bad cast from bytes '" +
                                           casted.decode(decodePolicy.replace) +
                                           "' to enum '" + enumName + "'");
  }

  /*
    Assert that a boolean condition is true.  If it is false, prints
    'assert failed' and halts the program.

    .. note :: In the current implementation, this assert never becomes a no-op.
               That is, using it will always incur execution-time checks.

    :arg test: the boolean condition
    :type test: `bool`
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc assert(test: bool) {
    if !test then
      __primitive("chpl_error", "assert failed".c_str());
  }


  /*
    Assert that a boolean condition is true.  If it is false, prints
    'assert failed - ' followed by all subsequent arguments, as though
    printed using :proc:`~IO.write()`.

    .. note :: In the current implementation, this assert never becomes a no-op.
               That is, using it will always incur execution-time checks.

    :arg test: the boolean condition
    :type test: `bool`

    :arg args: other arguments to print
  */
  pragma "insert line file info"
  pragma "always propagate line file info"
  proc assert(test: bool, args...) {
    if !test {
      var tmpstring = "assert failed - " + chpl_stringify_wrapper((...args));
      __primitive("chpl_error", tmpstring.c_str());
    }
  }

  //
  // compile-time diagnostics
  //
  // Note: the message printed by "error" and "warning" primitives
  // consists of the formals of the enclosing function, not their own args.
  //

  /*
     Generate a compile-time error.
     The error text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  @chpldoc.nodoc
  proc compilerError(param msg: string..., param errorDepth: int) {
    __primitive("error");
  }

  /*
     Generate a compile-time error.
     The error text is a concatenation of the arguments.
  */
  proc compilerError(param msg: string...) {
    __primitive("error");
  }

  /*
     Generate a compile-time warning.
     The warning text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  @chpldoc.nodoc
  proc compilerWarning(param msg: string..., param errorDepth: int) {
    __primitive("warning");
  }

  /*
     Generate a compile-time warning.
     The warning text is a concatenation of the arguments.
  */
  proc compilerWarning(param msg: string...) {
    __primitive("warning");
  }

  /*
     Generate a compile-time error if the `test` argument is false.
  */
  proc compilerAssert(param test: bool) {
    if !test then compilerError("assert failed");
  }

  /*
     Generate a compile-time error if the `test` argument is false.

     :arg errorDepth: controls the depth of the error stack trace
  */
  @chpldoc.nodoc
  proc compilerAssert(param test: bool, param errorDepth: int) {
    if !test then compilerError("assert failed", errorDepth + 1);
  }

  /*
     Generate a compile-time error if the `test` argument is false.
     The warning text is a concatenation of the string arguments.
  */
  proc compilerAssert(param test: bool, param msg: string...) {
    if !test then compilerError("assert failed - ", (...msg));
  }

  /*
     Generate a compile-time error if the `test` argument is false.
     The warning text is a concatenation of the string arguments.

     :arg errorDepth: controls the depth of the error stack trace
  */
  @chpldoc.nodoc
  proc compilerAssert(param test: bool, param msg: string...,
                      param errorDepth: int) {
    if !test then compilerError("assert failed - ", (...msg), errorDepth + 1);
  }

  //
  // end compile-time diagnostics
  //

  /*
    Exit the program.

    The compiler ignores the code following a call to `exit`
    until the end of the block where this call is made.

    :arg status: The exit code for the program
  */
  pragma "function terminates program"
  inline proc exit(status: int=0) {
    __primitive("chpl_exit_any", status);
  }

  /*
     Prints an error message to stderr giving the location of the call to
     ``halt`` in the Chapel source, then exits the program.

     The compiler ignores the code following a call to `halt`
     until the end of the block where this call is made.
   */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc halt() {
    if chpl_cpuVsGpuToken {
      __primitive("chpl_error", "halt reached".c_str());
    } else {
      __primitive("chpl_gpu_halt");
    }
  }

  pragma "function terminates program"
  pragma "always propagate line file info"
  @chpldoc.nodoc  // documented in the varargs overload
  proc halt(msg:string) {
    halt(msg.localize().c_str());
  }

  /*
     Prints an error message to stderr giving the location of the call to
     ``halt`` in the Chapel source, followed by the arguments to the call,
     then exits the program.

     The compiler ignores the code following a call to `halt`
     until the end of the block where this call is made.
   */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc halt(args...) {
    var tmpstring = "halt reached - " + chpl_stringify_wrapper((...args));
    __primitive("chpl_error", tmpstring.c_str());
  }

  /*
    Prints a warning to stderr giving the location of the call to ``warning``
    in the Chapel source, followed by the argument(s) to the call.
  */
  pragma "always propagate line file info"
  proc warning(msg:string) {
    __primitive("chpl_warning", msg.localize().c_str());
  }

  /*
    Prints a warning to stderr giving the location of the call to ``warning``
    in the Chapel source, followed by the argument(s) to the call.
  */
  pragma "always propagate line file info"
  proc warning(args...) {
    var tmpstring = chpl_stringify_wrapper((...args));
    warning(tmpstring);
  }
}
