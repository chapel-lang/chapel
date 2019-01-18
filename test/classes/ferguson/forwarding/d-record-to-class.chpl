
record Wrapper {
  var instance; // e.g. some class
  forwarding instance;
  proc foo() { writeln("in Wrapper.foo()"); }
  proc deinit() {
    delete instance;
  }
}

class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
  proc baz() { field = 1; }
}

var r = new Wrapper(new unmanaged C());
r.foo(); // direct method shadows forwarding method
r.bar(); // same as r.instance.bar(), prints "in C.foo()"


