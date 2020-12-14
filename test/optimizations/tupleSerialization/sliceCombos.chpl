use BlockDist;
use CommDiagnostics;


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

  var numGetsPuts = 0:uint;
  numGetsPuts += + reduce diags.get;
  numGetsPuts += + reduce diags.get_nb;
  numGetsPuts += + reduce diags.put;
  numGetsPuts += + reduce diags.put_nb;

  write(str, ": ");
  writeln(if numGetsPuts == 0 then "No excess comm" else "Excess comm detected");
}

assert((+ reduce A) == 0, "Something went terribly wrong");
assert((+ reduce B) == 0, "Something went terribly wrong");
assert((+ reduce C) == 0, "Something went terribly wrong");
