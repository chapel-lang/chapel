proc main() {
  var i: int = 0;
  writeln("In main before foo i = ", i);
  atomic {
    foo(i);
  }
  writeln("In main after foo i = ", i);
}

proc foo(inout j: int) {
  j += 1;
  if j < 5 then foo(j);
}