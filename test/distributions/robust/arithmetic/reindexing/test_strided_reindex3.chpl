use driver_domains;

const D: domain(1,int,true) dmapped Dist1D = Space1;

var A: [D] int;
A = -1;
ref ASlice = A[48..53];
// We expect this range to straddle multiple locales.
ref AReindex = ASlice.reindex(1..11 by 2);

forall a in AReindex do a = here.id;

{
  // Make sure we have at least two locales represented by the reindex.
  var locs : domain(int);
  for a in AReindex do locs.add(a);
  assert(locs.size > 1);
}

forall a in ASlice do assert(a == here.id);

writeln("SUCCESS");
