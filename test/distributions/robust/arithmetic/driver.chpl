public use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist, StencilDist;

enum DistType { default, block, cyclic, blockcyclic, replicated, stencil };

config param distType: DistType = if CHPL_COMM=="none" then DistType.default
                                                       else DistType.block;

config const n1 = 100;
config const n2 = 20;
config const n3 = 5;
config const n4 = 3;
config const n5 = 12345679:int(32);
config const o5 = 8;

const Space1 = {1..n1};
const Space2 = {1..n2, 1..n2};
const Space3 = {1..n3, 1..n3, 1..n3};
const Space4 = {1..n4, 1..n4, 1..n4, 1..n4};
const Space2D32 = {n5-o5:int(32)..n5, n5..n5+o5:int(32)};

proc setupDistributions(param DT : DistType) {
  if DT == DistType.default {
    return (
            defaultDist,
            defaultDist,
            defaultDist,
            defaultDist,
            defaultDist,
           );
  }
  if DT == DistType.block {
    return (
            new dmap(new Block(rank=1, boundingBox=Space1)),
            new dmap(new Block(rank=2, boundingBox=Space2)),
            new dmap(new Block(rank=3, boundingBox=Space3)),
            new dmap(new Block(rank=4, boundingBox=Space4)),
            new dmap(new Block(rank=2, idxType=int(32), boundingBox=Space2D32))
           );
  }
  if DT == DistType.cyclic {
    return (
            new dmap(new Cyclic(startIdx=0)),
            new dmap(new Cyclic(startIdx=(0,0))),
            new dmap(new Cyclic(startIdx=(0,0,0))),
            new dmap(new Cyclic(startIdx=(0,0,0,0))),
            new dmap(new Cyclic(startIdx=(0:int(32), 0:int(32))))
           );
  }
  if DT == DistType.blockcyclic {
    return (
            new dmap(new BlockCyclic(startIdx=(0,), blocksize=(3,))),
            new dmap(new BlockCyclic(startIdx=(0,0), blocksize=(3,3))),
            new dmap(new BlockCyclic(startIdx=(0,0,0), blocksize=(3,3,3))),
            new dmap(new BlockCyclic(startIdx=(0,0,0,0), blocksize=(3,3,3,3))),
            new dmap(new BlockCyclic(startIdx=(0:int(32),0:int(32)), blocksize=(2:int(32),3:int(32))))
           );
  }
  if DT == DistType.replicated {
    return (
            new dmap(new Replicated()),
            new dmap(new Replicated()),
            new dmap(new Replicated()),
            new dmap(new Replicated()),
            new dmap(new Replicated())
           );
  }
  if DT == DistType.stencil {
    return (
            new dmap(new Stencil(rank=1, boundingBox=Space1)),
            new dmap(new Stencil(rank=2, boundingBox=Space2)),
            new dmap(new Stencil(rank=3, boundingBox=Space3)),
            new dmap(new Stencil(rank=4, boundingBox=Space4)),
            new dmap(new Stencil(rank=2, idxType=int(32), boundingBox=Space2D32))
           );
  }
  halt("unexpected 'distType': ", DT);
}

const (Dist1D, Dist2D, Dist3D, Dist4D, Dist2D32) = setupDistributions(distType);

//
// creates a tuple of size 'rank' initialized with values 'x'
//
proc fill(param rank, x) {
  if rank == 1 {
    var y: 1*x.type;
    y(0) = x;
    return y;
  } else if rank == 2 {
    return (x, x);
  } else {
    return (x, (...fill(rank-1, x)));
  }
}

//
// creates a domain of rank 'rank' with each dimension equal to 'extent'
//
proc rankDomain(param rank, extent) {
  return {(...fill(rank, extent))};
}

//
// returns a next integer starting at 1
//
var next_i = 0;
proc next() { next_i += 1; return next_i; }

//
// Compare C pointers (there are cases where we want to do this for
//  testing purposes)
//
inline
proc dist_eq(a, b) return __primitive("ptr_eq", a._value:object, b._value:object);
inline
proc dist_neq(a, b) return __primitive("ptr_neq", a._value:object, b._value:object);
