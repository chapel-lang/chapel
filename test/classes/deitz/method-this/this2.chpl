class F {
  var x : int = 2;
}

class D {
  var f : F = new F();
  proc this() ref : F
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

delete c.d.f;
delete c.d;
delete c;
