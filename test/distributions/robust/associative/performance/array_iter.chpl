config const debug = false;
config const printTiming = false;

config const n = 1024;
const D = {1..n};
var AD: domain(int);
config const offset = 7;
var Aref: [D] int;
var A: [D] int;
var success = true;

for i in D {
  Aref(i) = 2*i-offset;
  AD += Aref(i);
}
QuickSort(Aref);
if debug then writeln(AD);
if debug then writeln(Aref);

var AA: [AD] int;
forall i in AD {
  AA(i) = i;
}
if debug then writeln(AA);
for i in AD {
  if AA(i) != i then halt("uh oh! ", i);
}

use Time;
var t2: Timer;
t2.start();
for aa in AA {
  var ai = aa;
  A((ai+offset)/2) = ai;
}
t2.stop();
if debug then writeln(A);
success = true;
QuickSort(A);
for i in D {
  if A(i) != Aref(i) {
    success = false;
    break;
  }
}
writeln("Serial array iteration: ", if success then "SUCCESS" else "FAILED");
if printTiming then writeln("Serial: ", t2.elapsed(TimeUnits.milliseconds));


var t3: Timer;
t3.start();
forall aa in AA {
  var ai = aa;
  A((ai+offset)/2) = ai;
}
t3.stop();
if debug then writeln(A);
success = true;
QuickSort(A);
for i in D {
  if A(i) != Aref(i) {
    success = false;
    break;
  }
}
writeln("Parallel array iteration: ", if success then "SUCCESS" else "FAILED");
if printTiming then writeln("Parallel: ", t3.elapsed(TimeUnits.milliseconds));

