class C {
  type t;
  var x : t;
}

var ownC = (new owned C(t = int));
var c : borrowed C(t = int) = ownC.borrow();
c.x = 12;
writeln(c);
