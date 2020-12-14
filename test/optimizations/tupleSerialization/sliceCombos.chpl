use BlockDist;
use CommDiagnostics;

config const printTable = false;

var dom = newBlockDom(1..10);
var innerDom = dom.expand(-1);

var A: [dom] int;
var B: [innerDom] int;
var C: [dom] int;

startDiag();
forall (x,y) in zip(A[innerDom], B) do x=y;
endDiag("zip(slice, arr)");

startDiag();
forall (x,y) in zip(B, A[innerDom]) do x=y;
endDiag("zip(arr, slice)");

startDiag();
forall (x,y) in zip(A[innerDom], C[innerDom]) do x=y;
endDiag("zip(slice, slice)");

proc startDiag() {
  startCommDiagnostics();
}

proc endDiag(str) {
  stopCommDiagnostics();
  var diags = getCommDiagnostics();

  writeln(str);

  // we want no get/put
  var numGetsPuts = 0:uint;
  numGetsPuts += + reduce diags.get;
  numGetsPuts += + reduce diags.get_nb;
  numGetsPuts += + reduce diags.put;
  numGetsPuts += + reduce diags.put_nb;
  if numGetsPuts == 0 then writeln("\tNo excess comm");
  else writeln("\tExcess comm detected");

  // we want no slow on
  var numOns = 0:uint;
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

assert((+ reduce A) == 0, "Something went terribly wrong");
assert((+ reduce B) == 0, "Something went terribly wrong");
assert((+ reduce C) == 0, "Something went terribly wrong");
