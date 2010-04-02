config const n: int = 4;

{
  use BlockDist;

  var Dist: dist(Block(rank=2)) = new dist(new Block(boundingBox=[1..n,1..n]));
  var Dom: domain(2) distributed Dist = [1..n,1..n] distributed Dist;
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

  var Dist: dist(Cyclic(rank=2)) = new dist(new Cyclic(rank=2,low=(1,1)));
  var Dom: domain(2) distributed Dist = [1..n,1..n] distributed Dist;
  var Arr: [Dom] 2*int;

  forall (i,j) in Dom {
    Arr(i,j) = (here.id,(i-1)*n + j);
  }
  writeln("Cyclic-Distributed Array");
  writeln("------------------------");
  writeln(Arr);
}
