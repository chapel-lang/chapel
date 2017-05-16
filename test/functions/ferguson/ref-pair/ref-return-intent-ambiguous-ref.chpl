
var global = 0;

proc access(x=3) ref {
  return global;
}


proc access(y=4) ref {
  return global;
}

proc access() {
  return global;
}

var x = access();
writeln(x);
