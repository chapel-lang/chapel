class C {
  param rank: int;
  var x: owned D;

  pragma "use default init"
  class D {
  }

  proc init() {
    rank = 2;
    this.complete();
    x = new owned D();
  }
}

var myC = new owned C();
writeln(myC);
