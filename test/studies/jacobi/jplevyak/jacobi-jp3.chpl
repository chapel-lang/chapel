config const n = 4;
config const epsilon = 1.0e-5;
var D = [1..n, 1..n];
var t = [D.expand(1)] 0.0;
t[max,..] = 1.0;          // is there a good way of getting the min or max of a
                          // dimension of an array?
const stencil = makeList((0, -1), (0, 1), (-1, 0), (1, 0));
do {
  var tt = t; // can we recognize this as an idiom for A/B swaps?
  [i in D] t(i) = tt(i + stencil)/+ / 4.0;
} while epsilon > abs((t - tt))/max;

writeln(t);
