
var global = 0;

proc access() ref {
  return global;
}

proc access() {
  return global;
}

proc access_access() ref {
  return access();
}

proc access_access() const ref {
  return access();
}


var y = access_access();
writeln(y);

