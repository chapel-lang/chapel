module Error {
  use SysBasic;
  use Sys;

  class ErrorHandler {
    // override this method to get error handling other
    // than saving the error.
    proc onError(syserr:err_t, path:string, offset:int(64)) {
      var errstr:string;
      var strerror_err:err_t;
      errstr = sys_strerror_str(syserr, strerror_err); 
      if path == "" {
        halt("Unhandled system error ", syserr, " ", errstr);
      } else {
        halt("Unhandled system error ", syserr, " ", errstr,
             " with file ", path, " : ", offset);
      }
    }
  }

  class ErrorSaver : ErrorHandler {
    var syserr:err_t;
    var path:string; // path for file errors
    var offset:int(64); // offset for file errors
    proc onError(syserr:err_t, path:string, offset:int(64)) {
      this.syserr = syserr;
      this.path = path;
      this.offset = offset;
    }
  }

  proc seterr(err:ErrorHandler, syserr:err_t, path="", offset:int(64)=-1)
  {
    if( syserr ) {
      if( err != nil ) {
        err.onError(syserr, path, offset);
      } else {
        var errstr:string;
        var strerror_err:err_t;
        errstr = sys_strerror_str(syserr, strerror_err); 
        if path == "" {
          halt("Unhandled system error ", syserr, " ", errstr);
        } else {
          halt("Unhandled system error ", syserr, " ", errstr,
               " with file ", path, " : ", offset);
        }
      }
    }
  }

}

