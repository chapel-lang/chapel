use UniquePtr;

class C {
  proc ~C() {
    writeln("Destroying C");
  }
  proc method() {
    writeln("in C.method");
  }
}

proc f(arg:C) {
  writeln("in f");
}

proc test() {
  var x = new Owned(new C());

  x.get().method();

  {
    var y = new Owned(x.get());
  }

  var y = x;
}


test();

