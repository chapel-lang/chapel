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

   This module helps handle system errors.

   This module defines the type ``syserr``, which can encode an error code or
   error message and be returned from routines generating an error. Additional
   routines are provided to print a useful error message from a ``syserr``.

   The IO module uses these routines in a way that supports error inspection
   and rapid prototyping. Most routines in the IO module have two forms.
   In one form, an error (of type syserr) is returned in an out error argument.
   In the second form, no error is returned, and instead the task will halt
   with a fatal error if an error is encountered.

   .. note::

      This module is unrelated to the implementation of error handling in
      Chapel generally, and is being maintained for legacy reasons.

 */
module SysError {

use SysBasic;

/*

   ``SystemError`` is a base class for ``Errors`` generated from ``syserr``.
   It provides factory methods to create different subtypes based on the
   ``syserr`` that is passed.

*/
class SystemError : Error {
  var err:     syserr;
  var details: string;

  proc SystemError(err: syserr, details: string = "") {
    this.err     = err;
    this.details = details;
  }

  /*
     Provides a formatted string output for ``SystemError``, generated
     from the internal ``err`` and the ``details`` string.
  */
  proc message() {
    var strerror_err: err_t = ENOERR;
    var errstr:  c_string   = sys_strerror_syserr_str(err, strerror_err);
    var err_msg: string     = errstr:string;

    if !details.isEmptyString() then
      err_msg = errstr:string + " (" + details + ")";

    return err_msg;
  }

  proc writeThis(f) {
    try! f.write(msg);
  }

  /*
    Return the matching ``SystemError`` subtype for a given ``syserr``,
    with an optional string containing extra details.

    :arg err: the syserr to generate from
    :arg details: extra information to include with the error
  */
  proc type fromSyserr(err: syserr, details: string = "") {
    if err == EAGAIN || err == EALREADY || err == EWOULDBLOCK || err == EINPROGRESS {
      return new BlockingIOError(err, details);
    } else if err == ECHILD {
      return new ChildProcessError(details, err);
    } else if err == EPIPE || err == ESHUTDOWN {
      return new BrokenPipeError(err, details);
    } else if err == ECONNABORTED {
      return new ConnectionAbortedError(details, err);
    } else if err == ECONNREFUSED {
      return new ConnectionRefusedError(details, err);
    } else if err == ECONNRESET {
      return new ConnectionResetError(details, err);
    } else if err == EEXIST {
      return new FileExistsError(details, err);
    } else if err == ENOENT {
      return new FileNotFoundError(details, err);
    } else if err == EINTR {
      return new InterruptedError(details, err);
    } else if err == EISDIR {
      return new IsADirectoryError(details, err);
    } else if err == ENOTDIR {
      return new NotADirectoryError(details, err);
    } else if err == EACCES || err == EPERM {
      return new PermissionError(err, details);
    } else if err == ESRCH {
      return new ProcessLookupError(details, err);
    } else if err == ETIMEDOUT {
      return new TimeoutError(details, err);
    } else if err == EEOF {
      return new EOFError(details, err);
    } else if err == ESHORT {
      return new UnexpectedEOFError(details, err);
    } else if err == EFORMAT {
      return new BadFormatError(details, err);
    } else if err == EINVAL {
      return new InvalidArgumentError(details, err);
    }

    return new SystemError(err, details);
  }

  /*
    Return the matching ``SystemError`` subtype for a given error number,
    with an optional string containing extra details.

    :arg err: the number to generate from
    :arg details: extra information to include with the error
  */
  proc type fromSyserr(err: int, details: string = "") {
    return fromSyserr(err:syserr, details);
  }
}

/*

   ``BlockingIOError`` is the subclass of ``SystemError`` corresponding to
   ``EAGAIN``, ``EALREADY``, ``EWOULDBLOCK``, and ``EINPROGRESS``.

*/
class BlockingIOError : SystemError {
  proc BlockingIOError(err: syserr, details: string = "") {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``ChildProcessError`` is the subclass of ``SystemError`` corresponding to
   ``ECHILD``.

*/
class ChildProcessError : SystemError {
  proc ChildProcessError(details: string = "", err: syserr = ECHILD) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``ConnectionError`` is the subclass of ``SystemError`` that serves as the
   base class for all connection-based system errors.

*/
class ConnectionError : SystemError { }

/*

   ``BrokenPipeError`` is the subclass of ``ConnectionError`` corresponding
   to ``EPIPE`` and ``ESHUTDOWN``.

*/
class BrokenPipeError : ConnectionError {
  proc BrokenPipeError(err: syserr, details: string = "") {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``ConnectionAbortedError`` is the subclass of ``ConnectionError``
   corresponding to ``ECONNABORTED``.

*/
class ConnectionAbortedError : ConnectionError {
  proc ConnectionAbortedError(details: string = "", err: syserr = ECONNABORTED) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``ConnectionRefusedError`` is the subclass of ``ConnectionError``
   corresponding to ``ECONNREFUSED``.

*/
class ConnectionRefusedError : ConnectionError {
  proc ConnectionRefusedError(details: string = "", err: syserr = ECONNREFUSED) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``ConnectionResetError`` is the subclass of ``ConnectionError``
   corresponding to ``ECONNRESET``.

*/
class ConnectionResetError : ConnectionError {
  proc ConnectionResetError(details: string = "", err: syserr = ECONNRESET) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``FileExistsError`` is the subclass of ``SystemError`` corresponding to
   ``EEXIST``.

*/
class FileExistsError : SystemError {
  proc FileExistsError(details: string = "", err: syserr = EEXIST) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``FileNotFoundError`` is the subclass of ``SystemError`` corresponding to
   ``ENOENT``.

*/
class FileNotFoundError : SystemError {
  proc FileNotFoundError(details: string = "", err: syserr = ENOENT) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``InterruptedError`` is the subclass of ``SystemError`` corresponding to
   ``EINTR``.

*/
class InterruptedError : SystemError {
  proc InterruptedError(details: string = "", err: syserr = EINTR) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``IsADirectoryError`` is the subclass of ``SystemError`` corresponding to
   ``EISDIR``.

*/
class IsADirectoryError : SystemError {
  proc IsADirectoryError(details: string = "", err: syserr = EISDIR) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``NotADirectoryError`` is the subclass of ``SystemError`` corresponding to
   ``ENOTDIR``.

*/
class NotADirectoryError : SystemError {
  proc NotADirectoryError(details: string = "", err: syserr = ENOTDIR) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``PermissionError`` is the subclass of ``SystemError`` corresponding to
   ``EACCES`` and ``EPERM``.

*/
class PermissionError : SystemError {
  proc PermissionError(err: syserr, details: string = "") {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``ProcessLookupError`` is the subclass of ``SystemError`` corresponding to
   ``ESRCH``.

*/
class ProcessLookupError : SystemError {
  proc ProcessLookupError(details: string = "", err: syserr = ESRCH) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``TimeoutError`` is the subclass of ``SystemError`` corresponding to
   ``ETIMEDOUT``.

*/
class TimeoutError : SystemError {
  proc TimeoutError(details: string = "", err: syserr = ETIMEDOUT) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``EOFError`` is the subclass of ``SystemError`` corresponding to
   ``EEOF``.

*/
class EOFError : SystemError {
  proc EOFError(details: string = "", err: syserr = EEOF) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``UnexpectedEOFError`` is the subclass of ``SystemError`` corresponding to
   ``ESHORT``.

*/
class UnexpectedEOFError : SystemError {
  proc UnexpectedEOFError(details: string = "", err: syserr = ESHORT) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``BadFormatError`` is the subclass of ``SystemError`` corresponding to
   ``EFORMAT``.

*/
class BadFormatError : SystemError {
  proc BadFormatError(details: string = "", err: syserr = EFORMAT) {
    this.err     = err;
    this.details = details;
  }
}

/*

   ``InvalidArgumentError`` is the subclass of ``SystemError`` corresponding to
   ``EINVAL``.

*/
class InvalidArgumentError : SystemError {
  proc InvalidArgumentError(details: string = "", err: syserr = EINVAL) {
    this.err     = err;
    this.details = details;
  }
}

// here's what we need from Sys
private extern proc sys_strerror_syserr_str(error:syserr, out err_in_strerror:err_t):c_string;

/* This function takes in a string and returns it in double-quotes,
   with internal double-quotes escaped with backslash.
   */
pragma "no doc"
proc quote_string(s:string, len:ssize_t) {
  extern const QIO_STRING_FORMAT_CHPL: uint(8);
  extern proc qio_quote_string(s:uint(8), e:uint(8), f:uint(8),
                               ptr: c_string, len:ssize_t,
                               ref ret:c_string, ti: c_void_ptr): syserr;
  extern proc qio_strdup(s: c_string): c_string;

  var ret: c_string;
  // 34 is ASCII double quote
  var err: syserr = qio_quote_string(34:uint(8), 34:uint(8),
                                    QIO_STRING_FORMAT_CHPL,
                                    s.localize().c_str(), len, ret, c_nil);

  // This doesn't handle the case where ret==NULL as did the previous
  // version in QIO, but I'm not sure how that was used.

  if err then return qio_strdup("<error>");
  return ret;
}

/* Halt with a useful message if there was an error. Do nothing if the error
   argument does not indicate an error occurred. The error message printed
   when halting will describe the error passed and msg will be appended to it.

   :arg error: the error object
   :arg msg: extra information to print after the error description
 */
proc ioerror(error:syserr, msg:string) throws
{
  if error then throw SystemError.fromSyserr(error, msg);
}

/* Halt with a useful message if there was an error. Do nothing if the error
   argument does not indicate an error occurred. The error message printed
   when halting will describe the error passed and msg will be appended to it,
   along with the path related to the error (for example, the path to a file
   which could not be opened).

   :arg error: the error object
   :arg msg: extra information to print after the error description
   :arg path: a path to print out that is related to the error
 */
proc ioerror(error:syserr, msg:string, path:string) throws
{
  if error {
    const quotedpath = quote_string(path, path.length:ssize_t): string;
    var   details    = msg + " with path " + quotedpath;
    throw SystemError.fromSyserr(error, details);
  }
}

/* Halt with a useful message if there was an error. Do nothing if the error
   argument does not indicate an error occurred. The error message printed
   when halting will describe the error passed and msg will be appended to it,
   along with the path and file offset related to the error. For example, this
   routine might indicate a file format error at a particular location.

   :arg error: the error object
   :arg msg: extra information to print after the error description
   :arg path: a path to print out that is related to the error
   :arg offset: an offset to print out that is related to the error
 */
proc ioerror(error:syserr, msg:string, path:string, offset:int(64)) throws
{
  if error {
    const quotedpath = quote_string(path, path.length:ssize_t): string;
    var   details    = msg + " with path " + quotedpath + " offset " + offset:string;
    throw SystemError.fromSyserr(error, details);
  }
}

/* Halt with a useful message. Instead of an error argument, this routine takes
   in an error string to report.
   The error message printed when halting will describe the error passed and
   msg will be appended to it, along with the path and file offset related to
   the error. For example, this routine might indicate a file format error at a
   particular location.

   This routine .

   :arg errstr: the error string
   :arg msg: extra information to print after the error description
   :arg path: a path to print out that is related to the error
   :arg offset: an offset to print out that is related to the error
 */
proc ioerror(errstr:string, msg:string, path:string, offset:int(64)) throws
{
  const quotedpath = quote_string(path, path.length:ssize_t): string;
  const details    = errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string;
  throw SystemError.fromSyserr(EIO:syserr, details);
}

/* Convert a syserr error code to a human-readable string describing that
   error.

   :arg errstr: the error string
   :returns: a string describing the error
 */
proc errorToString(error:syserr):string
{
  var errstr:c_string = "unknown"; // Why initialize this?
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_syserr_str(error, strerror_err);
  return errstr;
}

}
