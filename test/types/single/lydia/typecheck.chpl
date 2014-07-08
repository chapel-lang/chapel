var a = 0;
var b: single int;
b.writeEF(1);
writeln(_isSingleType(a.type));
writeln(_isSingleType(b.type));
writeln(_isSingle(a));
writeln(_isSingle(b));
writeln(_isSingle(b.readXX()));