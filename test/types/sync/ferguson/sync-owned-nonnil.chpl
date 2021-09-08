class C { var x: int; }

var x: sync owned C;

x.writeEF(new owned C(1));

writeln(x.readFE());
