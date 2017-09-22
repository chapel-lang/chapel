require "err-return.h";

use ExampleErrors;

extern record my_struct {
  var x:c_int;
}

record MyRecord {
  var y:my_struct;
  proc MyRecord(i:int) {
    y.x = i:c_int;
  }
  proc deinit() {
    writeln("deinit ", y.x);
  }
}

config const case = 1;

proc returnOrThrow(i:int, j = 4):MyRecord throws {

  if case == i {
    throw new StringError("test error");
  }
  return new MyRecord(i);
}

proc test() {
  try {
    var x = returnOrThrow(1);
    writeln(x);
    var y = returnOrThrow(2);
    writeln(y);
    var z = returnOrThrow(3);
    writeln(z);
  } catch e {
    writeln(e);
  }
}

test();
