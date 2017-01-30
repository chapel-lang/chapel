
record Wrapper {
  var instance; // e.g. some class
  delegate instance;
  proc foo() { writeln("in Wrapper.foo()"); }
}

record C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
  proc baz() { field = 1; }
}

proc +(a:C, b:C) {
}

var r = new Wrapper(new C());
r.foo(); // direct method shadows delegated method
r.bar(); // same as r.instance.bar(), prints "in C.foo()"

r + r;

proc frobnify( A:[] ) {
}

proc frobnify( Matrix ) {
}

frobnify(mat);

record Matrix {

  var D;
  var instance:[D] int;

  proc canCoerce(type t) where t == instance.type {
    return true;
  }

  proc cast(type t) ref where t == instance.type {
    return instance;
  }

}

