use List;

record R {
  type eltType;

  param parSafe = false;

  proc init(type eltType, bub: eltType, param parSafe = false) {
    this.eltType = eltType;
    this.parSafe = parSafe;
  }
}


proc makeThing(const ref x: list(?t), bub: x.type, parSafe: bool = false) {
  var x: t;
  var h = new R(t, x, parSafe);
  return h;
}

var l:list(int) = [2, 1, 3];
var h = makeThing(l, l);
writeln(h);
