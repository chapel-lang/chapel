record Wrapper {
  forwarding var instance:C;
  proc foo() { writeln("in Wrapper.foo()"); }
}

class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
  proc baz() { field = 1; }
}

var r = new Wrapper(new C());
r.foo(); // direct method shadows forwarding method
r.foo();
r.bar(); // same as r.instance.bar(), prints "in C.foo()"
r.bar();
