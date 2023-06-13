// R(?x,?y,?z) is illegal because R has fewer than 3 generic fields.

record R {
  param p1;
}

proc test(r: R(?x, ?y, ?z)) {
  writeln(r);
}

test(new R(2));
compilerError("done");
