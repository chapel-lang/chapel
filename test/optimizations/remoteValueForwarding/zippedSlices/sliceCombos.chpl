use BlockDist;
use CommDiagnostics;

config const printTable = false;
config const printArrays = false;

var dom = newBlockDom(1..10);
var innerDom = dom.expand(-1);

var A: [dom] int;
var B: [innerDom] int;
var C: [dom] int;
var D: [innerDom] int;
var E: [innerDom] int;

// we have to make sure that none of the following here cause GETs when slice
// serialization is enabled (the test also checks other commcounts to be below a
// threshold)
startDiag();
forall (x,y) in zip(A[innerDom], B) do x=y;
endDiag("zip(slice, arr)");

startDiag();
A[innerDom] = B;
endDiag("promotion(slice, arr)");

/* ------------------------------ */

startDiag();
forall (x,y) in zip(B, A[innerDom]) do x=y;
endDiag("zip(arr, slice)");

startDiag();
B = A[innerDom];
endDiag("promotion(arr, slice)");

/* ------------------------------ */

startDiag();
forall (x,y) in zip(A[innerDom], C[innerDom]) do x=y;
endDiag("zip(slice, slice)");

startDiag();
A[innerDom] = C[innerDom];
endDiag("promotion(slice, slice)");

/* ------------------------------ */

startDiag();
forall (x,y,z) in zip(A[innerDom], B, D) do x=y+z;
endDiag("zip(slice, arr, arr)");

startDiag();
A[innerDom] = B + D;
endDiag("promotion(slice, arr, arr)");

/* ------------------------------ */

startDiag();
forall (x,y,z) in zip(A[innerDom], B, C[innerDom]) do x=y+z;
endDiag("zip(slice, arr, slice)");

startDiag();
A[innerDom] = B + C[innerDom];
endDiag("promotion(slice, arr, slice)");

/* ------------------------------ */

startDiag();
forall (x,y,z) in zip(D, B, C[innerDom]) do x=y+z;
endDiag("zip(arr, arr, slice)");

startDiag();
D = B + C[innerDom];
endDiag("promotion(arr, arr, slice)");

/* ------------------------------ */

startDiag();
forall (x,y,z,t) in zip(A[innerDom], B, C[innerDom], D) do x=y+z+t;
endDiag("zip(slice, arr, slice, arr)");

startDiag();
A[innerDom] = B + C[innerDom] + D;
endDiag("promotion(slice, arr, slice, arr)");

/* ------------------------------ */

startDiag();
forall (x,y,z,t) in zip(E, B, C[innerDom], D) do x=y+z+t;
endDiag("zip(arr, arr, slice, arr)");

startDiag();
E = B + C[innerDom] + D;
endDiag("promotion(arr, arr, slice, arr)");

/* ------------------------------ */

startDiag();
forall (x,y,z,t) in zip(A[innerDom], B, E, D) do x=y+z+t;
endDiag("zip(slice, arr, arr, arr)");

startDiag();
A[innerDom] = B + E + D;
endDiag("promotion(slice, arr, arr, arr)");

proc startDiag() {
  startCommDiagnostics();
}

proc endDiag(str) {
  stopCommDiagnostics();
  var diags = getCommDiagnostics();

  writeln(str);

  // we want no get/put
  var numGetsPuts = 0;
  numGetsPuts += + reduce diags.get;
  numGetsPuts += + reduce diags.get_nb;
  numGetsPuts += + reduce diags.put;
  numGetsPuts += + reduce diags.put_nb;
  if numGetsPuts == 0 then writeln("\tNo excess comm");
  else writeln("\tExcess comm detected");

  // we want no slow on
  var numOns = 0;
  numOns += + reduce diags.execute_on;
  if numOns == 0 then writeln("\tNo excess slow on");
  else writeln("\tExcess slow on detected");

  // all locales except for 0 should do at most 1 fast on
  var fastOns = diags.execute_on_fast;
  var excessFastOn = false;
  if fastOns[0] > 0 then excessFastOn = true;
  for l in Locales[1..] do
    if fastOns[l.id] > 1 then excessFastOn = true;
  if !excessFastOn then writeln("\tNo excess fast on");
  else writeln("\tExcess fast on detected");

  // only locale 0 should do nb ons to other locales
  var nbOns = diags.execute_on_nb;
  var excessNBOn = false;
  if nbOns[0] > 3 then excessNBOn = true;
  for l in Locales[1..] do
    if nbOns[l.id] > 0 then excessNBOn = true;
  if !excessNBOn then writeln("\tNo excess non-blocking on");
  else writeln("\tExcess non-blocking on detected");

  if printTable then printCommDiagnosticsTable();
  resetCommDiagnostics();
}

if printArrays {
  writeln(A);
  writeln(B);
  writeln(C);
}
