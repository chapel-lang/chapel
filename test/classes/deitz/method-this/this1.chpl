class D {
  var x : int = 2;
}

class C {
  var d : D = new unmanaged D();
  proc this() ref : D
    return d;
}

var c : C = new unmanaged C();

writeln(c);
writeln(c.d);
writeln(c().x);

delete c.d;
delete c;
