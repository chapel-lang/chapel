var a = 0;
var b: sync int;
b.writeEF(1);
writeln(isSyncType(a.type));
writeln(isSyncType(b.type));
writeln(isSync(a));
writeln(isSync(b));
writeln(isSync(b.readXX()));