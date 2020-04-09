use BlockDist;
config const N = 500 : int(64);

var Dist = new dmap(new Block(rank=1, idxType=int(64), boundingBox={0..#N}));
var Dom : domain(1, int(64)) dmapped Dist = {0..#N};

var Au : [Dom] real;

var vv : real;

forall (au,i) in zip(Au,0..#N) with (ref vv) do  // race is insignificant
  vv += au;
writeln(vv);
