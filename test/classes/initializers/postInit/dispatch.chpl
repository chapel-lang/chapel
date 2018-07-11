class C {
  proc foo() {
    writeln("In C.foo()");
  }

  proc bar() {
    writeln("In C.bar()");
  }

  proc postinit() {
    foo();
    bar();
    //    baz();  // not possible since C doesn't define it
  }
}

class D : C {
  proc foo() {
    writeln("In D.foo()");
  }

  proc baz() {
    writeln("In D.baz()");
  }
}

class E : C {
  proc foo() {
    writeln("In E.foo()");
  }

  proc baz() {
    writeln("In E.baz()");
  }

  proc postinit() {
    foo();
    bar();
    baz();
  }
}

writeln("C:");
var myC = new C();
writeln("D:");
var myD = new D();
writeln("E:");
var myE = new E();

delete myE;
delete myD;
delete myC;
