use Color;

proc testRange(r) {
  // copy
  var r2 = r;
  writeln(r2);
  writeln(r2.type:string);
  writeln();

  // serial loop
  for c in r do
    writeln(c);
  writeln();

  writeln("length = ", r.length);
  writeln("size = ", r.size);
  writeln();

  // array
  var A: [r] real;

  // standalone parallel loop
  forall c in r do
    A[c] = chpl__enumToOrder(c)+1;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // zip enum range and int range
  forall (c,i) in zip(r, 10..) do
    A[c] = i;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  /*
  // zip int range and enum range
  forall (i,c) in zip(5..#r.size, r) do
    A[c] = i;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");
  */

  /*
  // range as leader, array as follower
  forall (c,a) in zip(r, A) do
    a = -(c:int - color.red:int + 1): real;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // array as leader, range as follower
  forall (a,c) in zip(A, r) do
    a = (c:int - color.red:int): real;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // standalone array
  forall a in A do
    a = -a;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  */
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
