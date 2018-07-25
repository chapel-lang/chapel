class MyClass {
  var _x:int;
  proc ref accessor() ref {
    writeln("ref");
    return _x;
  }
  proc const ref accessor() const ref {
    writeln("const ref");
    return _x;
  }
}

record R {
  forwarding var ptr:MyClass;
}

proc getx1(arg:R) {
  return arg._x;
}

proc getx2(arg:R) {
  return arg.accessor();
}

proc setx3(ref arg:R) {
  arg._x = 3;
}

proc setx4(ref arg:R) {
  arg.accessor() = 4;
}

proc test() {
  var r = new R(new MyClass(1));
  var x1 = getx1(r);
  writeln(x1);
  var x2 = getx2(r);
  writeln(x2);
  setx3(r);
  writeln(r);
  setx4(r);
  writeln(r);
}

test();
