var r64: range(int(64), boundKind.both, false) = 7..9;
var r32 = r64 : range(int(32), boundKind.both, false);
writeln(r32);
