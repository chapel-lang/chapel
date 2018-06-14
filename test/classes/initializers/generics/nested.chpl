class C {
  param rank: int;
  var x: D;

  pragma "use default init"
  class D {
  }

  proc init() {
    rank = 2;
    this.complete();
    x = new D();
  }
}

var myC = new C();
writeln(myC);
