var global:[1..3] int;

proc getter() ref {
  writeln("REF");
  return global;
}
proc getter() const ref {
  writeln("CONST REF");
  return global;
}


proc refIdentity(arg) ref {
  arg[1] += 1;
  return arg;
}

proc ifexpr(a, b, c) ref {
   return if a[1] == 1 then refIdentity(b) else refIdentity(c);
}

proc ifexpr(a, b, c) const ref {
   return if a[1] == 1 then refIdentity(b) else refIdentity(c);
}

writeln(ifexpr(getter(), getter(), getter()));
