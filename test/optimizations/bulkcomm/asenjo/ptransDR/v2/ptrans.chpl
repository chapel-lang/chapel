/*
Traspose a given matrix B to matrix A
Array is "manually" distributed on locales so each locale has a DR subblock 
Rows to columns communications are implemented a la MPI: 
explicit Data[localeA].DR[slicedest]=Data[localeB].DR[sliceSource]
*/

public use BlockDist;
use util;

config const n=2, m=4: int;
config const g=3, h=3;
const gridDom = {1..g, 1..h};
var gridLocales: [gridDom] locale = setupGridLocales();

type elType = real;

record localInfo {
  var domA = {1..n, 1..m};  // does not change
  var domB = {1..m, 1..n};  // does not change

  var A, C: [domA] elType;
  var B: [domB] elType;
}

// set up the data arrays
const gridDist = gridDom dmapped Block(gridDom, gridLocales);
var Data: [gridDist] localInfo;

// make sure we got it right
forall (dat, loc) in zip(Data, gridLocales) {
  assert(here == dat.locale);
  assert(dat.locale == loc);
}

config const singleinit = false;
var errCount: atomic int;

//showfetch(true);
init();
transpose();          
showfetch(false);
verify();
//writeln("Done");
if errCount.read() != 0 then writeln(errCount.read(), " ERRORS");

/////////////////////////////////

proc init(){
  forall (dat, (gi,gj)) in zip(Data, gridDom) {
    if singleinit {
      dat.B[2,2] = 99;
    } else {
      forall ((i,j), b) in zip(dat.domB, dat.B) {
	b = i+ j*0.1;
      }
    }
  }
}

proc verify() {
  forall (dat, (gi,gj)) in zip(Data, gridDom) {
    proc check(act, exp, i, j) {
      if act == exp then return;
      errCount.add(1);
      writeln("error at ", (gi,gj), (i,j), ": expected ", exp, "  actual ", act);
    }
    if singleinit {
      check(dat.A[2,2], 99, 2, 2);
    } else {
      forall ((j,i), b) in zip(dat.domA, dat.A) {
	check(b, i+ j*0.1, j, i);
      }
    }
  }
}

proc transposeColumnwise() {
  var k: int;
  forall (dat, (gi,gj)) in zip(Data, gridDom) 
  {
     for k in 1..m do {
         dat.A[1..n,k] = Data[gj, gi].B[k, 1..n];
     } // copy B rows into A columns
  } 
}

proc transpose() {
  // local transpose
  forall dat in Data {
    local {
      ref B = dat.B, C = dat.C;
      forall (i,j) in dat.domB do C[j,i] = B[i,j];
    }
  }  
  // global transpose (comms)
  forall (dat, (gi,gj)) in zip(Data, gridDom) {
    Data[gj,gi].A = dat.C;
  }
}
