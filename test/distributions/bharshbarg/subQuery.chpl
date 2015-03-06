use BlockDist,
    CyclicDist,
    BlockCycDist,
    ReplicatedDist;

config const n = 10;

const Space = {1..n, 1..n};

const B = Space dmapped Block(Space);
test(B);

const C = Space dmapped Cyclic(startIdx = Space.low);
test(C);

const BC = Space dmapped BlockCyclic(startIdx = Space.low, blocksize = (2,2));
test(BC);

proc test(Dist) {
  const Data : [Dist] int;

  forall d in Data do d = here.id;

  for L in Locales {
    on L {
      if Data.hasSingleLocalSubdomain() {
        for d in Data[Data.localSubdomain()] do
          assert(d == here.id);
      } else {
        for sub in Data.localSubdomains() do
          for d in Data[sub] do
            assert(d == here.id);
      }
    }
  }
}
