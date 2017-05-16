
var global = 0;

proc access() ref {
  return global;
}

proc access(x=4) {
  return global;
}

proc access(y=3) {
  return global;
}

var x = access();
writeln(x);
