var a = 0;
var b: sync int;
b.writeEF(1);
writeln(_isSyncType(a.type));
writeln(_isSyncType(b.type));
writeln(_isSync(a));
writeln(_isSync(b));
writeln(_isSync(b.readXX()));