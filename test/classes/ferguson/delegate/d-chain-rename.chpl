
class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
  proc baz() { writeln("in C.baz()"); }
}

record WrapperOne {
  var instance; // e.g. some class
  delegate instance only foo as bar;
  proc foo() { writeln("in WrapperOne.foo()"); }
}

record WrapperTwo {
  var instance; // WrapperOne
  delegate instance only bar as baz;
  proc foo() { writeln("in WrapperTwo.foo()"); }
}


var r = new WrapperTwo(new WrapperOne(new C()));

r.baz(); // should call C.foo (baz->bar->foo)
