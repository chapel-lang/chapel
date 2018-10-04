
iter willthrow() throws {
  for idx in 5..8 do
    if idx == 7 then
      throw new Error();
    else
      yield idx;
}

// the behavior should not change when adding this pragma:
pragma "init copy fn"
proc driver(ARG) {
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
  var BBB = driver(willthrow());
  writeln("main finish");
}
