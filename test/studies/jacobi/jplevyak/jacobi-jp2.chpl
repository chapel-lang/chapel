var m = 3, n = 4;
var D = [1..m, 1..n];
var a[D] = 0.0;
var epsilon = 0.001;

writeln("initial = ", a);

var X = [0..m+1, 0..n+1];
var t[X] = 0.0;
[i in D] t(i) = a(i);
t[m+1, ..] = 1.0; // bottom
var stencil = (/ (-1,-1), (1, -1), (-1, 1), (1, 1) /);

while (1) {
  var tt = t;
  [i in D] tt(i) = t[i + stencil]/+;
  if (epsilon < fabs((t - tt))/max) break;
}

writeln("final = ", a);
