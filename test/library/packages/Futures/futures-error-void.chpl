use Futures;

const F = async(lambda(x: int) { return 2*x; });
writeln(F.get());
