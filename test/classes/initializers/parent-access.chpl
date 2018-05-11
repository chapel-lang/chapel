//
// This test should fail to compile.  The initializer for Derived attempts
// to access Base.a during phase 1.
//

class Base {
  var a : int = 10;
  var b : int = 20;

  proc init() {

  }
}

class Derived : Base {
  var x : int = 30;
  var y : int = 40;

  proc init() {
    x = a;

    this.complete();
  }
}

proc main() {
  var c = new owned Derived();

  writeln(c);
}
