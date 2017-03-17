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
  var y: Shared(Impl);

  // check assignment
  y = x;

  writeln(x.borrow());
  writeln(y.borrow());


  var z: y.type;
  z = y;

  writeln(x.borrow());
  writeln(y.borrow());
  writeln(z.borrow());
}

run();
