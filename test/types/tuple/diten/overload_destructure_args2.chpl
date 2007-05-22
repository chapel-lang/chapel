def foo(a, (x,y,z)) {
  writeln("a, (x,y,z) = ", a, ", (", x, ",", y, ",", z, ")"); 
}

def foo(a, (x,y)) {
  writeln("a, (x,y) = ", a, ", (", x, ",", y, ")");
  return (a,x,y);
}

def foo(a, b: real) {
  writeln("a, b: real = ", a, ", ", b);
}

foo(3, foo(0, (1,2)));
foo(1, 2.72);

