class C {
  type t;
  var x: t;
}

var ownC1 = new owned C(int);
var c1: C(int) = ownC1.borrow();
var ownC2 = new owned C(real);
var c2: C(real) = ownC2.borrow();

writeln(c1.x.type: string);
writeln(c2.x.type: string);
