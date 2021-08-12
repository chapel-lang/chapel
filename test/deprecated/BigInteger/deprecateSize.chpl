use BigInteger;

// Tests the size deprecation message
var a = new bigint(-65533256);
var b = new bigint(-639283632585);

var c = a * b * a;
writeln(a, " has ", a.size(), " limb(s)");
writeln(b, " has ", b.size(), " limb(s)");
writeln(c, " has ", c.size(), " limb(s)");
