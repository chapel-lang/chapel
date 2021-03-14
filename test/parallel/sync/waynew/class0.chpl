// Test sync field in a class

class D {
  var s: sync int;
}

var d: unmanaged D = new unmanaged D();
var i: int;

d.s.writeEF(4);
i = d.s.readFE();
writeln( "i is ", i);


class E {
  var sf: sync real;
}

var e: unmanaged E = new unmanaged E();
var f: real;

e.sf.writeEF(1.2);
f    = e.sf.readFE();

writeln( "f is ", f);

delete e;
delete d;
