
var global = 0;

proc access() ref {
  return global;
}

proc access(x=4) const ref {
  return global;
}

proc access(y=3) const ref {
  return global;
}

var x = access();
writeln(x);
