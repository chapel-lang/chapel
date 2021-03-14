record R {
  const c: int;
  var v: int;
}

record R2 {
  var r: R;

  proc init(initR:R) {
    r = initR;
  }
}

var myR = new R(c=1, v=2);
var myR2 = new R2(myR);

writeln(myR2);
