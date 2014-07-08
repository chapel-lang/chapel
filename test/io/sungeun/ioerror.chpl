// Test IO error function interfaces
use IO;

config const testError = 0;

select testError {
  when 0 do ioerror(ENOMEM:syserr, "ENOMEM");
  when 1 do ioerror(EINVAL:syserr, "EINVAL", "this/is/my/path");
  when 2 do ioerror(EPERM:syserr, "EPERM", "this/is/my/path", -1);
  when 3 do ioerror(errorToString(EACCES:syserr),
                    "EACCES", "this/is/my/path", -1);
  when 4 {
    writeln(errorToString(ENOERR:syserr));
    ioerror(ENOERR:syserr, "This is a test");
    ioerror(ENOERR:syserr, "This is a test", "this/is/my/path");
    ioerror(ENOERR:syserr, "This is a test", "this/is/my/path", -1);
  }
}

