var global:[1..3] int;

proc getter() ref {
  writeln("REF");
  return global;
}
proc getter() const ref {
  writeln("CONST REF");
  return global;
}


proc ifexpr(a, b, c) ref {
   return if a[1] == 1 then b else c;
}

proc ifexpr(a, b, c) const ref {
   return if a[1] == 1 then b else c;
}

proc setit(x) {
  x[1] = 1;
}

proc readit(x) {
  writeln(x[1]);
}

writeln("setit");
setit(getter());
writeln("readit");
readit(getter());
writeln("setit ifexpr");
setit(ifexpr(getter(), getter(), getter()));
writeln("readit ifexpr");
readit(ifexpr(getter(), getter(), getter()));
