class C {
  var s: sync int;
}

var c = new unmanaged C();

c.s = 2;

var r = c.s;

writeln(r);

delete c;
