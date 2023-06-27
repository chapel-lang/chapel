class C {
  var c;
}

class D : C {
  var d;
}

var ownX = new owned D(c = 1, d = 2.0);
var x = ownX.borrow();
var ownY = new owned D(c = 3.0, d = 4);
var y = ownY.borrow();

writeln(x.c + y.d);
writeln(x.d + y.c);
