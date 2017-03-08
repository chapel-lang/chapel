record Wrapper {
  var instance; // e.g. some class
  forwarding instance only bar as f;
}

class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
}

var r = new Wrapper(new C());
r.foo(); // error: foo not included in only statement
