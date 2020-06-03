

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
  writeln(x.borrow());

  var c:unmanaged Impl? = nil;
  var y = shared.create(c);
  writeln(y.borrow());
}

run();
