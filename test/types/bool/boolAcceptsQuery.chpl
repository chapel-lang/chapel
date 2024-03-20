config param testW = false;  // causes internal error

proc foo(a: bool(?)) {
  writeln("In foo");
}

proc bar(a: bool(?w)) {
  writeln("In bar");
  if testW then
    writeln("w is: ", w);
}

record R {}

proc baz(r: R(?)) {
  writeln("In baz");
}

foo(true);
bar(false);
baz(new R());
