class D {
  var x : int = 2;
}

class C {
  var d : unmanaged D = new unmanaged D();
  proc this() ref : unmanaged D
    return d;
}

var c : unmanaged C = new unmanaged C();

writeln(c);
writeln(c.d);
writeln(c().x);

delete c.d;
delete c;
