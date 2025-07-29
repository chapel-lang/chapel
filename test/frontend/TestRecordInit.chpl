
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

proc println(r: R) {
  println(r.x);
  println(r.y);
}

proc main() {
  var r : R;
  println(r);

  var x : DefaultInit;
  println(x.r);

  var y : ExplicitInit;
  println(y.r);

  var z = new R(100, 200);
  println(z);
}
