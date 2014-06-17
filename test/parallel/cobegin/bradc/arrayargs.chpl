config var n = 10;

const D = {1..n};

var A: [D] real;

proc foo(X, val) {
  X = val;
}

proc bar(X: [?Dom]) {
  const lo = Dom.low,
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  cobegin {
    foo(X[lo..mid-1], 1.1);
    foo(X[mid+1..hi], 2.2);
  }
}

bar(A);

writeln(A);
