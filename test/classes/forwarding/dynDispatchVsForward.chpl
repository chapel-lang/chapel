class C {
  proc foo() {
    writeln("In C's foo()");
  }
}

class D: C {
  proc foo() {
    writeln("In D's foo()");
  }

  proc bar() {
    writeln("In D's bar()");
  }
}

class E: C {
  forwarding var myD: D;
  proc deinit() {
    delete myD;
  }
}

proc E.init() {
  myD = new D();
}

var myE = new E();
myE.bar();
myE.foo();
delete myE;
