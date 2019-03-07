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
  override proc foo() {
    writeln("In D.foo()");
  }

  proc baz() {
    writeln("In D.baz()");
  }
}

class E : C {
  override proc foo() {
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
var myC = new owned C();
writeln("D:");
var myD = new owned D();
writeln("E:");
var myE = new owned E();
