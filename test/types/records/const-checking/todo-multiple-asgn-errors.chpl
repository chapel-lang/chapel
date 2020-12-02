// Multiple errors should be reported.

record RECTYPE {
  var VARFIELD: int;
  const CONSTFIELD: int;
}

var r1 = new RECTYPE();
var r2 = new RECTYPE();
r1 = r2;  // error
r2 = r1;  // also error

proc rproc(ref arg: RECTYPE) {}
rproc(r1); // also error

var r3: RECTYPE = r1; // legal

/////////////////////////////////

record OUTERREC {
  var outervar: RECTYPE;
  const outerconst: RECTYPE;
}

var r6 = new OUTERREC();
var r7 = new OUTERREC();
r6 = r7;  // error
r7 = r6;  // also error

r6.outervar = r7.outervar; // also error

proc oproc(ref arg: OUTERREC) {}
oproc(r6); // also error
