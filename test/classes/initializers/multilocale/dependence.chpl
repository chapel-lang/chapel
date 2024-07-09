use BlockDist;

class C {
  var Dom = {1..numLocales};
  var Space = Dom dmapped new blockDist(Dom);
  var x : int;

  proc init(x = 5) {
    this.x = x;
  }
}

var cTmp = new owned C();
var c = cTmp.borrow();
writeln(c.Dom);
