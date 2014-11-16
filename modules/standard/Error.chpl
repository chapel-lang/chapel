/*
 * Copyright 2004-2014 Cray Inc.
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
    var errstr_s =  errstr + " ";
    var errstr_s_m = errstr_s + msg.c_str();
    __primitive("chpl_error", errstr_s_m);
    chpl_free_c_string_copy(errstr_s_m);
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
    // Intermediate concatenations consume their c_string_copy args.
    var errstr_s = errstr + " ";
    var errstr_s_m = errstr_s + msg.c_str();
    var errstr_s_m_w = errstr_s_m + " with path ";
    var errstr_s_m_w_q = errstr_s_m_w + quotedpath;
    __primitive("chpl_error", errstr_s_m_w_q);
    // But the last one must be freed explicitly.
    chpl_free_c_string_copy(errstr_s_m_w_q);
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
    // Intermediate concatenations consume their c_string_copy args.
    var errstr_s = errstr + " ";
    var errstr_s_m = errstr_s + msg.c_str();
    var errstr_s_m_w = errstr_s_m + " with path ";
    var errstr_s_m_w_q = errstr_s_m_w + quotedpath;
    var errstr_s_m_w_q_o = errstr_s_m_w_q + " offset ";
    var offset_cstr = offset:c_string_copy;
    var errstr_s_m_w_q_o_o = errstr_s_m_w_q_o + offset_cstr;
    __primitive("chpl_error", errstr_s_m_w_q_o_o);
    // But the last one must be freed explicitly.
    chpl_free_c_string_copy(errstr_s_m_w_q_o_o);
  }
}

proc ioerror(errstr:string, msg:string, path:string, offset:int(64))
{
  var quotedpath:c_string;
  quotedpath = quote_string(path, path.length:ssize_t);
  // TODO: Because the output of concatenation (+) is an allocated string,
  // this routine leaks like a sieve.
  __primitive("chpl_error", errstr + " " + msg.c_str() + " with path " + quotedpath + " offset " + offset:c_string_copy);
}

proc errorToString(error:syserr):string
{
  var errstr:c_string = "unknown"; // Why initialize this?
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_syserr_str(error, strerror_err); 
  return errstr;
}
