use Futures;

const F = async(proc(x: int) { return 2*x; });
writeln(F.get());
