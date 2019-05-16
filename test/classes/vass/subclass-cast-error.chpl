class C{};
class D:C{};

var c = new unmanaged C();
var d = c:unmanaged D?;

writeln(c);
writeln(d);

delete c;

