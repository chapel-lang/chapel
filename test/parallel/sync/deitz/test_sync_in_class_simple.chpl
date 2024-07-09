class C {
  var s: sync int;
  proc init() {}
}

var c = new unmanaged C();

c.s.writeEF(2);

var r = c.s.readFE();

writeln(r);

delete c;
