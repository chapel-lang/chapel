class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

var ownC = new owned C(), ownD = new owned D();
var c : borrowed C = ownC.borrow(), d : borrowed D = ownD.borrow();

writeln(c);
writeln(d);
