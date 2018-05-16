class Impl {
  var x:int;
  proc init(arg:int) {
    writeln("In Impl.init");
  }
  proc deinit() {
    writeln("In Impl.deinit");
  }
}

record R {
  var field;

  proc init() {
    field = new Shared(new Impl(1));
  }
}

proc test() {
  var x = new R();
  writeln(x);
}

test();

