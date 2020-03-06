use BlockDist;

config const gx=3, gy=3;
const gridDom = {1..gx, 1..gy};
const gridBig = {0..gx+1, 0..gy+1};

var manylocs: bool;
config const showlocales = false;
var gridLocales: [gridDom] locale = setupGridLocales();

proc setupGridLocales(ensureManyLocs = false) {
  var gridLocales: [gridDom] locale?;
  manylocs = (numLocales >= gridLocales.numElements);

  if manylocs {
    var i = 0;
    for ml in gridLocales { ml = Locales(i); i += 1; }
    if showlocales {
      write("set up with ", i, " distinct locales");
      if i < numLocales then write(" out of ", numLocales);
      writeln();
    }
  } else {
    gridLocales = Locales(0);
    if showlocales then
      writeln("oversubscribed Locales(0) over ", gridLocales.numElements,
              " locales");
  }

  if !manylocs && ensureManyLocs then halt("not enough locales: wanted ",
    gridLocales.numElements, ", got ", numLocales);
  writeln();

  return gridLocales!;
}

const gridDist = gridDom dmapped Block(gridDom, gridLocales);

config var chk = true;
config const v1 = true;
proc msg1(args...) { if v1 then writeln((...args)); }

// the size of node-local data
config const ldx=2, ldy=3;
type elType = real;

// for neighbor-cache pointers
var auxArr: [1..1, 1..1] elType;
type cacheType = auxArr[1,..]._value.type; // a class type, so it can be nil

class LocalInfo {
  // our coordinates on the grid (perhaps not strictly needed)
  const mygx, mygy: int;
  // our coordinates in the data space
  const myhighx = mygx * ldx,
        mylowx  = myhighx - ldx + 1,
        myhighy = mygy * ldy,
        mylowy  = myhighy - ldy + 1;

  // frequently-used domains
  const domAlloc = {mylowx-1 .. myhighx+1, mylowy-1 .. myhighy+1};
  const domCompute = {mylowx..myhighx, mylowy..myhighy};
}

// A class for all the node-local domains.
class GlobalInfo {
  var infos: [gridDist] unmanaged LocalInfo;
}

// constructor for GlobalInfo
proc GlobalInfo.init() {
  const dummyLI = new unmanaged LocalInfo();
  infos = dummyLI;
  this.complete();
  coforall ((ix,iy), inf, loc) in zip(gridDist, infos, gridLocales) do on loc {
    inf = new unmanaged LocalInfo(mygx=ix, mygy=iy);
  }
  delete dummyLI;
}

proc GlobalInfo.deinit() {
  forall inf in infos do
    delete inf;
}

// Here are all our local domains. WI <- Working Indices.
const WI = new unmanaged GlobalInfo();

///////////

// Node-local computation data.
class LocalData {
  const linfo: unmanaged LocalInfo;

  // the locale share of the compute data, plus neighbor cache
  var ldata: [linfo.domAlloc] elType;

  // pointers to slices of neighbor data
  // from which we will fetch into our neighbor cache
  var cnorth, csouth, cwest, ceast: cacheType?;
}

// A class for all node-local data.
class GlobalData {
  const name: string;
  var datas: [gridDist] unmanaged LocalData;
}

// constructor for GlobalData
proc GlobalData.init(nameArg: string) {
  name=nameArg;
  var datasTemp: [gridDist] unmanaged LocalData?;
  coforall (inf, datElm, loc) in zip(WI.infos, datasTemp, gridLocales) do on loc {
    const dat;
    dat = new unmanaged LocalData(inf);
    // sanity checks
    assert(dat.locale == loc);
    assert(dat.linfo.locale == loc);
    assert(dat.linfo == inf);
    datElm = dat;
  }
  this.datas = datasTemp!;
  this.complete();

  /// get and store pointers to neighbor data slices ///
  forall ((ix,iy), dat, inf) in zip(gridDist, datas, WI.infos) {
    msg1("neighbors for ", name, (ix, iy), "  domCompute ", inf.domCompute);
    storecache(dat.cnorth, -1,0, inf.mylowx-1,  inf.mylowy..inf.myhighy);
    storecache(dat.csouth, +1,0, inf.myhighx+1, inf.mylowy..inf.myhighy);
    storecache(dat.cwest, 0,-1, inf.mylowx..inf.myhighx, inf.mylowy-1);
    storecache(dat.ceast, 0,+1, inf.mylowx..inf.myhighx, inf.myhighy+1);

    proc storecache(ref cache, dx, dy, slicex, slicey) {
      const ind = (ix+dx, iy+dy);
      if !gridDist.contains(ind) {
        msg1("  ", ind, "  no neighbor");
        return;
      }
      const nbr = datas[ind]; // our neighbor
      on nbr {
        msg1("  ", ind, "  slice at [", slicex, ",", slicey, "]");
        pragma "no auto destroy"
          ref slice = nbr.ldata[slicex, slicey];
        cache = slice._value;
      }
    }  // storecache()
  }  // forall
}  // GlobalData constructor

proc GlobalData.deinit() {
  forall dat in datas do
    delete dat;
}

// Our two global arrays, to switch between.
const WA = new unmanaged GlobalData("WA"),
      WB = new unmanaged GlobalData("WB");

proc deinit() {
  delete WA;
  delete WB;
  delete WI;
}

// Reuse the name for an indexing operation.
// This does not access neighbor caches.
proc GlobalData.dsiAccess(ix,iy) ref {
  if chk then assert(1 <= ix && ix <= ldx * gx && 1 <= iy && iy <= ldy * gy);
  const gridx = (ix-1) / gx + 1, gridy = (iy-1) / gy + 1;
  return datas[gridx, gridy].ldata[ix,iy];
}

/////////// reference data ///////////

const refx = ldx * gx,
      refy = ldy * gy;
assert(refx == WI.infos[gx,gy].myhighx);
assert(refy == WI.infos[gx,gy].myhighy);

const refAlloc = {0..refx+1, 0..refy+1},
    refCompute = {1..refx, 1..refy},
  refAllocDist = refAlloc dmapped Block(refAlloc, gridLocales);

// Our reference data arrays, corresponding to WA, WB.
var RA, RB: [refAllocDist] elType;
//Helper functions for stencil.chpl

