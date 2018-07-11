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

var a = new owned A();
var b = new owned B();

a.foo();
b.bar();
b.super.foo();
