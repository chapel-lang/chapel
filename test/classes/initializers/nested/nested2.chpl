class C {
  param rank: int;
  var x: D;

  pragma "use default init"
  class D {
  }

  proc init() {
    rank = 2;
    x = new D();
  }
}

var myC = new C();
writeln(myC);
