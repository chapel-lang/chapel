record R1 {
  var x: int;
}

record R2 {
  var y: real;
}

class C {
  var x: R;

  proc init(y: real) {
    x = y;
  }
}

var myC = new shared C(new R2(1.2));
