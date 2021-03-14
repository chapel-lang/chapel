use Futures;

config const X = 23;

const A = async(lambda(x: int) { return  5 + x; }, X);
const B = async(lambda(x: int) { return  7 * x; }, X);
const C = async(lambda(x: int) { return 11 - x; }, X);
const D = waitAll(A, B, C);

writeln(D.get()); // prints (28, 161, -12)
