module SysBasic {
  /* BASIC TYPES */
  _extern type c_uint = uint(32);
  _extern type c_int = int(32);
  _extern type c_long = int(64);
  _extern type c_ulong = uint(64);
  _extern type c_longlong = int(64);
  _extern type c_ulonglong = uint(64);
  _extern type c_double = real(64);
  _extern type c_wchar_t = uint(32);
  _extern type c_ptr; // opaque; no ptr arithmetic in Chapel code!
  _extern type ssize_t = int(64);
  _extern type size_t = uint(64);
  _extern type off_t = int(64);
  _extern type mode_t = uint(32);
  _extern type socklen_t = int(32);

  // system error number.
  _extern type err_t = c_int;

  // system file descriptor.
  _extern type fd_t = c_int;

  // NULL
  _extern const c_nil:c_ptr;

  // error numbers

  // end of file
  _extern const EEOF:err_t;
  _extern const ESHORT:err_t;
  _extern const EFORMAT:err_t;

  // system error numbers
  _extern const E2BIG:err_t;
  _extern const EACCES:err_t;
  _extern const EADDRINUSE:err_t;
  _extern const EADDRNOTAVAIL:err_t;
  _extern const EAFNOSUPPORT:err_t;
  _extern const EAGAIN:err_t;
  _extern const EALREADY:err_t;
  _extern const EBADE:err_t;
  _extern const EBADF:err_t;
  _extern const EBADFD:err_t;
  _extern const EBADMSG:err_t;
  _extern const EBADR:err_t;
  _extern const EBADRQC:err_t;
  _extern const EBADSLT:err_t;
  _extern const EBUSY:err_t;
  _extern const ECANCELED:err_t;
  _extern const ECHILD:err_t;
  _extern const ECHRNG:err_t;
  _extern const ECOMM:err_t;
  _extern const ECONNABORTED:err_t;
  _extern const ECONNREFUSED:err_t;
  _extern const ECONNRESET:err_t;
  _extern const EDEADLK:err_t;
  _extern const EDESTADDRREQ:err_t;
  _extern const EDOM:err_t;
  _extern const EDQUOT:err_t;
  _extern const EEXIST:err_t;
  _extern const EFAULT:err_t;
  _extern const EFBIG:err_t;
  _extern const EHOSTDOWN:err_t;
  _extern const EHOSTUNREACH:err_t;
  _extern const EIDRM:err_t;
  _extern const EILSEQ:err_t;
  _extern const EINPROGRESS:err_t;
  _extern const EINTR:err_t;
  _extern const EINVAL:err_t;
  _extern const EIO:err_t;
  _extern const EISCONN:err_t;
  _extern const EISDIR:err_t;
  _extern const EISNAM:err_t;
  _extern const EKEYEXPIRED:err_t;
  _extern const EKEYREJECTED:err_t;
  _extern const EKEYREVOKED:err_t;
  _extern const EL2HLT:err_t;
  _extern const EL2NSYNC:err_t;
  _extern const EL3HLT:err_t;
  _extern const EL3RST:err_t;
  _extern const ELIBACC:err_t;
  _extern const ELIBBAD:err_t;
  _extern const ELIBMAX:err_t;
  _extern const ELIBSCN:err_t;
  _extern const ELIBEXEC:err_t;
  _extern const ELOOP:err_t;
  _extern const EMEDIUMTYPE:err_t;
  _extern const EMFILE:err_t;
  _extern const EMLINK:err_t;
  _extern const EMSGSIZE:err_t;
  _extern const EMULTIHOP:err_t;
  _extern const ENAMETOOLONG:err_t;
  _extern const ENETDOWN:err_t;
  _extern const ENETRESET:err_t;
  _extern const ENETUNREACH:err_t;
  _extern const ENFILE:err_t;
  _extern const ENOBUFS:err_t;
  _extern const ENODATA:err_t;
  _extern const ENODEV:err_t;
  _extern const ENOENT:err_t;
  _extern const ENOEXEC:err_t;
  _extern const ENOKEY:err_t;
  _extern const ENOLCK:err_t;
  _extern const ENOLINK:err_t;
  _extern const ENOMEDIUM:err_t;
  _extern const ENOMEM:err_t;
  _extern const ENOMSG:err_t;
  _extern const ENONET:err_t;
  _extern const ENOPKG:err_t;
  _extern const ENOPROTOOPT:err_t;
  _extern const ENOSPC:err_t;
  _extern const ENOSR:err_t;
  _extern const ENOSTR:err_t;
  _extern const ENOSYS:err_t;
  _extern const ENOTBLK:err_t;
  _extern const ENOTCONN:err_t;
  _extern const ENOTDIR:err_t;
  _extern const ENOTEMPTY:err_t;
  _extern const ENOTSOCK:err_t;
  _extern const ENOTSUP:err_t;
  _extern const ENOTTY:err_t;
  _extern const ENOTUNIQ:err_t;
  _extern const ENXIO:err_t;
  _extern const EOPNOTSUPP:err_t;
  _extern const EOVERFLOW:err_t;
  _extern const EPERM:err_t;
  _extern const EPFNOSUPPORT:err_t;
  _extern const EPIPE:err_t;
  _extern const EPROTO:err_t;
  _extern const EPROTONOSUPPORT:err_t;
  _extern const EPROTOTYPE:err_t;
  _extern const ERANGE:err_t;
  _extern const EREMCHG:err_t;
  _extern const EREMOTE:err_t;
  _extern const EREMOTEIO:err_t;
  _extern const ERESTART:err_t;
  _extern const EROFS:err_t;
  _extern const ESHUTDOWN:err_t;
  _extern const ESPIPE:err_t;
  _extern const ESOCKTNOSUPPORT:err_t;
  _extern const ESRCH:err_t;
  _extern const ESTALE:err_t;
  _extern const ESTRPIPE:err_t;
  _extern const ETIME:err_t;
  _extern const ETIMEDOUT:err_t;
  _extern const ETXTBSY:err_t;
  _extern const EUCLEAN:err_t;
  _extern const EUNATCH:err_t;
  _extern const EUSERS:err_t;
  _extern const EWOULDBLOCK:err_t;
  _extern const EXDEV:err_t;
  _extern const EXFULL:err_t;

}

