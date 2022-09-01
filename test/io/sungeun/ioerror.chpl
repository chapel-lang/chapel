// Test IO error function interfaces
use IO, OS.POSIX; import SysBasic.{ENOERR};

config const testError = 0;

select testError {
  when 0 do ioerror(ENOMEM:errorCode, "ENOMEM");
  when 1 do ioerror(EINVAL:errorCode, "EINVAL", "this/is/my/path");
  when 2 do ioerror(EPERM:errorCode, "EPERM", "this/is/my/path", -1);
  when 3 do ioerror(errorToString(EACCES:errorCode),
                    "EACCES", "this/is/my/path", -1);
  when 4 {
    writeln(errorToString(ENOERR:errorCode));
    ioerror(ENOERR:errorCode, "This is a test");
    ioerror(ENOERR:errorCode, "This is a test", "this/is/my/path");
    ioerror(ENOERR:errorCode, "This is a test", "this/is/my/path", -1);
  }
}

