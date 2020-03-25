/*
Computes heat equation until convergence
Array is "manually" distributed on locales so each locale has a DR subblock 
Halo communications are implemented a la MPI: 
explicit Data[localeA].DR[sliceHalo]=Data[localeB].DR[sliceSource]
*/

public use BlockDist;
use util;

config const n=2, m=3: int;
config const g=3, h=3;
const gridDom = {1..g, 1..h};
var gridLocales: [gridDom] locale = setupGridLocales();

type elType = real;
//const adjcoords = ((0,-1), (-1,0), (0,1), (1,0));

record localInfo {
  var domAlloc = {0..n+1, 0..m+1};  // does not change
  var domCompute = {1..n, 1..m};    // does not change

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
forall (dat, loc) in zip(Data, gridLocales) {
  assert(here == dat.locale);
  assert(dat.locale == loc);
}

// reference computation

const globN = n * g, globM = m * h;
var domRef ={0..globN+1,0..globM+1};
var refdataB, refdataA: [domRef] elType;
var refdelta: elType;
var errCount = 0;

// oddphase: refdataB -> refdataA
proc refcomp(oddphase: bool, out delta: elType) {
  if oddphase {
    forall (i,j) in {1..globN, 1..globM} {
      refdataA[i,j] = (refdataB[i-1,j] + refdataB[i,j+1] + refdataB[i+1,j] + refdataB[i,j-1]) / 4;
    }
  } else {
    forall (i,j) in {1..globN, 1..globM} {
      refdataB[i,j] = (refdataA[i-1,j] + refdataA[i,j+1] + refdataA[i+1,j] + refdataA[i,j-1]) / 4;
    }
  }
  delta = max reduce [(i,j) in zip(1..globN, 1..globM)] abs(refdataB(i,j) - refdataA(i,j));
}
// oddphase: A vs. refdataA
proc verify(oddphase: bool) {
  var globdiff$: sync elType = min(elType);
  forall ((gi,gj), dat) in zip(gridDom, Data) {
    const locdiff = max reduce [(i,j) in dat.domCompute]
      abs( (if oddphase then dat.A[i,j] else dat.B[i,j]) -
	   (if oddphase then refdataA[work2ref(i,j,gi,gj)]
	                else refdataB[work2ref(i,j,gi,gj)]) );
    globdiff$ = max(globdiff$.readFE(), locdiff);
  } // forall

  if globdiff$.readXX() > 0.000001 {
    writeln("too much of a difference from reference: ", globdiff$.readXX());
    errCount += 1;
  }
}
proc work2ref(i,j,gi,gj) {
  return (n*(gi-1) + i, m*(gj-1) + j);
}

// initialize B

config const singleinit = true;
forall (dat, (gi,gj)) in zip(Data, gridDom) {
  if singleinit {
    dat.B[2,2] = 100;
    refdataB[work2ref(2,2,gi,gj)] = 100;
  } else {
    forall ((i,j), a) in zip(dat.domCompute, dat.Bcompute) {
      a = i*0.1 + j;
      refdataB[work2ref(i,j,gi,gj)] = a;
    }
  }
}

showref(false,  "After initialization");
verify(false);

// oddphase=true: fetch from B -> B; compute from B -> A

proc compute(oddphase: bool, out delta: elType) {
  forall dat in Data {

    if oddphase {
      forall (i,j) in dat.domCompute with (ref dat) do
        dat.A[i,j] = (dat.B[i,j-1] + dat.B[i-1,j] + dat.B[i,j+1] + dat.B[i+1,j]) / 4;

    } else {
      forall (i,j) in dat.domCompute with (ref dat) do
        dat.B[i,j] = (dat.A[i,j-1] + dat.A[i-1,j] + dat.A[i,j+1] + dat.A[i+1,j]) / 4;

    } // if oddphase

    dat.localDelta = max reduce [(a,b) in zip(dat.Acompute, dat.Bcompute)] abs(a-b);
  } // forall dat

  delta =  max reduce [dat in Data] dat.localDelta;
}

proc fetch(oddphase: bool) {
  forall (dat, (gi,gj)) in zip(Data, gridDom) {
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

var i=0;
proc progress() {
  i=i+1;
  fetch(true);          showfetch(true); 
  compute(true, delta); showme(true, delta, "After odd phase: "+i:string);
  refcomp(true, refdelta); showref(true);
  verify(true);
  if delta < epsilon then return true;

  i=i+1;
  fetch(false);          showfetch(false);
  compute(false, delta); showme(false, delta, "After even phase: "+i:string);
  refcomp(false, refdelta);showref(false);
  verify(false);
  if delta < epsilon then return true;

  return false;
}


do {} while !progress();

writeln("Converged with delta=",delta," after ",i," iterations.");
if errCount > 0 then writeln("Got ", errCount, " ERRORS.");
