
// This test exists to ensure that the RHS temporary is destroyed for a
// variable declaration that invokes init=

record R {
  type T;
  var x : T;

  proc init=(other: R) {
    this.T = other.T;
    this.x = other.x;
  }
  proc init=(other: this.type.T) {
    this.T = this.type.T;
    this.x = other;
  }
}

record X {
  var x : int;

  proc init(x = 0) {
    this.x = x;
    writeln("X.init: ", x);
  }

  proc init=(other: X) {
    this.x = other.x;
    writeln("X.init=: ", x);
  }

  proc deinit() {
    writeln("destroy X: ", x);
  }
}

proc main() {
  var r : R(X) = new X(5);
}
