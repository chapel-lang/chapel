def foo(a: int, (b, (c, d))) {
  writeln("One normal arg, one nested tuple arg.");
}

def foo(a: int, (b, c)) {
  writeln("One int arg, one two-tuple arg.");
}

foo(1, (2,(3,4)));
foo(1, (2,3));
