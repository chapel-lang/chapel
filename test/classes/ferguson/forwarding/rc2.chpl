use ReferenceCounting;

class Impl {
  var field: int;
  proc init(arg:int) {
    field = arg;
    writeln("in Impl.init");
  }

  proc ~Impl() {
    writeln("in Impl.~Impl");
  }
}

proc run() {
  var x = new RefCounted(new Impl(1));

  // check initCopy
  var y = x;

  writeln(x.p);
  writeln(y.p);
}

run();
