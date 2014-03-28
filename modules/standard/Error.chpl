use SysBasic;

// here's what we need from Sys
extern proc sys_strerror_syserr_str(error:syserr, out err_in_strerror:err_t):string;
// here's what we need from QIO
extern proc qio_quote_string_chpl(ptr:string, len:ssize_t):string;

proc ioerror(error:syserr, msg:string)
{
  var errstr:string;
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_syserr_str(error, strerror_err); 
  __primitive("chpl_error", errstr + " " + msg);
}

proc ioerror(error:syserr, msg:string, path:string)
{
  if( error ) {
    var errstr:string;
    var quotedpath:string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_syserr_str(error, strerror_err); 
    quotedpath = qio_quote_string_chpl(path, path.length:ssize_t);
    __primitive("chpl_error", errstr + " " + msg + " with path " + quotedpath);
  }
}

proc ioerror(error:syserr, msg:string, path:string, offset:int(64))
{
  var errstr:string;
  var quotedpath:string;
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_syserr_str(error, strerror_err); 
  quotedpath = qio_quote_string_chpl(path, path.length:ssize_t);
  __primitive("chpl_error", errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string);
}

proc ioerror(errstr:string, msg:string, path:string, offset:int(64))
{
  var quotedpath:string;
  quotedpath = qio_quote_string_chpl(path, path.length:ssize_t);
  __primitive("chpl_error", errstr + " " + msg + " with path " + quotedpath + " offset " + offset:string);
}

proc errorToString(error:syserr):string
{
  var errstr:string = "unknown";
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_syserr_str(error, strerror_err); 
  return errstr;
}
