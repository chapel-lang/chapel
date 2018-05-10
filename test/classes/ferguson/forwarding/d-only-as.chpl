record Wrapper {
  var instance; // e.g. some class
  forwarding instance only bar as f;
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
r.f(); // 'bar' was renamed to 'f'
