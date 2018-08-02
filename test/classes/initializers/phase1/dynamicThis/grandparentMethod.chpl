
//
// This test exists to ensure that the compiler does not emit an error for the
// call to 'grandMethod'. This could happen if the compiler only checked the
// methods in 'Parent'.
//

class Grandparent {
  var g : int;

  proc init() {
    this.g = 1;
  }

  proc grandMethod() {
    writeln("Grandparent.grandMethod()");
  }
}

class Parent : Grandparent {
  var p : real;

  proc init() {
    this.p = 2.0;
  }
}

class Child : Parent {
  var c : string;

  proc init() {
    this.c = "foo";
    grandMethod();
  }
}

var c = new owned Child();
