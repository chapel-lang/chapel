var a = 0;
var b: single int;
b.writeEF(1);
writeln(isSingleType(a.type));
writeln(isSingleType(b.type));
writeln(isSingle(a));
writeln(isSingle(b));
writeln(isSingle(b.readXX()));