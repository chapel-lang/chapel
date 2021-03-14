use Futures;

config const X = 42;

const F = async(lambda(x: int) { return 2 * x; }, X)
  .andThen(lambda(x: int) { return x + 4; })
  .andThen(lambda(x: int) { return 5 * x; });

const G = async(lambda(x: real) { return x - 7.0; }, X:real)
  .andThen(lambda(x: real) { return x * 11.0; });

const H = waitAll(F, G);

writeln(H.get());
