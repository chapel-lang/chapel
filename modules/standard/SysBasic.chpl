
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
extern type err_t; // = c_int, opaque so we can manually override ==,!=,etc

// system file descriptor.
extern type fd_t = c_int;

// NULL
extern const c_nil:c_ptr;
extern proc is_c_nil(x):c_int;

// error numbers

extern proc chpl_err_eq(a:err_t, b:err_t):c_int;
extern proc chpl_err_to_int(a:err_t):int(32);
extern proc chpl_int_to_err(a:int(32)):err_t;

inline proc ENOERR return chpl_int_to_err(0); 

inline proc ==(a: err_t, b: err_t) return (chpl_err_eq(a,b) != 0);
inline proc !=(a: err_t, b: err_t) return !(a == b);
inline proc !(a: err_t) return chpl_err_to_int(a) == 0;
inline proc _cond_test(a: err_t) return chpl_err_to_int(a) != 0;
inline proc _cast(type t, x: err_t) where t == int(32)
  return chpl_err_to_int(x);
inline proc _cast(type t, x: int(32)) where t == err_t
  return chpl_int_to_err(x);

// end of file
extern const EEOF:err_t;
extern const ESHORT:err_t;
extern const EFORMAT:err_t;

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


