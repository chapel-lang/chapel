record Wrapper {
  var instance; // e.g. some class
  forwarding instance only bar;
}

class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
}

var r = new Wrapper(new C());
r.bar(); // in only list
