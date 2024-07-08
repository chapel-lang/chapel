// This test checks that for a bug in the futures implementation
// where certain operations assumed that a future wasn't destroyed
// while the relevant task was running.

// It uses sleeps to make it more likely it will run into the problem
// if it exists.

use Futures;
use Time;

proc foo() {
  var x = async(proc(x:int) { sleep(1); return x + 1; }, 1);
  var y = x.andThen(proc(x:int) { sleep(1); return 2*x; });

  return y;
}

proc bar() {
  var x = async(proc(x:int) { sleep(1); return x + 1; }, 1);
  var y = async(proc(x:int) { sleep(1); return 2*x; }, 1);
  return waitAll(x, y);
}

proc consume(in x) {
  writeln(x.get());
}

proc main() {
  consume(foo());
  consume(bar());
}
