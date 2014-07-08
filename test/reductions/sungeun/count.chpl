class countVal: ReduceScanOp {
  type eltType;
  const val: eltType;
  var count: atomic int;

  proc countVal(eltType, val) { count.write(0); }

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

writeln(countVal reduce A);

var myCountVal = new countVal(int, 1);

writeln(myCountVal reduce A);
