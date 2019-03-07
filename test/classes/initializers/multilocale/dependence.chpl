use BlockDist;

class C {
  var Dom = {1..numLocales};
  var Space = Dom dmapped Block(Dom);
  var x : int;

  proc init(x = 5) {
    this.x = x;
  }
}

var c = new borrowed C();
writeln(c.Dom);
