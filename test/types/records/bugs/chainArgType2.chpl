record r1 {
  type t;
}

proc foo(x: r1(?), y: x.type) {
  writeln("In foo, got ", (x, y));
}

foo(new r1(int), new r1(real));
