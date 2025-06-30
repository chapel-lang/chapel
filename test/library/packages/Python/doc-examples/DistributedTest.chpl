/*START_EXAMPLE*/
use Python, BlockDist;

config const n = 100;
var Arr = blockDist.createArray({1..n}, int);
Arr = 1..n;

coforall l in Arr.targetLocales() {
  on l {
    // each locale has its own interpreter
    const interp = new Interpreter();
    const func = interp.compileLambda("lambda x,: x + 1");

    forall i in Arr.localSubdomain() with (var gil = new GIL()) {
      Arr[i] = func(Arr.eltType, Arr[i]);
    }
  }
}
writeln(Arr);
/*STOP_EXAMPLE*/
