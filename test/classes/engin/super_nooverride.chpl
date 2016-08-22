class A {

  proc foo(){
    writeln("A.foo() called");
  }
}

class B:A {

  proc bar() {
    super.foo();
    writeln("B.foo() called");
  }
}

var a = new A();
var b = new B();

a.foo();
b.bar();
b.super.foo();

delete b;
delete a;
