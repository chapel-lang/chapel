use SysBasic;

// here's what we need from Sys
extern proc sys_strerror_str(error:err_t, inout err_in_strerror:err_t):string;

proc ioerror(syserr:err_t, msg:string)
{
  var errstr:string;
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_str(syserr, strerror_err); 
  __primitive("chpl_error", "Unhandled system error: " + errstr + " (" + (syserr:int(32)):string + ") " + msg);
}

proc ioerror(syserr:err_t, msg:string, path:string)
{
  if( syserr ) {
    var errstr:string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_str(syserr, strerror_err); 
    __primitive("chpl_error", "Unhandled system error: " + errstr + " (" + (syserr:int(32)):string + ") " + msg + " with path " + path);
  }
}

proc ioerror(syserr:err_t, msg:string, path:string, offset:int(64))
{
  var errstr:string;
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_str(syserr, strerror_err); 
  __primitive("chpl_error", "Unhandled system error: " + errstr + " (" + (syserr:int(32)):string + ") " + msg + " with path " + path + " offset " + offset:string);
}


proc errorToString(syserr:err_t):string
{
  var errstr:string = "unknown";
  var strerror_err:err_t = ENOERR;
  errstr = sys_strerror_str(syserr, strerror_err); 
  return errstr;
}
