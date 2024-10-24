use CompressedSparseLayout;

proc foo(x: csrLayout) {
  writeln("In foo: ", x.type:string);
}

proc bar(x: csrLayout(true)) {
  writeln("In bar: ", x.type:string);
}

proc bar(x: csrLayout(false)) {
  writeln("In bar: ", x.type:string);
}

proc baz(x: csrLayout(?)) {
  writeln("In baz: ", x.type:string);
}

for param i in 0..1 {
  // workaround for bug captured in
  // test/types/records/generic/dispatchWithinParamLoop.chpl
  param flag = i: bool;
  var csr = new csrLayout(flag);
  if flag == true then foo(csr);
  bar(csr);
  baz(csr);
}
