class D {
  proc this(x:int) {
    return 1..10;
  }
}

class C {
  param rank: int;
  type t;
  var d: unmanaged D = new unmanaged D();

  var n = 10;
  var Dom = {d(1)};
  var A: [Dom] t;
}


var c = new unmanaged C(2,int);
writeln("c.A is: ", c.A);

delete c.d;
delete c;
