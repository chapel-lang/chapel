class F {
  var x : integer = 2;
}

class D {
  var f : F = F();
  function this() var : F
    return f;
}

class C {
  var d : D = D();
}

var c : C = C();

writeln(c);
writeln(c.d);
writeln(c.d.f);
writeln(c.d().x);
