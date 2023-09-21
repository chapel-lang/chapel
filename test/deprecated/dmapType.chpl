config var n = 10, leaksAndBreaks=false;
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

{
  // Private distributions leak, and it's not clear they're
  // ready for prime-time which is why this test of the
  // deprecation warning puts most of the code into a
  // conditional that doesn't run.  This checks that the
  // deprecation warnings still fire without dealing with
  // the leaks.  The small test outside of the conditional
  // is leak-free and shows that the type exists even if
  // it doesn't do anything interesting with it.

  use PrivateDist;
  proc foo(type t) { writeln("foo got ", t: string); }
  foo(Private);

  if leaksAndBreaks {
    var Dist = new dmap(new Private());
    var Dom: domain(1) dmapped Dist;
    var A: [Dom] real;
    writeln(A);
    const Dist2: dmap(Private) = new Private();
    writeln(Dist2.type:string);
  }
}

{
  use BlockCycDist;
  var Dist = new dmap(new BlockCyclic(1, 2));
  var Dom = {1..n} dmapped Dist;
  var A: [Dom] real;
  writeln(A);
  const Dist2: dmap(BlockCyclic(1)) = new BlockCyclic(1, 2);
  writeln(Dist2.type:string);
}

{
  use DimensionalDist2D, BlockDim;
  var lpd = sqrt(numLocales):int;
  var MyLocDom = {0..<lpd, 0..<lpd};
  var MyLocs: [MyLocDom] locale = reshape(Locales, MyLocDom);
  var dimA = new BlockDim(numLocales=lpd, 1..n);
  var dimB = new BlockDim(numLocales=lpd, 1..n);
  var Dist = if leaksAndBreaks
               then new dmap(new DimensionalDist2D(MyLocs, dimA, dimB))
               else new DimensionalDist2D(MyLocs, dimA, dimB);
  var Dom = {1..n, 1..n} dmapped Dist;
  var A: [Dom] real;
  writeln(A);

  var dim1 = new BlockDim(numLocales=lpd, 1..n);
  var dim2 = new BlockDim(numLocales=lpd, 1..n);
  const Dist2: dmap(DimensionalDist2D(MyLocs.type, dim1.type, dim2.type, int))
             = new DimensionalDist2D(MyLocs, dim1, dim2);
  writeln(Dist2.type:string);
}

{
  use HashedDist;
  var Dom: domain(int) dmapped new dmap(new Hashed(idxType=int));;
  Dom += 1;
  var A: [Dom] real;
  writeln(A);
  const Dist2: dmap(Hashed(int, DefaultMapper)) = new Hashed(int);
  writeln(Dist2.type:string);
}
