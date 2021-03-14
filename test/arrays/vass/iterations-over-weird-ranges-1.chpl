// The formulas in DefaultRectangularArr.getDataIndex() and
// DefaultRectangularArr.these() used to exhibit bugs for arrays
// over the domains [0..3 by 2 align 1, 0..5 by 3] and [-5..5 by 3 align 2]
// (from distributions/diten/{Dimensional,testCyclic}.chpl, resp.).

proc test(d) {
  writeln("testing ", d);
  var a: [d] int;
  var OK = true;
  for e in a do
    if e != 0 then
      OK = false;
  if !OK then writeln("  error in first loop");
  for (ix,e) in zip(d,a) do
    if e != 0 then
      writeln("  error in second loop: a", ix, " = ", e);
}

for param al1 in 0..3 do
  for param al2 in 0..5 do
    test({0..3 by 2 align al1, 0..5 by 3 align al2});

for param al1 in -5..5 do
  test({-5..5 by 3 align al1});
