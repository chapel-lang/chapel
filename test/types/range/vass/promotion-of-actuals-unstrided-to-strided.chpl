var r:range(int, boundKind.both, false);
writeln(r);

proc test(arg:range(int, boundKind.both, true)) {
  writeln(arg);
}

test(r);
