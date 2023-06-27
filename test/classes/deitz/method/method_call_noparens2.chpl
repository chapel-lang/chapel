class C {
  var x = 2;
  proc foo do return 3;
  proc bar do return foo;
}

var ownC = new owned C(4);
var c = ownC.borrow();

writeln(c.bar);
