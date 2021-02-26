
record R {
  var x: int = 5;
  proc y() { writeln("R.y"); }
}

proc Q(arg) type return R;

proc (Q(0)).z() {
  writeln(x);
  y();
}

var r: R;
r.z();
