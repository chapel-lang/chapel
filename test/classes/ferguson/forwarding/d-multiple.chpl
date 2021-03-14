record Wrapper {
  forwarding var a:unmanaged A;
  forwarding var b:unmanaged B;
  proc deinit() {
    delete a,b;
  }
}

class A {
  proc foo() { writeln("in A.foo()"); }
}

class B {
  proc bar() { writeln("in B.bar()"); }
}

var r = new Wrapper(new unmanaged A(), new unmanaged B());
r.foo();
r.bar();
