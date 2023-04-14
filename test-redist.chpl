use BlockDist;

var n = 10;
var B = new dmap(new Block({1..10}));
var D = {1..n} dmapped B;
var A: [D] real;

writeln(B.type:string);
inspectDist(A);

for i in 1..1 {
  D = {1..0}; // reset domain to avoid need to preserve data
  n *= 2;
  B.redistribute({1..n});
  inspectDist(A);
  writeln("----- B's represenation -----");
  B.dsiDisplayRepresentation();
  writeln("----- D's represenation -----");
  D.dsiDisplayRepresentation();
  writeln("----- A's represenation -----");
  A.dsiDisplayRepresentation();
  writeln("---- about to assign D ----");
  D = {1..n};
  writeln("---- done assigning D -----");
  inspectDist(A);
  writeln("----- B's represenation -----");
  B.dsiDisplayRepresentation();
  writeln("----- D's represenation -----");
  D.dsiDisplayRepresentation();
  writeln("----- A's represenation -----");
  A.dsiDisplayRepresentation();
}

proc inspectDist(X: [] ?t) {
  writeln("-------------");
  for loc in Locales do
    on loc {
      writeln(here.id, ": ", X.localSubdomain(), " from ", X.domain);
      writeln(here.id, ": ", D.localSubdomain(), " from ", D);
      writeln(here.id, ": ", X.domain.localSubdomain(), " from ", X.domain);
      //      writeln(here.id, ": ", X.domain.dist._value.locDist, " from ", X.domain);
      writeln(here.id, ": ", X.domain.dist._instance.locDist, " from ", X.domain);
    }
  writeln("-------------");
}

proc (_distribution(?)).redistribute(const in newBbox) {
  /* First attempt, didn't work:
  var newDist = new dmap(new Block((...args)));
  this = newDist;
  */
  this._value.redistribute(newBbox);
}

proc Block.redistribute(const in newBbox) {
  const newBboxDims = newBbox.dims();
  const pid = this.pid;
  coforall (locid, loc, locdist) in zip(targetLocDom, targetLocales, locDist)
    do on loc {
      writeln("Redistributing on ", locid, " ", loc.id, " ", locdist.locale.id, " ", locDist[locid].myChunk.locale.id);
      this.boundingBox = newBbox;
      /* TODO: re-enable this, replacing previous statement
      const that = chpl_getPrivatizedCopy(this.type, pid);
      that.boundingBox = newBbox;
*/
      var inds = chpl__computeBlock(chpl__tuplify(locid), targetLocDom, newBbox, newBboxDims);
      //      locDist.myChunk = {(...inds)};
      locDist[locid].myChunk = {(...inds)};
      writeln("Locale ", here.id, " taking ", inds);
    }
}
