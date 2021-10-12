class A {
  var aa : int;
  proc foo() {
    writeln("A.foo: ", aa);
  }
}

class Z : A {

  override proc foo() {
    writeln("Z.foo: ", aa);
  }

  proc bar(other : Z) {
    this.super.foo();
    other.super.foo(); // compiler error?
  }
}

var x = new Z(0);
var y = new Z(123);

x.bar(y);
