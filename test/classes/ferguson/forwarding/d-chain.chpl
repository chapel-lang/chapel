
class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
  proc baz() { writeln("in C.baz()"); }
}

class D {
  var field:int;
  proc foo() { writeln("in D.foo()"); }
  proc bar() { writeln("in D.bar()"); }
  proc baz() { writeln("in D.baz()"); }
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


{
  var a = new WrapperTwo(new WrapperOne(new unmanaged C(1)));

  a.foo(); // should call WrapperTwo.foo
  a.bar(); // should call WrapperOne.bar
  a.baz(); // should call C.baz

  // Can't use deinit, otherwise there will be a double-free
  delete a.instance.instance;
}

{
  var b = new WrapperTwo(new WrapperOne(new unmanaged D(2)));

  b.foo(); // should call WrapperTwo.foo
  b.bar(); // should call WrapperOne.bar
  b.baz(); // should call D.baz

  delete b.instance.instance;
}
