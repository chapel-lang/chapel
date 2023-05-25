use CTypes;
import OS.{errorCode};

proc doDebugWrite(x, y):c_int {
  extern proc qio_int_to_err(a:int(32)):errorCode;

  writeln("Debug Write: ", x, y);
  return qio_int_to_err(1);
}

proc test(arg:string, out error:c_int):bool {
  error = 0;
  on Locales[0] {
    if ! error {
      error = doDebugWrite("test ", arg);
    }
  }
  return error==0;
}

var e:c_int = 0;
test("hello", e);
