use BlockDist,
    CyclicDist,
    BlockCycDist,
    ReplicatedDist,
    StencilDist;

config const n = 10;

const Space = {1..n, 1..n};

const B = Space dmapped Block(Space);
test(B);

const C = Space dmapped Cyclic(startIdx = Space.low);
test(C);

const BC = Space dmapped BlockCyclic(startIdx = Space.low, blocksize = (2,2));
test(BC);

const SC = Space dmapped Stencil(Space, fluff=(1,1));
test(SC);

proc test(Dist) {
  const Data : [Dist] int;

  forall d in Data do d = here.id;

  // check targetLocales
  for (distloc, domloc, arrloc) in zip(Dist.dist.targetLocales(),
                                       Dist.targetLocales(),
                                       Data.targetLocales()) {
    assert( distloc == domloc );
    assert( domloc == arrloc );
  }

  for L in Locales {
    on L {
      if Data.hasSingleLocalSubdomain() {
        // check domain.hasSingleLocalSubdomain matches array
        assert(Dist.hasSingleLocalSubdomain());
        // check array.localSubdomain
        for d in Data[Data.localSubdomain()] do
          assert(d == here.id);
        // check domain.localSubdomain
        for d in Data[Dist.localSubdomain()] do
          assert(d == here.id);
      } else {
        // check domain.hasSingleLocalSubdomain matches array
        assert(!Dist.hasSingleLocalSubdomain());
        // check array.localSubdomain
        for sub in Data.localSubdomains() do
          for d in Data[sub] do
            assert(d == here.id);
        // check domain.localSubdomain
        for sub in Dist.localSubdomains() do
          for d in Data[sub] do
            assert(d == here.id);
      }
    }
  }
}
