
record R {
  var x;
  var y;

  proc x ref {
    writeln("R.x");
    return x;
  }

  proc init(A,B) {
    this.x = A;
    this.y = x * 2;
  }
}

var r = new R(1, 2);
writeln(r);
