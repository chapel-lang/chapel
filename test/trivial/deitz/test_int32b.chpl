def foo(i : int(32)) {
  writeln("int32 = ", i);
}

def foo(i : int) {
  writeln("int64 = ", i);
}

var i : int;
foo(i);

var j : int(32);
foo(j);
