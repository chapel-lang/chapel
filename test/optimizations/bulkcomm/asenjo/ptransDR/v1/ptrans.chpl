/*
Traspose a given matrix B to matrix A
Array is "manually" distributed on locales so each locale has a DR subblock 
Rows to columns communications are implemented a la MPI: 
explicit Data[localeA].DR[slicedest]=Data[localeB].DR[sliceSource]
*/

public use BlockDist;
public use util;

config const n=2, m=4: int;
config const g=3, h=3;
const gridDom = {1..g, 1..h};
var gridLocales: [gridDom] locale = setupGridLocales();

type elType = real;

record localInfo {
  var domA = {1..n, 1..m};  // does not change
  var domB = {1..m, 1..n};  // does not change

  var A: [domA] elType;
  var B: [domB] elType;

  pragma "no copy return"
  proc Acompute return A[domA];
  pragma "no copy return"
  proc Bcompute return B[domB];

  var localDelta: elType;
}

// set up the data arrays
const gridDist = gridDom dmapped Block(gridDom, gridLocales);
var Data: [gridDist] localInfo;

// make sure we got it right
forall (dat, loc) in zip(Data, gridLocales) {
  assert(here == dat.locale);
  assert(dat.locale == loc);
}

var errCount = 0;

proc verify() {
  forall ((gi,gj), dat) in zip(gridDom, Data) {
  if false {
    writeln("Error: ");
    errCount += 1;
  } // forall

  }
}

// initialize B

config const singleinit = false;
forall (dat, (gi,gj)) in zip(Data, gridDom) {
  if singleinit {
    dat.B[2,2] = 100;
   } else {
    forall ((i,j), a) in zip(dat.domB, dat.Bcompute) {
      a = i+ j*0.1;
     }
  }
}


// transpose B -> A

proc transpose() {
  var k: int;
  forall (dat, (gi,gj)) in zip(Data, gridDom) 
  {
     for k in 1..m do {
         dat.A[1..n,k] = Data[gj, gi].B[k, 1..n];
     } // copy B rows into A columns
  } 
}

//showfetch(true);
transpose();          
showfetch(false);

//  verify(true);


