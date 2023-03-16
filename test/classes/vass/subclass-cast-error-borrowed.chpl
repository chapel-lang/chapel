class C{};
class D:C{};
var ownC = new owned C();
var c = ownC.borrow();
var d = c:borrowed D?;

writeln(c);
writeln(d);
