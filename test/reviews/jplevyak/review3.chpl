function compute(a1, a2, result, i, end, o, (c0, p, fn)) {
  while (i <= end) do {
    result(i) = c0 * fn(a1(i), a2(i), p) + o.f(a1(i), a2(i));
    i = i + 1;
  }
}

function g1(p1, p2, (c1, c2)) { return c1 * p1 + c2 * p2; }
function g2(p1, p2, (c1, c2, c3)) { return (c1 * p1 - c2 * p2) / c3; }

var D: domain = 1..4;
var v1 = #[ 4.2, 5.2, 6.2, 7.2, 8.2 ];
var v2 = #[   3,   4,   5,   6,   7 ];
var v3 = [D] 0.0;

class FParams { var z1, z2; }
var fp1 = new FParams; fp1.z1 = 1.0; fp1.z2 = 2.0;
var fp2 = new FParams; fp2.z1 = 3;   fp2.z2 = 4;

function FParams::f(p1, p2) { return z1 * p1 + z2 * p2; }

var params1 = (2.34, (3.45, 4.56),    g1);
var params2 = (2,    (3,    4,    5), g2);

compute(v1, v2, v3, 2, 4, fp1, params1);
compute(v1, v2, v3, 2, 4, fp2, params2);
