
// This test exists to ensure that the compiler doesn't screw up when fiddling
// with the type and class ID of 'this' in phase one.

class Parent {
  type eltType;
  var x : eltType;

  proc init(type t) {
    this.eltType = t;
  }

  proc inheritedMethod() {
    writeln(this.type:string, ".inheritedMethod()");
  }
}

proc foobar(p : borrowed Parent(int)) {
  writeln("foobar(Parent(int))");
}
proc foobar(p : borrowed Parent) {
  writeln("foobar(Parent(other))");
}

class Child : Parent {
  var y : real;

  proc init(type t) {
    super.init(t);
    inheritedMethod();
    foobar(this);
    complete();
    inheritedMethod();
  }

  override proc inheritedMethod() {
    writeln(this.type:string, ".inheritedMethod()");
  }
}

writeln("----- integer -----");
var childInt = new unmanaged Child(int);
delete childInt;

writeln();

writeln("----- real -----");
var childReal = new unmanaged Child(real);
delete childReal;
