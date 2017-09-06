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

class SystemError : Error {
  var err: syserr;

  proc SystemError(err: syserr, msg: string) {
    this.err = err;
    this.msg = msg;
  }

  proc writeThis(f) {
    try! f.write(msg);
  }

  proc type fromSyserr(err: syserr, msg: string = "") {
    var strerror_err: err_t = ENOERR;
    var errstr : c_string = sys_strerror_syserr_str(err, strerror_err);
    var err_msg: string   = errstr:string;
    if !msg.isEmptyString() then
      err_msg = errstr:string + " '" + msg + "'";

    if err == EAGAIN || err == EALREADY || err == EWOULDBLOCK || err == EINPROGRESS {
      return new BlockingIOError(err, err_msg);
    } else if err == ECHILD {
      return new ChildProcessError(err_msg);
    } else if err == ENOENT {
      return new FileNotFoundError(err_msg);
    } else if err == EPIPE || err == ESHUTDOWN {
      return new BrokenPipeError(err, err_msg);
    } else if err == ECONNABORTED {
      return new ConnectionAbortedError(err_msg);
    } else if err == ECONNREFUSED {
      return new ConnectionRefusedError(err_msg);
    } else if err == ECONNRESET {
      return new ConnectionResetError(err_msg);
    } else if err == EEXIST {
      return new FileExistsError(err_msg);
    } else if err == ENOENT {
      return new FileNotFoundError(err_msg);
    } else if err == EINTR {
      return new InterruptedError(err_msg);
    } else if err == EISDIR {
      return new IsADirectoryError(err_msg);
    } else if err == ENOTDIR {
      return new NotADirectoryError(err_msg);
    } else if err == EACCES || err == EPERM {
      return new PermissionError(err, err_msg);
    } else if err == ESRCH {
      return new ProcessLookupError(err_msg);
    } else if err == ETIMEDOUT {
      return new TimeoutError(err_msg);
    }

    return new SystemError(err, err_msg);
  }

  proc type fromSyserr(err: int, msg: string) {
    return fromSyserr(err:syserr, msg);
  }
}

class BlockingIOError : SystemError {
  /*
  proc BlockingIOError(err: syserr, msg: string) where err == EAGAIN ||
      err == EALREADY || err == EWOULDBLOCK || err == EINPROGRESS {
      */
  proc BlockingIOError(err: syserr, msg: string) {
    this.err = err;
    this.msg = msg;
  }
}

class ChildProcessError : SystemError {
  proc ChildProcessError(msg: string) {
    this.err = ECHILD;
    this.msg = msg;
  }
}

class ConnectionError : SystemError { }

class BrokenPipeError : ConnectionError {
  /*
  proc BrokenPipeError(err: syserr, msg: string) where
      err == EPIPE || err == ESHUTDOWN {
      */
  proc BrokenPipeError(err: syserr, msg: string) {
    this.err = err;
    this.msg = msg;
  }
}

class ConnectionAbortedError : ConnectionError {
  proc ConnectionAbortedError(msg: string) {
    this.err = ECONNABORTED;
    this.msg = msg;
  }
}

class ConnectionRefusedError : ConnectionError {
  proc ConnectionRefusedError(msg: string) {
    this.err = ECONNREFUSED;
    this.msg = msg;
  }
}

class ConnectionResetError : ConnectionError {
  proc ConnectionResetError(msg: string) {
    this.err = ECONNRESET;
    this.msg = msg;
  }
}

class FileExistsError : SystemError {
  proc FileExistsError(msg: string) {
    this.err = EEXIST;
    this.msg = msg;
  }
}

class FileNotFoundError : SystemError {
  proc FileNotFoundError(msg: string) {
    this.err = ENOENT;
    this.msg = msg;
  }
}

class InterruptedError : SystemError {
  proc InterruptedError(msg: string) {
    this.err = EINTR;
    this.msg = msg;
  }
}

class IsADirectoryError : SystemError {
  proc IsADirectoryError(msg: string) {
    this.err = EISDIR;
    this.msg = msg;
  }
}

class NotADirectoryError : SystemError {
  proc NotADirectoryError(msg: string) {
    this.err = ENOTDIR;
    this.msg = msg;
  }
}

class PermissionError : SystemError {
  /*
  proc PermissionError(err:syserr, msg: string) where err == EACCES ||
      err == EPERM {
   */
  proc PermissionError(err:syserr, msg: string) {
    this.err = err;
    this.msg = msg;
  }
}

class ProcessLookupError : SystemError {
  proc ProcessLookupError(msg: string) {
    this.err = ESRCH;
    this.msg = msg;
  }
}

class TimeoutError : SystemError {
  proc TimeoutError(msg: string) {
    this.err = ETIMEDOUT;
    this.msg = msg;
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
    var   msg_path   = msg + " with path " + quotedpath;
    throw SystemError.fromSyserr(error, msg_path);
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
    const quotedpath      = quote_string(path, path.length:ssize_t): string;
    var   msg_path_offset = msg + " with path " + quotedpath + " offset " + offset:string;
    throw SystemError.fromSyserr(error, msg_path_offset);
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
  const err_msg    = errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string;
  throw SystemError.fromSyserr(EIO:syserr, err_msg);
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
