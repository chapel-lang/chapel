use Futures;

config const X = 42;

const F = async(proc(x: int) { return 2 * x; }, X)
  .andThen(proc(x: int) { return x + 4; })
  .andThen(proc(x: int) { return 5 * x; });

const G = async(proc(x: real) { return x - 7.0; }, X:real)
  .andThen(proc(x: real) { return x * 11.0; });

const H = waitAll(F, G);

writeln(H.get());
