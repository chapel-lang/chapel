function foo(x, y, z) {
  return (x - 1) * y + z;
}

writeln("expect 4", foo(2, 1, 3));
writeln("expect 6", foo(z = 5, 2, 1));
writeln("expect 6", foo(z = 5, x = 2, 1));
writeln("expect 7", foo(z = 5, y = 2, 2));
