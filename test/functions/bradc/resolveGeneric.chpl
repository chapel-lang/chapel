class C {
}

class D : C {
  type t;
  param rank: int;
  var x: string;

  proc foo(x) {
    x.x = this.x;
  }
}

var myD = new borrowed D(real, 2, "hi");
var myD2 = new borrowed D(real, 2);

writeln("myD2.x = ", myD2.x);

myD.foo(myD2);

writeln("myD2.x = ", myD2.x);

proc bar(x) {
  var xx: x.type = new x.type();
  x.foo(xx);
  writeln("xx.x = ", xx.x);
}

bar(myD);

