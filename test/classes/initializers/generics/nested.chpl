class C {
  param rank: int;
  var x: D;

  class D {
  }

  proc init() {
    rank = 2;
    this.initDone();
    x = new D();
  }
}

var myC = new C();
writeln(myC);
