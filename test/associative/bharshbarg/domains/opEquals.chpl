var a = {"b", "c", "d"};
var b = {"a", "c", "r"};
var c = {"a", "f", "q"};
var temp = b;

temp |= a;
for i in temp do assert(a.member(i) || b.member(i));
writeln(temp);

temp -= a;
for i in temp do assert(!a.member(i));
writeln(temp);

temp &= c;
for i in temp do assert(b.member(i) && c.member(i));
writeln(temp);

temp ^= c;
for i in temp do assert(!(b.member(i) && c.member(i)));
writeln(temp);
