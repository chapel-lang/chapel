/*
 * Copyright 2004-2015 Cray Inc.
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
   
   Support for error handling.

   This module helps to handle errors. In particular, it enables routines
   to return a syserr - encoding an error state - and then contains routines
   that can be provided a syserr in order to print out a useful error message.
   
   This module defines the type syserr, which can encode an error code or an
   error message. This type can be returned from routines generating an error.
   
   The IO module uses these routines in a way that supports error inspection
   and also rapid prototyping. Most routines in the IO module have two forms.
   In one form, an error (of type syserr) is returned in an out error argument.
   In the second form, no error is returned, and instead the task will halt
   with a fatal error if an error is encountered.
   
 */
module Error {

use SysBasic;

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
   argument does not indicate an error occured. The error message printed
   when halting will describe the error passed and msg will be appended to it.

   :arg error: the error object
   :arg msg: extra information to print after the error description
 */
proc ioerror(error:syserr, msg:string)
{
  if( error ) {
    var errstr:c_string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_syserr_str(error, strerror_err);
    const err_msg: string = errstr + " " + msg;
    __primitive("chpl_error", err_msg.c_str());
  }
}

/* Halt with a useful message if there was an error. Do nothing if the error
   argument does not indicate an error occured. The error message printed
   when halting will describe the error passed and msg will be appended to it,
   along with the path related to the error (for example, the path to a file
   which could not be opened).

   :arg error: the error object
   :arg msg: extra information to print after the error description
   :arg path: a path to print out that is related to the error
 */
proc ioerror(error:syserr, msg:string, path:string)
{
  if( error ) {
    var errstr:c_string;
    var quotedpath:c_string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_syserr_str(error, strerror_err);
    quotedpath = quote_string(path, path.length:ssize_t);
    const err_msg: string = errstr + " " + msg + " with path " + quotedpath;
    __primitive("chpl_error", err_msg.c_str());
  }
}

/* Halt with a useful message if there was an error. Do nothing if the error
   argument does not indicate an error occured. The error message printed
   when halting will describe the error passed and msg will be appended to it,
   along with the path and file offset related to the error. For example, this
   routine might indicate a file format error at a particular location.

   :arg error: the error object
   :arg msg: extra information to print after the error description
   :arg path: a path to print out that is related to the error
   :arg offset: an offset to print out that is related to the error
 */
proc ioerror(error:syserr, msg:string, path:string, offset:int(64))
{
  if( error ) {
    var errstr:c_string;
    var quotedpath:c_string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_syserr_str(error, strerror_err);
    quotedpath = quote_string(path, path.length:ssize_t);
    const err_msg: string = errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string;
    __primitive("chpl_error", err_msg.c_str());
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
proc ioerror(errstr:string, msg:string, path:string, offset:int(64))
{
  var quotedpath:c_string;
  quotedpath = quote_string(path, path.length:ssize_t);
  const err_msg = errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string;
  __primitive("chpl_error", err_msg.c_str());
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
