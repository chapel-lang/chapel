config param n = 10;

const D3D: domain(3) = {1..n, 2..n+1, 3..n+2};
const D4D: domain(4) = {2..n+1, 3..n+2, 4..n+3, 5..n+4};

proc foo(D: domain, A:[D], i) {
  A += n*i;
}

var A: [D3D] int;
A = -1;
foo(D3D, A.reindex(D3D), -1);
A += -2;
foo(D4D, A.reindex(D4D), -2);

writeln("FAIL: We should not get here!");
