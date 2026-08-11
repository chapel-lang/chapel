union U {
  var x: int;
  var y: int;
  var z: real;

  proc init() {
    init this;
    writeln(this);
    this.foo();
  }
  proc foo() {
    writeln("In U.foo()");
  }
}

var u: U;
