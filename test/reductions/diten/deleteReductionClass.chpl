var count$: sync int = 0;
class LastOp: ReduceScanOp {
  type eltType;
  var last: eltType;

  proc init(type eltType) {
    this.eltType = eltType;
    count$ += 1;
  }
  proc deinit() {
    count$ -= 1;
  }
  proc accumulate(x) {
    last = x;
  }
  proc combine(x) {
    last = x.last;
  }
  proc generate() {
    return last;
  }
}

proc main {
  var A: [1..5] int = 1..5;
  var x: int;
  x = LastOp reduce A;

  if count$ == 0 then
    writeln("Deleted every reduction class.");
  else
    halt("didn't delete every reduction class");
}
