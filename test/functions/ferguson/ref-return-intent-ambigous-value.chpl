
var global = 0;

proc access() ref {
  return global;
}

proc access() {
  return global;
}

proc access() const ref {
  return global;
}

var x = access();
writeln(x);
