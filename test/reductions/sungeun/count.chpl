class countVal: ReduceScanOp {
  type eltType;
  const val: eltType;
  var count: atomic int;

  proc init(type eltType, val) {
    this.eltType = eltType;
    this.complete();
    count.write(0);
  }

  proc accumulate(x) {
    if x == val then count.add(1);
  }

  proc combine(xx) {
    count.add(xx.count.read());
  }

  proc generate() return count;
}

const D = {5..9};
var A: [D] int;

forall d in D do if d&1 then A[d] = 1;

// This form does not provide the value to be counted.
//writeln(countVal reduce A);

var myCountVal = new owned countVal(int, 1);

writeln(myCountVal reduce A);
