param initVal = 42;
record R {
  var x = initVal;
}
config const n = 1_000_000;
config const nBigger = 2 * n;
var D = {1..n};
var A: [D] R;

// test default initialization
var wantSum = n * initVal;
var sum = + reduce [a in A] a.x;
if sum == wantSum then writeln('Pass'); else writeln('Fail');

// init manually
for i in D do
  A[i].x = i;
wantSum = (D.low + D.high) * D.size / 2;
sum = + reduce [a in A] a.x;
if sum == wantSum then writeln('Pass'); else writeln('Fail');

// this should realloc in place
D = {1..nBigger};
wantSum += (nBigger - n) * initVal;
sum = + reduce [a in A] a.x;
if sum == wantSum then writeln('Pass'); else writeln('Fail');

// this should not since the low bound is changing and A[i] must be preserved
D = {0..nBigger+1};
wantSum += 2 * initVal;
sum = + reduce [a in A] a.x;
if sum == wantSum then writeln('Pass'); else writeln('Fail');

on Locales[numLocales - 1] {
  if A[D.low].x == initVal && A[n].x == n && A[D.high].x == initVal {
    writeln('Pass');
  } else {
    writeln('Fail');
  }
}
