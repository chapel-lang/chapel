proc foo(inout x) {
  writeln(x);
  x = 4;
  writeln(x);
}

const y = 2;
writeln(y);
foo(y);
writeln(y);
