class C {
  proc foo() {
    writeln("In C's foo()");
  }
}

class D: C {
  override proc foo() {
    writeln("In D's foo()");
  }

  proc bar() {
    writeln("In D's bar()");
  }
}

class E: C {
  forwarding var myD: unmanaged D;
  proc deinit() {
    delete myD;
  }
}

proc E.init() {
  myD = new unmanaged D();
}

var myE = new unmanaged E();
myE.bar();
myE.foo();
delete myE;
