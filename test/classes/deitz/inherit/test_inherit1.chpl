class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

var c : C = new borrowed C(), d : D = new borrowed D();

writeln(c);
writeln(d);
