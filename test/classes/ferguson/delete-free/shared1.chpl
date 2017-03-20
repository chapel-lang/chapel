use SharedObject;

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
  var x = new Shared(new Impl(1));
  writeln(x.borrow());

  var c:Impl = nil;
  var y = new Shared(c);
  writeln(y.borrow());
}

run();
