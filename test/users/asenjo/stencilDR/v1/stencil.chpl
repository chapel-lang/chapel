use BlockDist;
use util;

config const n=2, m=3: int;
config const g=3, h=3;
const gridDom = {1..g, 1..h};
var gridLocales: [gridDom] locale;
setupGridLocales();

type elType = real;
const adjcoords = ((0,-1), (-1,0), (0,1), (1,0));

record localInfo {
  const domAlloc = {0..n+1, 0..m+1};
  const domCompute = {1..n, 1..m};

  var A, B: [domAlloc] elType;

  proc Acompute return A[domCompute];
  proc Bcompute return B[domCompute];

  var localDelta: elType;
}

// set up the data arrays
const gridDist = gridDom dmapped Block(gridDom, gridLocales);
var Data: [gridDist] localInfo;
var delta: elType;
config const epsilon = 0.1;

// make sure we got it right
forall (dat, loc) in (Data, gridLocales) {
  assert(here == dat.locale);
  assert(dat.locale == loc);
}

// initialize B

config const singleinit = true;
forall dat in Data {
  if singleinit {
    dat.B[2,2] = 100;
  } else {
    forall ((i,j), a) in (dat.domCompute, dat.Bcompute) do
      a = i*0.1 + j;
  }
}

showme(false, 0, "After initialization");

// oddphase=true: fetch from B -> B; compute from B -> A

proc compute(oddphase: bool, out delta: elType) {
  forall dat in Data {

    if oddphase {
      forall (i,j) in dat.domCompute do
        dat.A[i,j] = (dat.B[i,j-1] + dat.B[i-1,j] + dat.B[i,j+1] + dat.B[i+1,j]) / 4;

    } else {
      forall (i,j) in dat.domCompute do
        dat.B[i,j] = (dat.A[i,j-1] + dat.A[i-1,j] + dat.A[i,j+1] + dat.A[i+1,j]) / 4;

    } // if oddphase

    dat.localDelta = max reduce [(a,b) in (dat.Acompute, dat.Bcompute)] abs(a-b);
  } // forall dat

  delta =  max reduce [dat in Data] dat.localDelta;
}

proc fetch(oddphase: bool) {
  forall (dat, (gi,gj)) in (Data, gridDom) {
    if oddphase {
      if gi > 1 then dat.B[0,   1..m] = Data[gi-1, gj].B[n, 1..m]; // north
      if gi < g then dat.B[n+1, 1..m] = Data[gi+1, gj].B[1, 1..m]; // south
      if gj > 1 then dat.B[1..n, 0]   = Data[gi, gj-1].B[1..n, m]; // west
      if gj < h then dat.B[1..n, m+1] = Data[gi, gj+1].B[1..n, 1]; // east
    } else {
      if gi > 1 then dat.A[0,   1..m] = Data[gi-1, gj].A[n, 1..m]; // north
      if gi < g then dat.A[n+1, 1..m] = Data[gi+1, gj].A[1, 1..m]; // south
      if gj > 1 then dat.A[1..n, 0]   = Data[gi, gj-1].A[1..n, m]; // west
      if gj < h then dat.A[1..n, m+1] = Data[gi, gj+1].A[1..n, 1]; // east
    }
  } // forall
}

proc progress() {
  fetch(true);          showfetch(true);
  compute(true, delta); showme(true, delta, "After step 1");
  if delta < epsilon then return true;

  fetch(false);          showfetch(false);
  compute(false, delta); showme(false, delta, "After step 2");
  if delta < epsilon then return true;

  return false;
}

do {} while !progress();
