class C {
  var s: sync int;
}

begin {
  var c = new unmanaged C();

  c.s.writeEF(2);

  var r: int = c.s.readFE();

  writeln(r);

  c.s.writeEF(3);

  r = c.s.readFE();

  writeln(r);

  delete c;
}
