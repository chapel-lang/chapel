record Wrapper {
  var instance; // e.g. some class
  forwarding instance only bar;
  proc deinit() {
    delete instance;
  }
}

class C {
  var field:int;
  proc foo() { writeln("in C.foo()"); }
  proc bar() { writeln("in C.bar()"); }
}

var r = new Wrapper(new unmanaged C());
r.bar(); // in only list
r.foo(); // error: not in only list
