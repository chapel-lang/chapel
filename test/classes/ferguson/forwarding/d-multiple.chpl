record Wrapper {
  forwarding var a:A;
  forwarding var b:B;
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

var r = new Wrapper(new A(), new B());
r.foo();
r.bar();
