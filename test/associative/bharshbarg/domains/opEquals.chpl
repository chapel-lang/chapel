var a = {"b", "c", "d"};
var b = {"a", "c", "r"};
var c = {"a", "f", "q"};
var temp = b;

temp |= a;
for i in temp do assert(a.contains(i) || b.contains(i));

temp -= a;
for i in temp do assert(!a.contains(i));

temp &= c;
for i in temp do assert(b.contains(i) && c.contains(i));

temp ^= c;
for i in temp do assert(!(b.contains(i) && c.contains(i)));

var d = {"a", "b"};
var e = {"x", "y"};
d -= e;
