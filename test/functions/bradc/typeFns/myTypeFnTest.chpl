proc myint(param x: bool) type { return if x then int(32) else int(64); }

var x: myint(true);
var y: myint(false);

writeln("numBits(x) = ", numBits(x.type));
writeln("numBits(y) = ", numBits(y.type));

