class C {
  var c;
}

class D : C {
  var d;
}

var x = (new owned D(c = 1, d = 2.0)).borrow();
var y = (new owned D(c = 3.0, d = 4)).borrow();

writeln(x.c + y.d);
writeln(x.d + y.c);
