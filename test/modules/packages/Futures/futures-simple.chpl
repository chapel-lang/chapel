use Futures;

config const X = 42;

const A = async(lambda(x: int) { return 2 * x; }, X);
const B = async(lambda(x: int) { return 3 * x; }, X);
const C = async(lambda(x: int) { return 4 * x; }, X);

writeln(A.get());
writeln(B.get());
writeln(C.get());
