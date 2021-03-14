var r:range(stridable=false);
writeln(r);

proc test(arg:range(stridable=true)) {
  writeln(arg);
}

test(r);
