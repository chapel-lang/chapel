class A {

  proc foo(){
    writeln("A.foo() called");
  }
}

class B:A {

  /*
      bar runs if there is no foo that calls super.foo

  proc bar() {
    super.foo();
    writeln("B.foo() called");
  }
  */
  proc foo() {
    super.foo();
    writeln("B.foo() called");
  }
}

var a = new A();
var b = new B();

a.foo();
b.foo();
// b.bar();
// b.super.foo(); // this works as you'd expect

