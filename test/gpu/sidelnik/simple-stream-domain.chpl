use GPUDist;

config const threadsPerBlock = 2;

var m = 16: int(64);

proc main() {

var alpha = 3.0;

const GPUBlockDist = new GPUDist(rank=1, threadsPerBlock=threadsPerBlock);

const space: domain(1, int(64)) dmapped GPUBlockDist = [1..m];

var A, B, C : [space] real;

var _A, _B, _C : [1..m] real;

_A = 4.0;
_B = 9.0;
_C = 4.0;

A = _A;
B = _B;
C = _C;

forall i in space do {
  A(i) = B(i) + alpha * C(i);
}

_A = A;

writeln(_A);
}
