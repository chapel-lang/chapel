/*
Suggested TODO, in addition to implementing the actual computations:
- add a printout of grid/problem sizes etc. at start of program
- add verification that our cached slices survived after we have
  finished initializing everything.
  E.g. by dsiAccess-indexing into them and querying their domains.
- v1 messages can be now turned off (see util.chpl)
*/

use BlockDist;
use util;

config var chk = true;

/////////// grid of locales ///////////

// "x" -> first dimension
// from grid node (ix,iy): north (ix-1,iy); west (ix,iy-1)

config const gx=3, gy=3;
const gridDom = {1..gx, 1..gy};
const gridBig = {0..gx+1, 0..gy+1};

var gridLocales: [gridDom] locale = setupGridLocales();

const gridDist = gridDom dmapped Block(gridDom, gridLocales);

/////////// the computation data and index sets ///////////

// the size of node-local data
config const ldx=2, ldy=3;
type elType = real;

// the size of all data
const allx = ldx * gx,
      ally = ldy * gy;

// for neighbor-cache pointers
var auxArr: [1..1, 1..1] elType;
type cacheType = auxArr[1,..]._value.type?; // a class type, so it can be nil

///////////

// Node-local domains.
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
  infos = forall (ix,iy) in gridDist do
            new unmanaged LocalInfo(mygx=ix, mygy=iy);
}

proc GlobalInfo.deinit() {
  forall inf in infos do
    delete inf;
}

// Here are all our local domains. WI <- "Working Indices".
const WI = new unmanaged GlobalInfo();

assert(allx == WI.infos[gx,gy].myhighx, "a6 ");
assert(ally == WI.infos[gx,gy].myhighy, "a7");

///////////

// Node-local computation data.
class LocalData {
  const linfo: unmanaged LocalInfo;

  // the locale share of the compute data, plus neighbor cache
  var ldata: [linfo.domAlloc] elType;

  // pointers to slices of neighbor data
  // from which we will fetch into our neighbor cache
  var cnorth, csouth, cwest, ceast: cacheType;
}

// A class for all node-local data.
class GlobalData {
  const name: string;
  var datas: [gridDist] unmanaged LocalData;
}

// constructor for GlobalData
proc GlobalData.init(nameArg: string) {
  name=nameArg;
  datas = forall (inf, loc) in zip(WI.infos, gridLocales) do
            createAndCheckLocalData(inf, loc);

  proc createAndCheckLocalData(inf, loc) {
    const dat;
    dat = new unmanaged LocalData(inf);
    // sanity checks
    assert(dat.locale == loc);
    assert(dat.linfo.locale == loc);
    assert(dat.linfo == inf);
    return dat;
  }

  this.complete();

  /// get and store pointers to neighbor data slices ///

  serial(v1) do forall ((ix,iy), dat, inf) in zip(gridDist, datas, WI.infos) {
    msg1("neighbors for ", name, (ix, iy), "  domCompute=", inf.domCompute);
    dat.cnorth = storecache(-1,0, inf.mylowx-1,  inf.mylowy..inf.myhighy);
    dat.csouth = storecache(+1,0, inf.myhighx+1, inf.mylowy..inf.myhighy);
    dat.cwest = storecache(0,-1, inf.mylowx..inf.myhighx, inf.mylowy-1);
    dat.ceast = storecache(0,+1, inf.mylowx..inf.myhighx, inf.myhighy+1);

    proc storecache(dx, dy, slicex, slicey) {
      const ind = (ix+dx, iy+dy);
      if !gridDist.contains(ind) {
	msg1("  ", ind, "  no neighbor");
	return nil;
      }
      const nbr = datas[ind]; // our neighbor
      var result: cacheType;
      on nbr {
	msg1("  ", ind, "  slice at [", slicex, ",", slicey, "]");
        pragma "no auto destroy"
	ref slice = nbr.ldata[slicex, slicey];
	result = slice._value;
      }
      return result;
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
  if chk {
    if 1 <= ix && ix <= ldx * gx && 1 <= iy && iy <= ldy * gy
      then {/*OK*/}
      else halt("GlobalData.dsiAccess: index out of bounds ",
                (ix,ldx,gx), (iy,ldy,gy));
  }
  const gridx = (ix-1) / gx + 1, gridy = (iy-1) / gy + 1;
  return datas[gridx, gridy].ldata[ix,iy];
}

/////////// reference data ///////////

const refAlloc = {0..allx+1, 0..ally+1},
    refCompute = {1..allx, 1..ally},
  refAllocDist = refAlloc dmapped Block(refAlloc, gridLocales);

// Our reference data arrays, corresponding to WA, WB.
var RA, RB: [refAllocDist] elType;
