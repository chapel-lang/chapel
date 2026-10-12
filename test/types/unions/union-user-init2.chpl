union U {
  var x: int;
  var y: int;
  var z: real;

  proc init() {
    this.x = 42;
    writeln(this);
    this.foo();
  }
  proc foo() {
    writeln("In U.foo()");
  }
}

var u: U;
