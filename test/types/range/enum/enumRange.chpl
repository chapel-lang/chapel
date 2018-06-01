use Color;

proc testRange(r) {
  var r2 = r;
  writeln(r2);
  writeln(r2.type:string);
  writeln();

  for c in r do
    writeln(c);
  writeln();

  writeln(r.length);
  writeln(r.size);

  var A: [r] real;
  forall c in r do
    A[c] = chpl__enumToOrder(c);
  for c in r do
    writeln("A[", c, "] = ", A[c]);
}


proc main() {
  testRange(color.red..color.violet);
  for c in color.red..color.violet do
    writeln(c);
  writeln();

testRange(color.yellow..color.indigo);
for c in color.yellow..color.indigo do
  writeln(c);
writeln();
}
