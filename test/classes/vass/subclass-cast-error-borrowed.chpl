class C{};
class D:C{};

var c = (new owned C()).borrow();
var d = c:borrowed D?;

writeln(c);
writeln(d);
