use List;

record R {
  type eltType;

  param parSafe = false;

  proc init(type eltType, param parSafe = false) {
    this.eltType = eltType;
    this.parSafe = parSafe;
  }
}


proc makeThing(const ref x: list(?t), parSafe: bool = false) {
  var h = new R(t, parSafe);
  return h;
}

var l:list(int) = [2, 1, 3];
var h = makeThing(l);
writeln(h);
