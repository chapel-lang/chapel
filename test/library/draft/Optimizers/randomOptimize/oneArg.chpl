use Optimizers;
use List;

proc myFunc(x: int): real {
  return abs(x): real;
}

proc main() {
  var f = myFunc;
  var optimizableArgs: list(optimizableArg);
  optimizableArgs.pushBack(new optimizableArg("x", 16, (-30, 30)));

  var res = randomOptimize(f, optimizableArgs);
  writeln(res);
}
