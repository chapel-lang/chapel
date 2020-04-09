// Test that for a '[const] ref' formal, the type of the actual
// must equal the type of the formal.
// (Unless an implicit conversion comes to play.)

class C    { }
class D: C { }

proc test(ref lhs: unmanaged C) {}

var c = new unmanaged C();
var d = new unmanaged D();

test(c);
test(d); // must be an error

delete c;
delete d;
