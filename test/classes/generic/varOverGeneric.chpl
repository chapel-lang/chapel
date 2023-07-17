class C {
  type t;
  var x: t;
}

var ownC = new owned C(int, 1);
var myC: borrowed C = ownC.borrow();
writeln(myC);
