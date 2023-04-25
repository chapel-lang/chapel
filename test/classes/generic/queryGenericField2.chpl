// R(p2=?) is illegal because R does not have a field p2.

record R {
  param p1;
}

proc test(r: R(p2=?)) {
  writeln(r);
}

test(new R(2));
compilerError("done");
