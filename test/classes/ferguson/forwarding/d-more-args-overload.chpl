
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
  proc foo(a:int, b:real, c:string) { writeln("in C.foo", (a,b,c)); }
  proc bar(b:real, c:string) { writeln("in C.bar", (b,c)); }
  proc baz(a:int) { field = a; }
}

var r = new Wrapper(new unmanaged C());
r.foo(1, 2.0, " 3 "); // Wrapper's 'foo' has different signature
r.bar(20.0, " 30 "); // same as r.instance.bar(), prints "in C.foo()"


