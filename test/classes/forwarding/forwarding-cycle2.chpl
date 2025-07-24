
class C {
  forwarding var impl: unmanaged A;
  proc init() {
    this.impl = new unmanaged A();
  }
  proc foo() {
    writeln("C::foo called");
  }
}

class B {
  forwarding var impl: owned C;
  proc init() {
    this.impl = new owned C();
  }
}

class A {
  forwarding var impl: owned B;
  proc init() {
    this.impl = new owned B();
  }
}

proc main() {
  var a = new owned A();
  a.foo();
}
