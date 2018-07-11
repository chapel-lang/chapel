
class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
  proc baz() { writeln("in C.baz()"); }
}

class D {
  var field:int;
  proc dfoo() { writeln("in D.dfoo()"); }
  proc dbar() { writeln("in D.dbar()"); }
  proc dbaz() { writeln("in D.dbaz()"); }
}


record WrapperOne {
  var instance; // e.g. some class
  forwarding instance;
  proc foo() { writeln("in WrapperOne.foo()"); }
  proc bar() { writeln("in WrapperOne.bar()"); }
  proc dfoo() { writeln("in WrapperOne.dfoo()"); }
  proc dbar() { writeln("in WrapperOne.dbar()"); }
}
record WrapperTwo {
  var instance; // e.g. some class
  forwarding instance;
  proc foo() { writeln("in WrapperTwo.foo()"); }
  proc bar() { writeln("in WrapperTwo.bar()"); }
  proc dfoo() { writeln("in WrapperTwo.dfoo()"); }
  proc dbar() { writeln("in WrapperTwo.dbar()"); }
}


record WrapperOuter {
  var instance; // WrapperOne
  forwarding instance;
  proc foo() { writeln("in WrapperOuter.foo()"); }
  proc dfoo() { writeln("in WrapperOuter.dfoo()"); }
}


{
  var a = new WrapperOuter(new WrapperOne(new unmanaged C(1)));

  a.foo(); // should call WrapperOuter.foo
  a.bar(); // should call WrapperOne.bar
  a.baz(); // should call C.baz

  // Can't use deinit, otherwise there will be a double-free
  delete a.instance.instance;
}

{
  var b = new WrapperOuter(new WrapperTwo(new unmanaged D(2)));

  b.dfoo(); // should call WrapperOuter.dfoo
  b.dbar(); // should call WrapperTwo.dbar
  b.dbaz(); // should call D.dbaz

  delete b.instance.instance;
}

{
  var c = new WrapperOuter(new WrapperOne(new unmanaged D(3)));

  c.dfoo(); // should call WrapperOuter.dfoo
  c.dbar(); // should call WrapperOne.dbar
  c.dbaz(); // should call D.dbaz

  delete c.instance.instance;
}

{
  var d = new WrapperOuter(new WrapperTwo(new unmanaged C(4)));

  d.foo(); // should call WrapperOuter.foo
  d.bar(); // should call WrapperTwo.bar
  d.baz(); // should call C.baz

  delete d.instance.instance;
}
