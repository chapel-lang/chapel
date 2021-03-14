use SysBasic;

proc doDebugWrite(x, y):err_t {
  extern proc qio_int_to_err(a:int(32)):syserr;

  writeln("Debug Write: ", x, y);
  return qio_int_to_err(1);
}

proc test(arg:string, out error:err_t):bool {
  error = ENOERR;
  on Locales[0] {
    if ! error {
      error = doDebugWrite("test ", arg);
    }
  }
  return error==ENOERR;
}

var e:err_t = ENOERR;
test("hello", e);
