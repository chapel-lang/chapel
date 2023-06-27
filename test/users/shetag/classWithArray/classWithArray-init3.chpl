class C {
  const arr : [1..2] int = 0;
}

const a : [1..2] int = (1, 2);

var ownC2 = new owned C(a);
var c2 = ownC2.borrow();

var c1 : borrowed C?;
var ownC1 = new owned C(a);
c1 = ownC1.borrow();

var ownC3 = new owned C();
var c3 = ownC3.borrow();

var c4: borrowed C?;


writeln("c1 is: ", c1);
writeln("c2 is: ", c2);
writeln("c3 is: ", c3);
writeln("c4 is: ", c4);
