config const n: int = 100;
config param distString: string = 'default';

const D: domain (1) = [1..n];
var A: [D] int;

proc foo(A: [], e) {
  if A.domain.numIndices == e then
    return true;
  else
    return false;
}

proc main() {
  writeln(foo(A, n));
}
