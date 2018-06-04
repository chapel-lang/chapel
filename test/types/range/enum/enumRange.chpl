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
  writeln("first = ", r.first);
  writeln("last = ", r.last);
  writeln("low = ", r.low);
  writeln("high = ", r.high);
  writeln("alignedLow = ", r.alignedLow);
  writeln("alignedHigh = ", r.alignedHigh);
  writeln("isEmpty = ", r.isEmpty());
  writeln("hasFirst = ", r.hasFirst());
  writeln("hasLast = ", r.hasLast());
  writeln("isNaturallyAligned = ", r.isNaturallyAligned());
  writeln("isAmbiguous = ", r.isAmbiguous());
  writeln("member(blue) = ", r.member(color.blue));
  writeln("member(red) = ", r.member(color.red));
  writeln("ident(color.red..color.violet) = ", ident(r, color.red..color.violet));
  writeln("ident(color.yellow..color.indigo) = ", ident(r, color.yellow..color.indigo));
  writeln("indexOrder(color.yellow) = ", r.indexOrder(color.yellow));
  writeln("indexOrder(color.green) = ", r.indexOrder(color.green));
  if (r.size > 1) then
    writeln("orderToIndex(1) = ", r.orderToIndex(1));
  if (r.last != color.violet) then
    writeln("r.translate(1) = ", r.translate(1));
  if (r.size > 1) {
    writeln("interior(1) = ", r.interior(1));
    writeln("interior(-1) = ", r.interior(-1));
  }
  if (r.size > 1 && r.last != color.violet) {
    writeln("r.exterior(1) = ", r.exterior(1));
    writeln("r.exterior(-1) = ", r.exterior(-1));
    writeln("r.expand(1) = ", r.expand(1));
    writeln("r.offset(1) = ", r.offset(1));
  }


  writeln();

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

  testRange(color.green..color.green);
  for c in color.green..color.green do
    writeln(c);
  writeln();

  testRange(color.blue..color.green);
  for c in color.blue..color.green do
    writeln(c);
  writeln();
}
