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

use SysBasic;

// here's what we need from Sys
extern proc sys_strerror_syserr_str(error:syserr, out err_in_strerror:err_t):c_string;

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
                                    s.c_str(), len, ret, c_nil);

  // This doesn't handle the case where ret==NULL as did the previous
  // version in QIO, but I'm not sure how that was used.

  if err then return qio_strdup("<error>");
  return ret;
}

proc ioerror(error:syserr, msg:string)
{
  if( error ) {
    var errstr:c_string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_syserr_str(error, strerror_err);
    const err_msg: string = errstr + " " + msg;
    __primitive("chpl_error", msg.c_str());
  }
}

proc ioerror(error:syserr, msg:string, path:string)
{
  if( error ) {
    var errstr:c_string;
    var quotedpath:c_string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_syserr_str(error, strerror_err);
    quotedpath = quote_string(path, path.length:ssize_t);
    const err_msg: string = errstr + " " + msg + " with path " + quotedpath;
    __primitive("chpl_error", msg.c_str());
  }
}

proc ioerror(error:syserr, msg:string, path:string, offset:int(64))
{
  if( error ) {
    var errstr:c_string;
    var quotedpath:c_string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_syserr_str(error, strerror_err);
    quotedpath = quote_string(path, path.length:ssize_t);
    const err_msg: string = errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string;
    __primitive("chpl_error", msg.c_str());
  }
}

proc ioerror(errstr:string, msg:string, path:string, offset:int(64))
{
  var quotedpath:c_string;
  quotedpath = quote_string(path, path.length:ssize_t);
  const err_msg = errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string;
  __primitive("chpl_error", err_msg.c_str());
}

proc errorToString(error:syserr):string
{
  var errstr:c_string = "unknown"; // Why initialize this?
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_syserr_str(error, strerror_err); 
  return errstr;
}
