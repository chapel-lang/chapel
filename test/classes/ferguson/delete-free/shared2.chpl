

class Impl {
  var field: int;
  proc init(arg:int) {
    field = arg;
    writeln("in Impl.init");
  }

  proc deinit() {
    writeln("in Impl.~Impl");
  }
}

proc run() {
  var x = new shared Impl(1);

  // check initCopy
  var y = x;

  // increment one of their fields so
  // we know they refer to the same data
  x.field += 1;

  writeln(x.borrow());
  writeln(y.borrow());
}

run();
