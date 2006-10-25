class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

var c : C = C(), d : D = D();

writeln(c);
writeln(d);
