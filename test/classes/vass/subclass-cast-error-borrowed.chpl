class C{};
class D:C{};

var c = new borrowed C();
var d = c:borrowed D?;

writeln(c);
writeln(d);
