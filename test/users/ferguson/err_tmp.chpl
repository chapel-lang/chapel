use SysBasic;

proc doDebugWrite(x, y):c_int {
  extern proc qio_int_to_err(a:int(32)):syserr;

  writeln("Debug Write: ", x, y);
  return qio_int_to_err(1);
}

proc test(arg:string, out error:c_int):bool {
  error = ENOERR;
  on Locales[0] {
    if ! error {
      error = doDebugWrite("test ", arg);
    }
  }
  return error==ENOERR;
}

var e:c_int = ENOERR;
test("hello", e);
