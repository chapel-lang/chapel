class C{};
class D:C{};

var c = new borrowed C();
var d = c:D;

writeln(c);
writeln(d);
