class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

var c : C = new C(), d : D = new D();

writeln(c);
writeln(d);

delete c;
delete d;
