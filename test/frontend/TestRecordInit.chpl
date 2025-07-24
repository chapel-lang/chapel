
use Print;

record R {
  var x: int = 4;
  var y: int = 2;
}

record DefaultInit {
  var r : R;
}

record ExplicitInit {
  var r : R;
  proc init() {
    var r : R;
    r.x = 99;
    r.y = 101;
    this.r = r;
  }
}

proc main() {
  var r : R;
  println(r.x);
  println(r.y);

  var x : DefaultInit;
  println(x.r.x);
  println(x.r.y);

  var y : ExplicitInit;
  println(y.r.x);
  println(y.r.y);
}
