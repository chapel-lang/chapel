config const n: int = 100;
config param distString: string = 'default';

const D: domain (1) = [1..n];
var A: [D] int;

def foo(A: [], e) {
  if A.domain.numIndices == e then
    return true;
  else
    return false;
}

def main() {
  writeln(foo(A, n));
}
