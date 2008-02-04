class F {
  var x : int = 2;
}

class D {
  var f : F = new F();
  def this() var : F
    return f;
}

class C {
  var d : D = new D();
}

var c : C = new C();

writeln(c);
writeln(c.d);
writeln(c.d.f);
writeln(c.d().x);
