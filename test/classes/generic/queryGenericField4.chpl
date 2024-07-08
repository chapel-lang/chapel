// R(?R) should probably be illegal because of the weird shadowing of `R`

record R {
  param p1;
}

proc test(r: R(?R)) {
  writeln(r);
}

test(new R(2));
compilerError("done");
