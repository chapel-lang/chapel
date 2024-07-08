use BlockDist;
use CopyAggregation;

config const numUpdates = 10;
var Src = [1, 2, 3, 4, 5];
var Dest: [0..<numUpdates] int;

{
  const D = {0..numUpdates-1};
  var Inds: [D] int;

  for i in D do
    Dest[i] = Src[Inds[i]];
}

{
  const D = {0..numUpdates-1};
  var Inds: [D] int;
  forall i in D with (ref Dest) do
    Dest[i] = Src[Inds[i]];
}

{
  const D = blockDist.createDomain({0..numUpdates-1});
  var Inds: [D] int;

  forall i in D with (ref Dest) do
    Dest[i] = Src[Inds[i]];
}

{
  const D = blockDist.createDomain({0..numUpdates-1});
  var Inds: [D] int;

  forall (d, i) in zip(Dest, Inds) with (var agg = new SrcAggregator(int)) do
    agg.copy(d, Src[i]);
}
