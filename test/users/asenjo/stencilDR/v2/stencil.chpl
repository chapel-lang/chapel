/*
- make 'forall gridDist' distribute over locales
- ensure all component objects get created on the right locales as intended
- add a printout of sizes, grid, etc. at start of program
- add verification that our cached slices survived after we have
  finished initializing everything.
  E.g. by dsiAccess-indexing into them.
 - remove the debugging writeln() and re-enable asserts a1-a3
*/




use BlockDist;
use util;

config var chk = true;
config var init1 = true;

/////////// grid of locales ///////////

// "x" -> first dimension
// from grid node (ix,iy): north (ix-1,iy); west (ix,iy-1)

config const gx=3, gy=3;
const gridDom = {1..gx, 1..gy};
const gridBig = {0..gx+1, 0..gy+1};

var gridLocales: [gridDom] locale;
setupGridLocales();
const gridDist = gridDom dmapped Block(gridDom, gridLocales);

/////////// the computation data and index sets ///////////

// the size of node-local data
config const ldx=2, ldy=3;
type elType = real;

// for neighbor-cache pointers
var auxArr: [1..1] elType;
type cacheType = auxArr._value.type; // a class type, so it can be nil

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
  var infos: [gridDist] LocalInfo;
}

// constructor for GlobalInfo
proc GlobalInfo.GlobalInfo() {
  coforall ((ix,iy), inf) in (gridDist, infos) {
    inf = new LocalInfo(mygx=ix, mygy=iy);
    writeln("GlobalInfo constructor ", (ix,iy), "  on ", here.id,
	    "  inf.locale=", inf.locale.id);
  }
}

// Here are all our local domains. WI <- Working Indices.
const WI = new GlobalInfo();

///////////

// Node-local computation data.
class LocalData {
  const linfo: LocalInfo;

  // the locale share of the compute data, plus neighbor cache
  var ldata: [linfo.domAlloc] elType;

  // pointers to slices of neighbor data
  // from which we will fetch into our neighbor cache
  var cnorth, csouth, cwest, ceast: cacheType;
}

// A class for all node-local data.
class GlobalData {
  const name: string;
  var datas: [gridDist] LocalData;
}

// constructor for GlobalData
proc GlobalData.GlobalData(nameArg: string) {
  name=nameArg;
  coforall (_, inf, dat, loc) in (gridDist, WI.infos, datas, gridLocales) {
    dat = new LocalData(inf);
    writeln("GlobalData constructor for ", (inf.mygx, inf.mygy),
	    "  here=", here.id,
	    "  loc=", loc.id,
	    "  dat.locale=", dat.locale.id);
    // sanity checks
    if false {
    assert(dat.locale == loc, "a1");
    assert(dat.linfo.locale == loc, "a2");
    assert(dat.linfo == inf, "a3");
    }
  }
  exit(0); //vass
  /// get and store pointers to neighbor data slices ///

  serial(v1) do forall ((ix,iy), dat, inf) in (gridDist, datas, WI.infos) {
    msg1("neighbors for ", name, (ix, iy), "  domCompute ", inf.domCompute);
    dat.cnorth = storecache(-1,0, inf.mylowx-1,  inf.mylowy..inf.myhighy);
    dat.csouth = storecache(+1,0, inf.myhighx+1, inf.mylowy..inf.myhighy);
    dat.cwest = storecache(0,-1, inf.mylowx..inf.myhighx, inf.mylowy-1);
    dat.ceast = storecache(0,+1, inf.mylowx..inf.myhighx, inf.myhighy+1);

    proc storecache(dx, dy, slicex, slicey) {
      const ind = (ix+dx, iy+dy);
      if !gridDist.member(ind) {
	msg1("  ", ind, "  no neighbor");
	return nil;
      }
      const nbr = datas[ind]; // our neighbor
      var result: cacheType;
      on nbr {
	msg1("  ", ind, "  slice at [", slicex, ",", slicey, "]");
	var slice => nbr.ldata[slicex, slicey];
	result = slice._value;
	if !noRefCount then result._arrCnt.add(1);  // this is a bit low-level
      }
      return result;
    }  // storecache()

  }  // coforall
}  // GlobalData constructor

// Our two global arrays, to switch between.
const WA = new GlobalData("WA"),
      WB = new GlobalData("WB");

// Reuse the name for an indexing operation.
// This does not access neighbor caches.
proc GlobalData.dsiAccess(ix,iy) var {
  if chk then assert(1 <= ix && ix <= ldx * gx && 1 <= iy && iy <= ldy * gy,
		     "a5 ", (ix,ldx,gx), (iy,ldy,gy));
  const gridx = (ix-1) / gx + 1, gridy = (iy-1) / gy + 1;
  return datas[gridx, gridy].ldata[ix,iy];
}

/////////// reference data ///////////

const refx = ldx * gx,
      refy = ldy * gy;
assert(refx == WI.infos[gx,gy].myhighx, "a6 ");
assert(refy == WI.infos[gx,gy].myhighy, "a7");

const refAlloc = {0..refx+1, 0..refy+1},
    refCompute = {1..refx, 1..refy},
  refAllocDist = refAlloc dmapped Block(refAlloc, gridLocales);

// Our reference data arrays, corresponding to WA, WB.
var RA, RB: [refAllocDist] elType;
