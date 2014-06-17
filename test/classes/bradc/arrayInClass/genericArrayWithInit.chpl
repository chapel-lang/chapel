class D {
  proc this(x:int) {
    return 1..10;
  }
}

class C {
  param rank: int;
  type t;
  var d: D = new D();

  var n = 10;
  var Dom = {d(1)};
  var A: [Dom] t = 0;
}


var c = new C(2,int);
writeln("c.A is: ", c.A);

delete c.d;
delete c;
