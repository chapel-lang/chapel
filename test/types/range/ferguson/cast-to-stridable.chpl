var a:range(int, boundKind.both, stridable=false) = 1..10;
var b = a:range(int, boundKind.both, stridable=true);
writeln(a);
writeln(b);
