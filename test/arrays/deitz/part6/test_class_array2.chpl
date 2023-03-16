class C {
  var x: [1..3] int;
}

var a: [1..3] int;
var ownC = new owned C(a);
var c = ownC.borrow();
a(2) = 1;
writeln(c);
writeln(a);
