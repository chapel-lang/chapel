class C {
  var s: sync int;
}

var c = C();

c.s = 2;

var r = c.s;

writeln(r);
