record R {}

var r1 = new R();
var r2 = new R();

assert((r1 == r2) == true);
assert((r1 != r2) == false);

assert((r1 < r2) == false);
assert((r2 < r1) == false);
assert((r1 <= r2) == true);
assert((r2 <= r1) == true);

assert((r1 > r2) == false);
assert((r2 > r1) == false);
assert((r1 >= r2) == true);
assert((r2 >= r1) == true);
