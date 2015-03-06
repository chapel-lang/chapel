
class C {
  proc param foo() {
    writeln("In foo() with param this");
  }

  proc ref foo() {
    writeln("In foo() with ref this");
  }

  proc foo() {
    writeln("In foo() with blank-intent this");
  }
}

var c = new C();
c.foo();
