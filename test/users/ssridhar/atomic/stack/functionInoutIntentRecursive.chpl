proc main() {
  var i: int = 0;
  writeln("In main before foo i = ", i);
  atomic {
    foo(i);
    bar(i+1);
  }
  writeln("In main after foo i = ", i);
}

proc foo(inout i: int) {
  i += 1;
  if i < 5 then foo(i);
}

proc bar(i: int): int(64) {
  var j: [1..10] int(64) = -5;
  var k: int(64) = -10;
  var l: int(64) = -20;
  j += 1;
  k += 1;
  l += 20;
  return k;
}