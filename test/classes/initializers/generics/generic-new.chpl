// This snippet is a heavily redacted version of a prototype of an
// effort to convert the Block distribution, from dists/BlockDist.chpl,
// from constructors to initializers.

// This generated a resolution error until the week of Mar 24, 2018
// This test locks on a work-around for the underlying issue.

const Space                       = {1..8, 1..8};
const OneLoc: [0..0, 0..0] locale = Locales[0];

proc main() {
  const B1 = new borrowed MyBlock(Space, OneLoc);
  const B2 = new borrowed MyBlock(Space);

  writeln('B1 ', B1);
  writeln('B2 ', B2);
}



class MyBlock {
  param rank: int;
  type  idxType          = int;
  type  sparseLayoutType = DefaultDist;

  proc init(boundingBox:   domain,
            targetLocales: [] locale  = Locales,

            param rank                = boundingBox.rank,
            type  idxType             = boundingBox.idxType,
            type  sparseLayoutType    = unmanaged DefaultDist) {
    this.rank             = rank;
    this.idxType          = idxType;
    this.sparseLayoutType = sparseLayoutType;
  }
}
