class D {
  var x : int = 2;
}

class C {
  var d : D = new D();
  def this() var : D
    return d;
}

var c : C = new C();

writeln(c);
writeln(c.d);
writeln(c().x);
