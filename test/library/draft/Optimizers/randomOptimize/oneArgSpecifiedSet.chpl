use Optimizers;
use List;

proc myFunc(x: int): real {
  return abs(x): real;
}

proc main() {
  var f = myFunc;
  var optimizableArgs: list(optimizableArg);
  var xOptions = [-3, 8, 11, 2, 24];
  optimizableArgs.pushBack(new optimizableArg("x", 16, xOptions));

  var res: list(optimizableArg) = randomOptimize(f, optimizableArgs);
  // Ensure we get as many args back as we provided (no more, no less)
  assert(res.size == 1);

  var bestX: optimizableArg = res.getValue(0);
  // Ensure our result was one of our pre-specified options
  var wasOption: bool = false;
  for i in xOptions {
    if (bestX.value == i) {
      wasOption = true;
      break;
    }
  }
  assert(wasOption);

  // The "best" option should be 2, given the limitations on what to try we've
  // specified.  But since we're randomly testing values, there's no guarantee
  // we'll see 2 as one of the options tried, even though we have a limited
  // number of options to check.
  // It at least shouldn't be "24", given that there is a 1e-7% chance of only
  // selecting that option for all ten tries (and we've set the seed via the
  // COMPOPTS file)
  assert(bestX.value != 24);
}
