use GPUDist;

config const threadsPerBlock = 2;

var m = 16: int(64);

proc main() {

var alpha = 3.0;

//const GPUBlockDist = new GPUDist(rank=1,bbox=[1..m], threadsPerBlock=threadsPerBlock);
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

forall (a,b,c) in (A,B,C) do {
	  a = b + alpha * c + _A(2);
}

_A = A;

writeln(_A);
}
