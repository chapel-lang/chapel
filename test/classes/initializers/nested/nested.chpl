class C {
  param rank: int;
  var x: owned D?;

  class D {
  }

  proc init() {
    rank = 2;
    init this;
    x = new owned D();
  }
}

var myC = new owned C();
writeln(myC);
