// this test verifies correct operation of the domain follower

proc testPP(param strideDom, param strideFT) {
  if strideDom == 0 || strideFT == 0 then return;
  var d = {1..15 by strideDom};
  var ft = 0..4 by strideFT;
  writeln("===  param strideDom ", strideDom, "  param strideFT ", strideFT);
  for i in d.these(iterKind.follower, (ft,)) do
    writeln(i);
}
proc testPC(param strideDom, const strideFT) {
  if strideDom == 0 || strideFT == 0 then return;
  var d = {1..15 by strideDom};
  var ft = 0..4 by strideFT;
  writeln("===  param strideDom ", strideDom, "  const strideFT ", strideFT);
  for i in d.these(iterKind.follower, (ft,)) do
    writeln(i);
}
proc testCP(const strideDom, param strideFT) {
  if strideFT == 0 || strideDom == 0 then return;
  var d = {1..15 by strideDom};
  var ft = 0..4 by strideFT;
  writeln("===  const strideDom ", strideDom, "  param strideFT ", strideFT);
  for i in d.these(iterKind.follower, (ft,)) do
    writeln(i);
}
proc testCC(const strideDom, const strideFT) {
  if strideDom == 0 || strideFT == 0 then return;
  var d = {1..15 by strideDom};
  var ft = 0..4 by strideFT;
  writeln("===  const strideDom ", strideDom, "  const strideFT ", strideFT);
  for i in d.these(iterKind.follower, (ft,)) do
    writeln(i);
}

for param st in -3..3 {
  for param ft in -3..3 {
    testPP(st,ft);
    testPC(st,ft);
    testCP(st,ft);
    testCC(st,ft);
  }
}
