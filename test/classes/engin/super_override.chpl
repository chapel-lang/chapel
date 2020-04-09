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
  override proc foo() {
    super.foo();
    writeln("B.foo() called");
  }
}

var a = new owned A();
var b = new owned B();

a.foo();
b.foo();
