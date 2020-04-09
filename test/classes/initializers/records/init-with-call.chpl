class Impl {
  var x:int;
  proc init(arg:int) {
    writeln("In Impl.init");
  }
  proc deinit() {
    writeln("In Impl.deinit");
  }
}

proc makeit() {
  return new shared Impl(1);
}

record R {
  var field;

  proc init() {
    field = makeit();
  }
}

proc test() {
  var x = new R();
  writeln(x);
}

test();

