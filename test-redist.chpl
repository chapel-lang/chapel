use BlockDist;

var n = 10;
var B = new dmap(new Block({1..10}));
var D = {1..n} dmapped B;
var A: [D] real;

inspectDist(A);

for i in 1..10 {
  D = {1..0}; // reset domain to avoid need to preserve data
  n *= 2;
  B.redistribute({1..n});
  D = {1..n};
  inspectDist(A);

/*
  writeln("----- B's represenation -----");
  B.dsiDisplayRepresentation();
  writeln("----- D's represenation -----");
  D.dsiDisplayRepresentation();
  writeln("----- A's represenation -----");
  A.dsiDisplayRepresentation();
*/
}

proc inspectDist(X: [] ?t) {
  writeln("-------------");
  for loc in Locales do
    on loc {
      writeln(here.id, ": ", X.localSubdomain(), " from ", X.domain);

      // Let's be quite sure these match...
      if D.localSubdomain() != X.localSubdomain() then
        writeln(here.id, ": ", D.localSubdomain(), " from ", D);
      if X.domain.localSubdomain() != X.localSubdomain() then
        writeln(here.id, ": ", X.domain.localSubdomain(), " from ", X.domain);
    }
  writeln("-------------");
}

proc Block.redistribute(const in newBbox) {
  const newBboxDims = newBbox.dims();
  const pid = this.pid;
  coforall (locid, loc, locdist) in zip(targetLocDom, targetLocales, locDist)
    do on loc {
      const that = chpl_getPrivatizedCopy(this.type, pid);
      that.boundingBox = newBbox;

      var inds = chpl__computeBlock(chpl__tuplify(locid), targetLocDom, newBbox, newBboxDims);
      locdist.myChunk = {(...inds)};
    }
}
