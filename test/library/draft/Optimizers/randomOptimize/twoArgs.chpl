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
  optimizableArgs.pushBack(new optimizableArg("y", 3, (-30, 5)));

  var res: list(optimizableArg) = randomOptimize(f, optimizableArgs);
  // Ensure we get as many args back as we provided (no more, no less)
  assert(res.size == 2);
  var bestX: optimizableArg = res.getValue(0);
  var bestY: optimizableArg = res.getValue(1);
  // Ensure the values are in range
  assert(bestX.value <= 30);
  assert(bestX.value >= 0);
  assert(bestY.value <= 5);
  assert(bestY.value >= -30);
  // It's a roughly 3% odds of getting one x,y combination where x < y
  // The odds of it happening for every x,y combination is 2.76e-9, assuming
  // we stick with 10 tries.  If there's more, it should be even lower
  // We've set the seed explicitly via the COMPOPTS, though, so it won't fail
  assert(bestX.value > bestY.value);
}
