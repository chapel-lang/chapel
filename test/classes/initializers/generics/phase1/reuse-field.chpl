
// August 2018
//
// The compiler does not currently allow methods to be resolved on partially
// instantiated types. At one point in time, the compiler was transforming the
// 'this.x' call below into a method call like "call x this", which would then
// fail during resolution because the type of 'y' was not yet known.

record R {
  var x;
  var y;

  proc init(A, B) {
    this.x = A;
    this.y = this.x * B;
  }
}

var r = new R(1, 2);
writeln(r);

record S {
  var x;
  var y;

  proc init(r:R) {
    this.x = r;
    this.y = x.y * 2;
  }
}

var s = new S(r);
writeln(s);
