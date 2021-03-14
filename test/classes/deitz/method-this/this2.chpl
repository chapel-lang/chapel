class F {
  var x : int = 2;
}

class D {
  var f : unmanaged F = new unmanaged F();
  proc this() ref : unmanaged F
    return f;
}

class C {
  var d : unmanaged D = new unmanaged D();
}

var c : unmanaged C = new unmanaged C();

writeln(c);
writeln(c.d);
writeln(c.d.f);
writeln(c.d().x);

delete c.d.f;
delete c.d;
delete c;
