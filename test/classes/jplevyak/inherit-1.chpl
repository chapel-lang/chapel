class C {
  var c;
}

class D : C {
  var d;
}

var x = new D(c = 1, d = 2.0);
var y = new D(c = 3.0, d = 4);

writeln(x.c + y.d);
writeln(x.d + y.c);

delete y;
delete x;
