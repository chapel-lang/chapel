/* a simple test */

var n: integer = 999;  -- here is a one-line comment

var D: domain(1) = 1..n;

var A: [D] float;

forall i in D {
  A(i) = i;
}
