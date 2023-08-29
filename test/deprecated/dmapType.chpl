config var n = 10;
{
  use BlockDist;
  var Dist = new dmap(new Block({1..n}));
  var Dom = {1..n} dmapped Dist;
  var A: [Dom] real;
  writeln(A);
  const Dist2: dmap(Block(1)) = new Block({1..10});
  writeln(Dist2.type:string);
}

{
  use CyclicDist;
  var Dist = new dmap(new Cyclic(1));
  var Dom = {1..n} dmapped Dist;
  var A: [Dom] real;
  writeln(A);
  const Dist2: dmap(Cyclic(1)) = new Cyclic(1);
  writeln(Dist2.type:string);
}

{
  use ReplicatedDist;
  var Dist = new dmap(new Replicated());
  var Dom = {1..n} dmapped Dist;
  var A: [Dom] real;
  writeln(A);
  const Dist2: dmap(Replicated) = new Replicated();
  writeln(Dist2.type:string);
}

{
  use StencilDist;
  var Dist = new dmap(new Stencil({1..n}));
  var Dom = {1..n} dmapped Dist;
  var A: [Dom] real;
  writeln(A);
  const Dist2: dmap(Stencil(1)) = new Stencil({1..n});
  writeln(Dist2.type:string);
}
