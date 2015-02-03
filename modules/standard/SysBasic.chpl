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


/* BASIC TYPES */
use SysCTypes;

extern type c_float = real(32);
extern type c_double = real(64);
/*extern type c_wchar_t = uint(32); */
extern type off_t = int(64);
extern type mode_t = uint(32);
extern type socklen_t = int(32);

// C File type
//type c_file = _file;

// stdin/stdout/sterr
//extern proc chpl_cstdin():_file;
extern proc chpl_cstdout():_file;
extern proc chpl_cstderr():_file;
extern proc chpl_cnullfile():_file;

// system error number.
extern type syserr; // = c_int, opaque so we can manually override ==,!=,etc
extern type err_t = c_int;
// When to use syserr and when to use err_t?
// err_t is a system error code (a c_int by a nicer name).
// syserr is an enhanced error that might also include an
// error message. All user-facing Chapel library code,
// or user Chapel code, should generally use syserr.
// When wrapping functions or declaring them in C,
// use err_t to indicate that a function is returning
// an error code.

// system file descriptor.
extern type fd_t = c_int;

// NULL
extern const c_nil:c_void_ptr;
// To generate legal C prototypes, we have to manually instantiate this
// prototype for each pointer type that might be associated with 'x'.
pragma "no prototype"
extern proc is_c_nil(x):bool;

// local C pointer class used in C integration.
// Similar to _ddata from ChapelBase, but differs 
// from _ddata because it can never be wide.
pragma "data class"
pragma "no object"
pragma "no default functions"
pragma "no wide class"
class c_ptr {
  type eltType;
  inline proc this(i: integral) ref {
    return __primitive("array_get", this, i);
  }
  inline proc deref() ref {
    return __primitive("array_get", this, 0);
  }
}

inline proc =(ref a:c_ptr, b:c_ptr) { __primitive("=", a, b); }

inline proc =(ref a:c_ptr, b:_nilType) { __primitive("=", a, c_nil); }

inline proc _cast(type t, x) where t:c_ptr && x:_nilType {
  return __primitive("cast", t, x);
}
inline proc _cast(type t, x) where t:c_ptr && x.type:c_ptr {
  return __primitive("cast", t, x);
}
inline proc _cast(type t, x) where t:c_void_ptr && x.type:c_ptr {
  return __primitive("cast", t, x);
}
inline proc _cast(type t, x) where t:c_ptr && x.type:c_void_ptr {
  return __primitive("cast", t, x);
}


inline proc c_calloc(type eltType, size: integral) {
  var ret:c_ptr(eltType);
  __primitive("array_alloc", ret, eltType, size);
  init_elts(ret, size, eltType);
  return ret;
}

inline proc c_free(data: c_ptr) {
  __primitive("array_free", data);
}

inline proc ==(a: c_ptr, b: c_ptr) where a.eltType == b.eltType {
  return __primitive("ptr_eq", a, b);
}
inline proc ==(a: c_ptr, b: c_void_ptr) {
  return __primitive("ptr_eq", a, b);
}
inline proc ==(a: c_void_ptr, b: c_ptr) {
  return __primitive("ptr_eq", a, b);
}
inline proc ==(a: c_ptr, b: _nilType) {
  return __primitive("ptr_eq", a, c_nil);
}
inline proc ==(a: _nilType, b: c_ptr) {
  return __primitive("ptr_eq", c_nil, b);
}

inline proc !=(a: c_ptr, b: c_ptr) where a.eltType == b.eltType {
  return __primitive("ptr_neq", a, b);
}
inline proc !=(a: c_ptr, b: c_void_ptr) {
  return __primitive("ptr_neq", a, b);
}
inline proc !=(a: c_void_ptr, b: c_ptr) {
  return __primitive("ptr_neq", a, b);
}
inline proc !=(a: c_ptr, b: _nilType) {
  return __primitive("ptr_neq", a, c_nil);
}
inline proc !=(a: _nilType, b: c_ptr) {
  return __primitive("ptr_neq", c_nil, b);
}

inline proc _cond_test(x: c_ptr) return x != c_nil;

inline proc !(x: c_ptr) return x == c_nil;
extern proc c_pointer_return(ref x:?t):c_ptr(t);

inline proc c_ptrTo(arr: []) where isRectangularArr(arr) {
  return c_pointer_return(arr[arr.domain.low]);
}
inline proc c_ptrTo(ref x:?t):c_ptr(t)
{
  if isArrayType(t) then
    compilerError("c_ptrTo unsupported array type", 2);
  if isDomainType(t) then
    compilerError("c_ptrTo domain type not supported", 2);
  // Other cases should be avoided, e.g. sync vars
  return c_pointer_return(x);
}

// error numbers

extern proc qio_err_eq(a:syserr, b:syserr):c_int;
extern proc qio_err_to_int(a:syserr):int(32);
extern proc qio_int_to_err(a:int(32)):syserr;
extern proc qio_err_iserr(a:syserr):c_int;

// No error occurred
inline proc ENOERR return 0:err_t;

// When err_t is no longer just int(32), will need to add cases for err_t too.
inline proc ==(a: syserr, b: syserr) return (qio_err_eq(a,b) != 0:c_int);
inline proc ==(a: syserr, b: int(32)) return (qio_err_to_int(a) == b:int(32));
inline proc ==(a: syserr, b: int(64)) return (qio_err_to_int(a) == b:int(32));
inline proc ==(a: int(32), b: syserr) return (a:int(32) == qio_err_to_int(b));
inline proc ==(a: int(64), b: syserr) return (a:int(32) == qio_err_to_int(b));
inline proc !=(a: syserr, b: syserr) return !(a == b);
inline proc !=(a: syserr, b: int(32)) return !(a == b);
inline proc !=(a: syserr, b: int(64)) return !(a == b);
inline proc !=(a: int(32), b: syserr) return !(a == b);
inline proc !=(a: int(64), b: syserr) return !(a == b);
inline proc !(a: syserr) return (qio_err_iserr(a) == 0:c_int);
inline proc _cond_test(a: syserr) return (qio_err_iserr(a) != 0:c_int);
inline proc _cast(type t, x: syserr) where t == int(32)
  return qio_err_to_int(x);
inline proc _cast(type t, x: syserr) where t == int(64)
  return qio_err_to_int(x):int(64);
inline proc _cast(type t, x: int(32)) where t == syserr
  return qio_int_to_err(x);
inline proc _cast(type t, x: int(64)) where t == syserr
  return qio_int_to_err(x:int(32));
pragma "trivial assignment"
inline proc =(ref ret:syserr, x:syserr) { __primitive("=", ret, x); }
inline proc =(ref ret:syserr, x:int(32)) 
{ __primitive("=", ret, qio_int_to_err(x)); }
inline proc =(ref ret:syserr, x:int(64)) 
{ __primitive("=", ret, qio_int_to_err(x:int(32))); }
inline proc =(ref ret:err_t, x:syserr) 
{ __primitive("=", ret, qio_err_to_int(x):err_t); }

// end of file
extern proc chpl_macro_int_EEOF():err_t;
inline proc EEOF return chpl_macro_int_EEOF():err_t;
extern proc chpl_macro_int_ESHORT():err_t;
inline proc ESHORT return chpl_macro_int_ESHORT():err_t;
extern proc chpl_macro_int_EFORMAT():err_t;
inline proc EFORMAT return chpl_macro_int_EFORMAT():err_t;

// system error numbers
extern const E2BIG:err_t;
extern const EACCES:err_t;
extern const EADDRINUSE:err_t;
extern const EADDRNOTAVAIL:err_t;
extern const EAFNOSUPPORT:err_t;
extern const EAGAIN:err_t;
extern const EALREADY:err_t;
extern const EBADE:err_t;
extern const EBADF:err_t;
extern const EBADFD:err_t;
extern const EBADMSG:err_t;
extern const EBADR:err_t;
extern const EBADRQC:err_t;
extern const EBADSLT:err_t;
extern const EBUSY:err_t;
extern const ECANCELED:err_t;
extern const ECHILD:err_t;
extern const ECHRNG:err_t;
extern const ECOMM:err_t;
extern const ECONNABORTED:err_t;
extern const ECONNREFUSED:err_t;
extern const ECONNRESET:err_t;
extern const EDEADLK:err_t;
extern const EDESTADDRREQ:err_t;
extern const EDOM:err_t;
extern const EDQUOT:err_t;
extern const EEXIST:err_t;
extern const EFAULT:err_t;
extern const EFBIG:err_t;
extern const EHOSTDOWN:err_t;
extern const EHOSTUNREACH:err_t;
extern const EIDRM:err_t;
extern const EILSEQ:err_t;
extern const EINPROGRESS:err_t;
extern const EINTR:err_t;
extern const EINVAL:err_t;
extern const EIO:err_t;
extern const EISCONN:err_t;
extern const EISDIR:err_t;
extern const EISNAM:err_t;
extern const EKEYEXPIRED:err_t;
extern const EKEYREJECTED:err_t;
extern const EKEYREVOKED:err_t;
extern const EL2HLT:err_t;
extern const EL2NSYNC:err_t;
extern const EL3HLT:err_t;
extern const EL3RST:err_t;
extern const ELIBACC:err_t;
extern const ELIBBAD:err_t;
extern const ELIBMAX:err_t;
extern const ELIBSCN:err_t;
extern const ELIBEXEC:err_t;
extern const ELOOP:err_t;
extern const EMEDIUMTYPE:err_t;
extern const EMFILE:err_t;
extern const EMLINK:err_t;
extern const EMSGSIZE:err_t;
extern const EMULTIHOP:err_t;
extern const ENAMETOOLONG:err_t;
extern const ENETDOWN:err_t;
extern const ENETRESET:err_t;
extern const ENETUNREACH:err_t;
extern const ENFILE:err_t;
extern const ENOBUFS:err_t;
extern const ENODATA:err_t;
extern const ENODEV:err_t;
extern const ENOENT:err_t;
extern const ENOEXEC:err_t;
extern const ENOKEY:err_t;
extern const ENOLCK:err_t;
extern const ENOLINK:err_t;
extern const ENOMEDIUM:err_t;
extern const ENOMEM:err_t;
extern const ENOMSG:err_t;
extern const ENONET:err_t;
extern const ENOPKG:err_t;
extern const ENOPROTOOPT:err_t;
extern const ENOSPC:err_t;
extern const ENOSR:err_t;
extern const ENOSTR:err_t;
extern const ENOSYS:err_t;
extern const ENOTBLK:err_t;
extern const ENOTCONN:err_t;
extern const ENOTDIR:err_t;
extern const ENOTEMPTY:err_t;
extern const ENOTSOCK:err_t;
extern const ENOTSUP:err_t;
extern const ENOTTY:err_t;
extern const ENOTUNIQ:err_t;
extern const ENXIO:err_t;
extern const EOPNOTSUPP:err_t;
extern const EOVERFLOW:err_t;
extern const EPERM:err_t;
extern const EPFNOSUPPORT:err_t;
extern const EPIPE:err_t;
extern const EPROTO:err_t;
extern const EPROTONOSUPPORT:err_t;
extern const EPROTOTYPE:err_t;
extern const ERANGE:err_t;
extern const EREMCHG:err_t;
extern const EREMOTE:err_t;
extern const EREMOTEIO:err_t;
extern const ERESTART:err_t;
extern const EROFS:err_t;
extern const ESHUTDOWN:err_t;
extern const ESPIPE:err_t;
extern const ESOCKTNOSUPPORT:err_t;
extern const ESRCH:err_t;
extern const ESTALE:err_t;
extern const ESTRPIPE:err_t;
extern const ETIME:err_t;
extern const ETIMEDOUT:err_t;
extern const ETXTBSY:err_t;
extern const EUCLEAN:err_t;
extern const EUNATCH:err_t;
extern const EUSERS:err_t;
extern const EWOULDBLOCK:err_t;
extern const EXDEV:err_t;
extern const EXFULL:err_t;


