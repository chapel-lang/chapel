record r1 {
}

record r2 {
}

proc foo(x: record, y: x.type) {
  writeln("In foo, got ", (x, y));
}

foo(new r1(), new r1());
foo(new r1(), new r2());
