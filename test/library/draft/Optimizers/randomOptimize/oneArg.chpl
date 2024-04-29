use Optimizers;
use List;

proc myFunc(x: int): real {
  return abs(x): real;
}

proc main() {
  var f = myFunc;
  var optimizableArgs: list(optimizableArg);
  optimizableArgs.pushBack(new optimizableArg("x", 16, (-30, 30)));

  var res: list(optimizableArg) = randomOptimize(f, optimizableArgs);
  // Ensure we get as many args back as we provided (no more, no less)
  assert(res.size == 1);
  var bestX: optimizableArg = res.getValue(0);
  // Ensure the value is in range and is not the highest/lowest possible value.
  // The "best" option should be 0, but since we're randomly testing values,
  // there's no guarantee we'll see 0 as one of the options tried.
  assert(bestX.value < 30);
  assert(bestX.value > -30);
}
