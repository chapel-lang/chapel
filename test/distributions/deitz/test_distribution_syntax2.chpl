config const n: int = 4;

{
  use BlockDist;

  var Dist: dmap(unmanaged Block(rank=2)) = new dmap(new unmanaged Block(boundingBox={1..n,1..n}));
  var Dom: domain(2) dmapped Dist = {1..n,1..n} dmapped Dist;
  var Arr: [Dom] 2*int;

  forall (i,j) in Dom {
    Arr(i,j) = (here.id,(i-1)*n + j);
  }
  writeln("Block-Distributed Array");
  writeln("-----------------------");
  writeln(Arr);
}

{
  use CyclicDist;

  var Dist: dmap(unmanaged Cyclic(rank=2)) = new dmap(new unmanaged Cyclic(startIdx=(1,1)));
  var Dom: domain(2) dmapped Dist = {1..n,1..n} dmapped Dist;
  var Arr: [Dom] 2*int;

  forall (i,j) in Dom {
    Arr(i,j) = (here.id,(i-1)*n + j);
  }
  writeln("Cyclic-Distributed Array");
  writeln("------------------------");
  writeln(Arr);
}
