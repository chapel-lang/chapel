function modify1(p1, p2, (c1, c2, c3)) { return (c1 * p1 + c2 * p2) / c3; }
function modify2(p1, p2, (c1, c2, c3)) { return (c1 * p1 - c2 * p2) / c3; }
var params1 = (2.34, (3.45, 4.56, 5),    modify1);
var params2 = (2,    (3,    4,    5.67), modify2);

class Operate { var z1, z2; }
function do_op(p1, p2, o:Operate) { return o.z1 * p1 + o.z2 * p2; }
var op1 = new Operate; op1.z1 = 1.0; op1.z2 = 2;
var op2 = new Operate; op2.z1 = 1;   op2.z2 = 2.0;

var D: domain = 1..4;
var v1 = #[ 4.2, 5.2, 6.2, 7.2, 8.2 ];
var v2 = #[   3,   4,   5,   6,   7 ];
var v3 = [D] 0.0;

function merge(a1, a2, result, i, end, o, (c0, p, fn)) {
  while (i <= end) do
    result(i) = c0 * fn(a1(i), a2(i), p) + do_op(a1(i), a2(i), o);
}
merge(v1, v2, v3, 2, 4, op1, params1);
merge(v1, v2, v3, 2, 4, op2, params2);
