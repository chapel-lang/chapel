use Futures;

const F = async(proc(x: int) { return 2*x; }, 29.0);
writeln(F.get());
