class C { var x: int; }

var x: sync shared C?;

x.writeEF(new shared C?(1));

writeln(x.readFE());
