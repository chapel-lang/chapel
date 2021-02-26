
iter willthrow() throws {
  for idx in 5..8 do
    if idx == 7 then
      throw new owned Error();
    else
      yield idx;
}

// the behavior should not change when adding this pragma:
// NOTE: we need the second argument to satisfy compiler assertions
pragma "init copy fn"
proc driver(ARG, definedConst: bool) {
  writeln("driver start");

  for idx1 in willthrow() do
    writeln("driver idx1 ", idx1);

  // we can do it again
  for idx2 in willthrow() do
    writeln("driver idx2 ", idx2);

  // ... or iterate over 'ARG'
  for idx3 in ARG do
    writeln("driver idx3 ", idx3);

  writeln("driver finish");
  return 555;
}

proc main throws {
  writeln("main start");
  var BBB = driver(willthrow(), definedConst=false);
  writeln("main finish");
}
