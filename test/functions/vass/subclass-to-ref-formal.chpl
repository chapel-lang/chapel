// Test that for a '[const] ref' formal, the type of the actual
// must equal the type of the formal.
// (Unless an implicit conversion comes to play.)

class C    { }
class D: C { }

proc test(ref lhs: C) {}

var c: C;
var d: D;

test(c);
test(d); // must be an error
