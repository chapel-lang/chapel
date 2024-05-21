use Optimizers;
use List;

proc myFunc(x: int, y: int): real {
  if (x > y) {
    return 0.0;
  } else {
    return 15.0;
  }
}

proc main() {
  var f = myFunc;
  var optimizableArgs: list(optimizableArg);
  optimizableArgs.pushBack(new optimizableArg("x", 16, (0, 30)));

  var res: list(optimizableArg) = randomOptimize(f, optimizableArgs, (3,));
  // Ensure we get as many optimizable args back as we provided (no more, no
  // less)
  assert(res.size == 1);
  var bestX: optimizableArg = res.getValue(0);
  // Ensure the value is in range
  assert(bestX.value <= 30);
  assert(bestX.value >= 0);
  // It's a roughly 12% odds of getting an x where x <= y
  // The odds of it happening for every x is 1.28e-9, assuming
  // we stick with 10 tries.  If there's more, it should be even lower
  // We've set the seed via the COMPOPTS file, though, so this won't fail
  assert(bestX.value > 3);
}
