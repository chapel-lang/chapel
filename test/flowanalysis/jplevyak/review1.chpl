function modify(p1, p2, (c1, c2, c3)) { 
  return (c1 * p1 + c2 * p2) / c3;
}
var params = (2.34, (3.45, 4.56, 5), modify);
var D: domain = 1..4;
var v1 = #[ 4.2, 5.2, 6.2, 7.2, 8.2 ];
var v2 = #[   3,   4,   5,  6,  7 ];
var v3 = [D] 0.0;

function merge(a1, a2, result, i, end, (c0, p, fn)) {
  while (i <= end) do
    result(i) = c0 * fn(a1(i), a2(i), p);
}
merge(v1, v2, v3, 2, 4, params);
