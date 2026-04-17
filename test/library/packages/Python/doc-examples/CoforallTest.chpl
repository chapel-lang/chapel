/*START_EXAMPLE*/
use Python;

var Arr: [1..10] int = 1..10;

var interp = new Interpreter();
var func = interp.compileLambda("lambda x,: x * x");

coforall tid in 1..10 {
  // the call to 'func' automatically acquires and releases the GIL
  Arr[tid] = func(int, tid);
}
writeln(Arr);
/*STOP_EXAMPLE*/
