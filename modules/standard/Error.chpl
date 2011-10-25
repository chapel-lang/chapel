  use SysBasic;

  // here's what we need from Sys
  extern proc sys_strerror_str(error:err_t, inout err_in_strerror:err_t):string;

  proc _ioerror(syserr:err_t, msg:string)
  {
    var errstr:string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_str(syserr, strerror_err); 
    __primitive("chpl_error", "Unhandled system error: " + errstr + " (" + (syserr:int(32)):string + ") " + msg);
        //__primitive("chpl_error", "Unhandled system error: " + syserr + " " +
        //            errstr + " with file " + path " : " + offset);
  }
/*
  inline proc ioerror(syserr:err_t, msg:string)
  {
    if syserr then _ioerror(syserr, msg);
  } */

  proc _ioerror(syserr:err_t, msg:string, path:string)
  {
    if( syserr ) {
      var errstr:string;
      var strerror_err:err_t = ENOERR;
      errstr = sys_strerror_str(syserr, strerror_err); 
      __primitive("chpl_error", "Unhandled system error: " + errstr + " (" + (syserr:int(32)):string + ") " + msg + " with path " + path);
          //__primitive("chpl_error", "Unhandled system error: " + syserr + " " +
          //            errstr + " with file " + path " : " + offset);
    }
  }
/*
  inline proc ioerror(syserr:err_t, msg:string, path:string)
  {
    if syserr then _ioerror(syserr, msg, path);
  }*/

  proc _ioerror(syserr:err_t, msg:string, path:string, offset:int(64))
  {
    var errstr:string;
    var strerror_err:err_t = ENOERR;
    errstr = sys_strerror_str(syserr, strerror_err); 
    __primitive("chpl_error", "Unhandled system error: " + errstr + " (" + (syserr:int(32)):string + ") " + msg + " with path " + path + " offset " + offset:string);
        //__primitive("chpl_error", "Unhandled system error: " + syserr + " " +
        //            errstr + " with file " + path " : " + offset);
  }
/*
  inline proc ioerror(syserr:err_t, msg:string, path:string, offset:int(64))
  {
    if syserr then _ioerror(syserr, msg, path, offset);
  }*/


