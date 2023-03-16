class C {
  var x: [1..3] int;
}

var ownC = new owned C();
var c = ownC.borrow();
c.x(2) = 1;

writeln(c);

var a: c.x.type;

writeln(a);
