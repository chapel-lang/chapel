enum color { red };

{
  writeln("explicit initialized");
  var r = color.red..color.red;
  testit(r);

  writeln("overwritten:");
  var r2: range(color);
  r = r2;
  testit(r);
}

{
  writeln("default initialized");
  var r: range(color);
  testit(r);

  writeln("overwritten:");
  r = color.red..color.red;
  testit(r);
}

{
  writeln("implicit strided");
  var r: range(color, stridable=true);
  testit(r);

  writeln("overwritten:");
  r = color.red..color.red;
  testit(r);

  writeln("strided:");
  r = color.red..color.red by 2;
  testit(r);
}





proc testit(r) {
  writeln("r is: ", r);
  writeln("r.size: ", r.size);
  writeln("r.low: ", r.low);
  writeln("r.high: ", r.high);

  writeln("serial iter:");
  for i in r do
    writeln(i);

  writeln("serial follower:");
  for i in zip(1..#r.size, r) do
    writeln(i);

  writeln("serial leader:");
  for i in zip(r, 1..) do
    writeln(i);

  writeln("standalone par iter:");
  forall i in r do
    writeln(i);

  writeln("parallel follower:");
  forall i in zip(1..#r.size, r) do
    writeln(i);

  writeln("parallel leader:");
  forall i in zip(r, 1..) do
    writeln(i);

  const D = {r};
  const D2 = {r, r};

  writeln("1D domain D: ", D);
  writeln("D.size: ", D.size);
  writeln("D serial iter:");
  for i in D do
    writeln(i);

  writeln("D standalone parallel:");
  forall i in D do
    writeln(i);

  writeln("D zip with self:");
  forall (i,j) in zip(D, D) do
    writeln((i,j));

  writeln("2D domain D2: ", D2);
  writeln("D2.size: ", D2.size);

  writeln("D2 serial iter:");
  for ij in D2 do
    writeln(ij);

  writeln("D2 standalone parallel:");
  forall i in D2 do
    writeln(i);

  writeln("D2 zip with self:");
  forall (i,j) in zip(D2, D2) do
    writeln((i,j));

  var A: [D] int;
  var B: [D2] int;
  var C: [r] int;

  writeln("A.size: ", A.size);
  writeln("A.dims.size: ", A.domain.dim(0).size);
  writeln("A: ", A);
  writeln("B.size:", B.size);
  writeln("B.dims.size: ", (B.domain.dim(0).size, B.domain.dim(1).size));
  writeln("B: ", B);
  writeln("C.size: ", C.size);
  writeln("C.dims.size: ", C.domain.dim(0).size);
  writeln("C: ", C);

  writeln("Par A Loop:");
  forall a in A do
    writeln(a);

  writeln("Par B Loop:");
  forall b in B do
    writeln(b);

  writeln("Par C Loop:");
  forall c in C do
    writeln(c);

  writeln();
}
