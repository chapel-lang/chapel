class C {
  var x: int;
}

class D : C {
  type t;
  param r: int;
}

proc foo(argD: D(?t, ?r)) {
  writeln("argD.x = ", argD.x);
}

var myD = new D(x=3, t=int, r=2);

foo(myD);
