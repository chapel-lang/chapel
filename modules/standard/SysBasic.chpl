
/* BASIC TYPES */
use SysCTypes;

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

// system file descriptor.
extern type fd_t = c_int;

// NULL
extern const c_nil:c_ptr;
// To generate legal C prototypes, we have to manually instantiate this prototype
// for each pointer type that might be associated with 'x'.
pragma "no prototype"
extern proc is_c_nil(x):c_int;

// error numbers

extern proc chpl_err_eq(a:syserr, b:syserr):c_int;
extern proc chpl_err_to_int(a:syserr):int(32);
extern proc chpl_int_to_err(a:int(32)):syserr;

inline proc ENOERR return chpl_int_to_err(0); 

inline proc ==(a: syserr, b: syserr) return (chpl_err_eq(a,b) != 0);
inline proc !=(a: syserr, b: syserr) return !(a == b);
inline proc !(a: syserr) return chpl_err_to_int(a) == 0;
inline proc _cond_test(a: syserr) return chpl_err_to_int(a) != 0;
inline proc _cast(type t, x: syserr) where t == int(32)
  return chpl_err_to_int(x);
inline proc _cast(type t, x: syserr) where t == int(64)
  return chpl_err_to_int(x);
inline proc _cast(type t, x: int(32)) where t == syserr
  return chpl_int_to_err(x);
inline proc _cast(type t, x: int(64)) where t == syserr
  return chpl_int_to_err(x);

// end of file
extern proc chpl_macro_int_EEOF():c_int;
inline proc EEOF return chpl_macro_int_EEOF():syserr;
extern proc chpl_macro_int_ESHORT():c_int;
inline proc ESHORT return chpl_macro_int_ESHORT():syserr;
extern proc chpl_macro_int_EFORMAT():c_int;
inline proc EFORMAT return chpl_macro_int_EFORMAT():syserr;

// system error numbers
extern const E2BIG:syserr;
extern const EACCES:syserr;
extern const EADDRINUSE:syserr;
extern const EADDRNOTAVAIL:syserr;
extern const EAFNOSUPPORT:syserr;
extern const EAGAIN:syserr;
extern const EALREADY:syserr;
extern const EBADE:syserr;
extern const EBADF:syserr;
extern const EBADFD:syserr;
extern const EBADMSG:syserr;
extern const EBADR:syserr;
extern const EBADRQC:syserr;
extern const EBADSLT:syserr;
extern const EBUSY:syserr;
extern const ECANCELED:syserr;
extern const ECHILD:syserr;
extern const ECHRNG:syserr;
extern const ECOMM:syserr;
extern const ECONNABORTED:syserr;
extern const ECONNREFUSED:syserr;
extern const ECONNRESET:syserr;
extern const EDEADLK:syserr;
extern const EDESTADDRREQ:syserr;
extern const EDOM:syserr;
extern const EDQUOT:syserr;
extern const EEXIST:syserr;
extern const EFAULT:syserr;
extern const EFBIG:syserr;
extern const EHOSTDOWN:syserr;
extern const EHOSTUNREACH:syserr;
extern const EIDRM:syserr;
extern const EILSEQ:syserr;
extern const EINPROGRESS:syserr;
extern const EINTR:syserr;
extern const EINVAL:syserr;
extern const EIO:syserr;
extern const EISCONN:syserr;
extern const EISDIR:syserr;
extern const EISNAM:syserr;
extern const EKEYEXPIRED:syserr;
extern const EKEYREJECTED:syserr;
extern const EKEYREVOKED:syserr;
extern const EL2HLT:syserr;
extern const EL2NSYNC:syserr;
extern const EL3HLT:syserr;
extern const EL3RST:syserr;
extern const ELIBACC:syserr;
extern const ELIBBAD:syserr;
extern const ELIBMAX:syserr;
extern const ELIBSCN:syserr;
extern const ELIBEXEC:syserr;
extern const ELOOP:syserr;
extern const EMEDIUMTYPE:syserr;
extern const EMFILE:syserr;
extern const EMLINK:syserr;
extern const EMSGSIZE:syserr;
extern const EMULTIHOP:syserr;
extern const ENAMETOOLONG:syserr;
extern const ENETDOWN:syserr;
extern const ENETRESET:syserr;
extern const ENETUNREACH:syserr;
extern const ENFILE:syserr;
extern const ENOBUFS:syserr;
extern const ENODATA:syserr;
extern const ENODEV:syserr;
extern const ENOENT:syserr;
extern const ENOEXEC:syserr;
extern const ENOKEY:syserr;
extern const ENOLCK:syserr;
extern const ENOLINK:syserr;
extern const ENOMEDIUM:syserr;
extern const ENOMEM:syserr;
extern const ENOMSG:syserr;
extern const ENONET:syserr;
extern const ENOPKG:syserr;
extern const ENOPROTOOPT:syserr;
extern const ENOSPC:syserr;
extern const ENOSR:syserr;
extern const ENOSTR:syserr;
extern const ENOSYS:syserr;
extern const ENOTBLK:syserr;
extern const ENOTCONN:syserr;
extern const ENOTDIR:syserr;
extern const ENOTEMPTY:syserr;
extern const ENOTSOCK:syserr;
extern const ENOTSUP:syserr;
extern const ENOTTY:syserr;
extern const ENOTUNIQ:syserr;
extern const ENXIO:syserr;
extern const EOPNOTSUPP:syserr;
extern const EOVERFLOW:syserr;
extern const EPERM:syserr;
extern const EPFNOSUPPORT:syserr;
extern const EPIPE:syserr;
extern const EPROTO:syserr;
extern const EPROTONOSUPPORT:syserr;
extern const EPROTOTYPE:syserr;
extern const ERANGE:syserr;
extern const EREMCHG:syserr;
extern const EREMOTE:syserr;
extern const EREMOTEIO:syserr;
extern const ERESTART:syserr;
extern const EROFS:syserr;
extern const ESHUTDOWN:syserr;
extern const ESPIPE:syserr;
extern const ESOCKTNOSUPPORT:syserr;
extern const ESRCH:syserr;
extern const ESTALE:syserr;
extern const ESTRPIPE:syserr;
extern const ETIME:syserr;
extern const ETIMEDOUT:syserr;
extern const ETXTBSY:syserr;
extern const EUCLEAN:syserr;
extern const EUNATCH:syserr;
extern const EUSERS:syserr;
extern const EWOULDBLOCK:syserr;
extern const EXDEV:syserr;
extern const EXFULL:syserr;


