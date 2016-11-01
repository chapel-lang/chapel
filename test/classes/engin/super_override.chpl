class A {

  proc foo(){
    writeln("A.foo() called");
  }
}

class B:A {

  /*
      bar runs if there is no foo that calls super.foo
      (see super_nooverride test)
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

delete b;
delete a;

