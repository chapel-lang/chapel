proc foo(i : int(32)) {
  writeln("int32 = ", i);
}

proc foo(i : int(64)) {
  writeln("int64 = ", i);
}

var i : int(64);
foo(i);

var j : int(32);
foo(j);
