
var  global0:int;
var  global:int;

proc access(i:int) const ref {
  writeln("access value");
  return global0;
}
proc access(i:int) ref {
  writeln("access ref");
  return global0;
}

proc access_access(i:int) const ref {
  writeln("access_access value");
  if i == 0 {
    return access(i);
  }
  return global;
}
proc access_access(i:int) ref {
  writeln("access_access ref");
  if i == 0 {
    return access(i);
  }
  return global;
}

var x = access_access(0);
writeln(x);

var y = access_access(1);
writeln(y);
