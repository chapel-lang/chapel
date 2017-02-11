use Futures;

const F = async(lambda(x: int) { return 2*x; }, 29.0);
writeln(F.get());
