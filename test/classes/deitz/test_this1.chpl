class D {
  var x : int = 2;
}

class C {
  var d : D = D();
  function this() var : D
    return d;
}

var c : C = C();

writeln(c);
writeln(c.d);
writeln(c().x);
