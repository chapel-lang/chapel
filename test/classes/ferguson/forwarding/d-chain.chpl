
class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
  proc baz() { writeln("in C.baz()"); }
}

record WrapperOne {
  var instance; // e.g. some class
  forwarding instance;
  proc foo() { writeln("in WrapperOne.foo()"); }
  proc bar() { writeln("in WrapperOne.bar()"); }
}

record WrapperTwo {
  var instance; // WrapperOne
  forwarding instance;
  proc foo() { writeln("in WrapperTwo.foo()"); }
}


var r = new WrapperTwo(new WrapperOne(new C()));

r.foo(); // should call WrapperTwo.foo
r.bar(); // should call WrapperOne.bar
r.baz(); // should call C.baz


