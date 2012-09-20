use BlockDist;
use util;

config var chk = true;
config var init1 = true;

// recommendation: print out sizes, grid, etc.
// at start of program

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
  coforall (inf, dat, loc) in (WI.infos, datas, gridLocales) {
    dat = new LocalData(inf);
    // sanity checks
    assert(dat.locale == loc);
    assert(dat.linfo.locale == loc);
    assert(dat.linfo == inf);
  }

  /// get and store pointers to neighbor data slices ///

  // TODO: work around "error: unexpected case [parallel.cpp:528]"
  // when using 'serial(v1) do forall' instead of 'for'
  for ((ix,iy), dat, inf) in (gridDist, datas, WI.infos) {
    msg1("neighbors for ", name, (ix, iy), "  domCompute ", inf.domCompute);
    storecache(dat.cnorth, -1,0, inf.mylowx-1,  inf.mylowy..inf.myhighy);
    storecache(dat.csouth, +1,0, inf.myhighx+1, inf.mylowy..inf.myhighy);
    storecache(dat.cwest, 0,-1, inf.mylowx..inf.myhighx, inf.mylowy-1);
    storecache(dat.ceast, 0,+1, inf.mylowx..inf.myhighx, inf.myhighy+1);

    proc storecache(ref cache, dx, dy, slicex, slicey) {
      const ind = (ix+dx, iy+dy);
      if !gridDist.member(ind) {
	msg1("  ", ind, "  no neighbor");
	return;
      }
      const nbr = datas[ind]; // our neighbor
      on nbr {
	msg1("  ", ind, "  slice at [", slicex, ",", slicey, "]");
	var slice => nbr.ldata[slicex, slicey];
	cache = slice._value;
	if !noRefCount then cache._arrCnt.add(1);  // this is a bit low-level
      }
    }  // storecache()

  }  // coforall
}  // GlobalData constructor

// Our two global arrays, to switch between.
const WA = new GlobalData("WA"),
      WB = new GlobalData("WB");

// Reuse the name for an indexing operation.
// This does not access neighbor caches.
proc GlobalData.dsiAccess(ix,iy) var {
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

use stencil;

// verboseness
config const v1 = true, v2 = true;
proc msg1(args...) { if v1 then writeln((...args)); }
proc msg2(args...) { if v2 then writeln((...args)); }

// gridLocales setup
var manylocs: bool;
proc setupGridLocales(ensureManyLocs = false) {
  manylocs = (numLocales >= gridLocales.numElements);

  if manylocs {
    var i = 0;
    for ml in gridLocales { ml = Locales(i); i += 1; }
    write("set up with ", i, " distinct locales");
    if i < numLocales then write(" out of ", numLocales);
    writeln();
  } else {
    gridLocales = Locales(0);
    writeln("oversubscribed Locales(0) over ", gridLocales.numElements, " locales");
  }

  if !manylocs && ensureManyLocs then halt("not enough locales: wanted ",
    gridLocales.numElements, ", got ", numLocales);
  writeln();
}

// show what we have
config const vcalc = true, vsend = true;
const fpstyle = new iostyle(realfmt = 1, precision = 2, min_width = 6);

/*
proc showme(oddphase: bool, delta: elType, msg = "") {
  if !vcalc then return;
  writeln(msg, "  delta ", delta, if oddphase then "  (B->A)" else "  (A->B)");
  writeln();

  for (gix, dat) in (gridDom, Data) {
    writeln("Grid point ", gix, " on locale ", dat.locale.id);
    if oddphase {
      writeln("B =");
      writeln(dat.B, fpstyle);
      writeln("A =");
      writeln(dat.A, fpstyle);
    } else {
      writeln("A =");
      writeln(dat.A, fpstyle);
      writeln("B =");
      writeln(dat.B, fpstyle);
    }
    writeln();
  }
}


proc showref(oddphase: bool, msg = "") {
  if !vcalc then return;
  writeln(msg);
  writeln();

  if oddphase {
    writeln("refdataA =");
    writeln(refdataA, fpstyle);
  } else {
    writeln("refdataB =");
    writeln(refdataB, fpstyle);
  }
  writeln();
}


//const showdummy: [0..n+1, 0..m+1] eltype;
const colsep = "  ";
proc showdummyrow() {
  const width = (m+2) * fpstyle.min_width;
  for 1..width do write(" ");
}
proc showrealrow(oddphase: bool, gi:int, gj:int, i:int) {
  if oddphase then
    showrealrow(Data[gi,gj].B, i);
  else
    showrealrow(Data[gi,gj].A, i);
}
proc showrealrow(ARR, i:int) {
  for j in 0..m+1 do write(ARR[i,j], fpstyle);
}

proc showfetch(oddphase: bool, msg = "") {
  if !vsend then return;
  writeln(msg, if oddphase then " (fetch B->B)" else " (fetch A->A)");
  writeln();

  const innerGrid = { if g <= 2 then 1..1 else 2..g-1,
                      if h <= 2 then 1..1 else 2..h-1 };
  for ((gi,gj), dat) in (innerGrid, Data[innerGrid]) {
    writeln("Grid point ", (gi, gj), " on locale ", dat.locale.id);
    // first row
    if gi > 1 {
      for i in 0..n+1 {
        if gj > 1 then showrealrow(oddphase, gi-1, gj-1, i); else showdummyrow();
        write(colsep);
        showrealrow(oddphase, gi-1, gj, i);
        write(colsep);
        if gj < h then showrealrow(oddphase, gi-1, gj+1, i); else showdummyrow();
        writeln();
      }
      writeln("-----------");
    } else {
      writeln("no first row");
    }
    // second row - always there
    if true {
      for i in 0..n+1 {
        if gj > 1 then showrealrow(oddphase, gi, gj-1, i); else showdummyrow();
        write(colsep);
        showrealrow(oddphase, gi, gj, i);
        write(colsep);
        if gj < h then showrealrow(oddphase, gi, gj+1, i); else showdummyrow();
        writeln();
      }
      writeln("-----------");
    }
    // third row
    if gi < g {
      for i in 0..n+1 {
        if gj > 1 then showrealrow(oddphase, gi+1, gj-1, i); else showdummyrow();
        write(colsep);
        showrealrow(oddphase, gi+1, gj, i);
        write(colsep);
        if gj < h then showrealrow(oddphase, gi+1, gj+1, i); else showdummyrow();
        writeln();
      }
    } else {
      writeln("no third row");
    }
    writeln();
  } // for gridDom

  writeln("done ", msg);
  writeln();
}

proc computeOld(oddphase: bool, out delta: elType) {
  forall dat in Data {

    if oddphase {

      forall ((i,j), a, b1, b2, b3, b4) in (dat.domCompute, dat.Acompute,
        dat.B[dat.domCompute.translate(adjcoords(1))],
        dat.B[dat.domCompute.translate(adjcoords(2))],
        dat.B[dat.domCompute.translate(adjcoords(3))],
        dat.B[dat.domCompute.translate(adjcoords(4))])
      {
        a = (b1 + b2 + b3 + b4) / 4;
      }

    } else { //  !oddphase

      forall ((i,j), a, b1, b2, b3, b4) in (dat.domCompute, dat.Bcompute,
        dat.A[dat.domCompute.translate(adjcoords(1))],
        dat.A[dat.domCompute.translate(adjcoords(2))],
        dat.A[dat.domCompute.translate(adjcoords(3))],
        dat.A[dat.domCompute.translate(adjcoords(4))])
      {
        a = (b1 + b2 + b3 + b4) / 4;
      }

    } // if oddphase

    dat.localDelta = max reduce [(a,b) in (dat.Acompute, dat.Bcompute)] abs(a-b);
  } // forall dat

  delta =  max reduce [dat in Data] dat.localDelta;
}
*/