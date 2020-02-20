//
// Test how the default tuple argument intent interacts with a tuple expr-
// ession where one element has been captured by `const ref`.
//

record R { var x: int = 0; }

const r = new R(1);

proc test(tup) {
  tup[1] = new R(42);
}

test((r,));
