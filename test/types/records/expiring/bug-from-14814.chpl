record R {
  var x:int = 0;
  proc init() {
    writeln("init");
  }
  proc deinit() {
    assert(x == 0);
    x = 99;
    writeln("deinit");
  }
}

config const option = true;
config const error = false;

proc throwsAndReturns(): R throws {
  if error then
    throw new Error();

  var ret: R;
  return ret;
}

proc main() {
  if option {
    throwsAndReturns();
  }
}
