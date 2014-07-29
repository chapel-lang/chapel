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
    __primitive("chpl_error", errstr + " " + msg.c_str());
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
    __primitive("chpl_error", errstr + " " + msg.c_str() + " with path " + quotedpath);
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
    __primitive("chpl_error", errstr + " " + msg.c_str() + " with path " + quotedpath + " offset " + offset:c_string);
  }
}

proc ioerror(errstr:string, msg:string, path:string, offset:int(64))
{
  var quotedpath:c_string;
  quotedpath = quote_string(path, path.length:ssize_t);
  __primitive("chpl_error", errstr + " " + msg.c_str() + " with path " + quotedpath + " offset " + offset:c_string);
}

proc errorToString(error:syserr):string
{
  var errstr:c_string = "unknown"; // Why initialize this?
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_syserr_str(error, strerror_err); 
  return errstr;
}
