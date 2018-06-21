use Color;

proc testit(r) {
  // array
  var A: [r] real;

  // standalone parallel loop over range
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

  // zip int range and enum range
  forall (i,c) in zip(5..#r.size, r) do
    A[c] = i;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // range as leader, array as follower
  forall (c,a) in zip(r, A) do
    a = -(chpl__enumToOrder(c)+1);
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // array as leader, range as follower
  forall (a,c) in zip(A, r) do
    a = chpl__enumToOrder(c)+1;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // zip enum array and int range
  forall (a,i) in zip(A, 10..) do
    a = i;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // zip int range and enum array
  forall (i,a) in zip(5..#r.size, A) do
    a = i;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // serial loop over domain
  for i in A.domain do
    writeln(i);
  writeln("---");

  // standalone parallel loop over domain
  forall i in A.domain do
    A[i] = chpl__enumToOrder(i)+1;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");
  
  // range as leader, array as follower
  forall (c,a) in zip(r, A) do
    a = -(chpl__enumToOrder(c)+1);
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // array as leader, range as follower
  forall (a,c) in zip(A, r) do
    a = chpl__enumToOrder(c)+1;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");
  
  // domain as leader, array as follower
  forall (c,a) in zip(A.domain, A) do
    a = -(chpl__enumToOrder(c)+1);
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // array as leader, domain as follower
  forall (a,c) in zip(A, A.domain) do
    a = chpl__enumToOrder(c)+1;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");
  
  // standalone array
  forall a in A do
    a = -a;
  for c in r do
    writeln("A[", c, "] = ", A[c]);
  writeln("---");

  // serial loop over array
  for a in A do
    writeln(a);
  writeln("---");

  writeln();
}


proc main() {
  testit(color.red..color.violet by 2);
  testit(color.yellow..color.indigo by 2);
  testit(color.green..color.green by 2);
  testit(color.blue..color.green by 2);
}
