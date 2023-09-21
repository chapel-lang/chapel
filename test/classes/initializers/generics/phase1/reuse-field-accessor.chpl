
record R {
  var x_;
  var y;

  proc x ref {
    writeln("R.x");
    return x_;
  }

  proc init(A,B) {
    this.x = A;
    this.y = x * 2;
  }
}

var r = new R(1, 2);
writeln(r);
