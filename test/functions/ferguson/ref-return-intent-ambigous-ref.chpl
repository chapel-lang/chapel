
var global = 0;

proc access(arg=4) ref {
  return global;
}


proc access() ref {
  return global;
}

proc access() {
  return global;
}

var x = access();
writeln(x);
