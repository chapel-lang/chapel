use Futures;

config const X = 42;

const F = async(proc(x: int) { return 2 * x; }, X)
  .andThen(proc(x: int) { return x + 7; })
  .andThen(proc(x: int) { return 3 * x; });

writeln(F.get() == (3 * ((2 * X) + 7))); // prints "true"
