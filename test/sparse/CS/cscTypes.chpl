use CompressedSparseLayout;

proc foo(x: cscLayout) {
  writeln("In foo: ", x.type:string);
}

proc bar(x: cscLayout(true)) {
  writeln("In bar: ", x.type:string);
}

proc bar(x: cscLayout(false)) {
  writeln("In bar: ", x.type:string);
}

proc baz(x: cscLayout(?)) {
  writeln("In baz: ", x.type:string);
}

for param flag in false..true {
  var csr = new cscLayout(flag);
  if flag == true then foo(csr);
  bar(csr);
  baz(csr);
}
