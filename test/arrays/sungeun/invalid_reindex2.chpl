config param n = 10;
config param m = 20;

const D4D0: domain(4) = {1..n, 2..n, 3..n, 4..n};
const D4D1: domain(4) = {10+1..m, 10+2..m, 10+2..m, 10+4..m};

proc foo(D: domain, A:[D], i) {
  A += n*i;
}

var A: [D4D0] int;
A = -1;
foo(D4D0, A.reindex(D4D0), -1);
A += -2;
foo(D4D1, A.reindex(D4D1), -2);

writeln("FAIL: We should not get here!");
