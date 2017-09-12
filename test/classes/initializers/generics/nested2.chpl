class C {
  param rank: int;
  var x: D;

  class D {
  }

  proc init() {
    rank = 2;
    x = new D();
    super.init();
  }
}

var myC = new C();
writeln(myC);
