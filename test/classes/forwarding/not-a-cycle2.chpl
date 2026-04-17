
class C {
  proc foo() {
    writeln("C::foo called");
  }
}

class B {
  forwarding var impl: unmanaged C;
  proc init() {
    this.impl = new unmanaged C();
  }

  proc deinit() {
    delete this.impl;
  }
}

class A {
  forwarding var impl: unmanaged B;
  proc init() {
    this.impl = new unmanaged B();
  }

  proc deinit() {
    delete this.impl;
  }
}

proc main() {
  var a = new unmanaged A();
  a.foo();
  delete a;
}
