config const n = 1000;

var rect_all = {1..n};

assert(rect_all.isSubset(rect_all));
assert(rect_all.isSuper(rect_all));

{
  var rect_sub = {1..n by 5};
  assert(rect_sub.isSubset(rect_all));
  assert(rect_all.isSuper(rect_sub));
  assert(!rect_all.isSubset(rect_sub));
}

{
  var rect_sub_1 = {1..n, 1..n};
  var rect_sub_2 = {1..n-1, 2..n-2};
  assert(rect_sub_2.isSubset(rect_sub_1));
  assert(rect_sub_1.isSuper(rect_sub_2));
  assert(!rect_sub_1.isSubset(rect_sub_2));
}

{
  var rect_sub = {1..n/2};
  assert(rect_sub.isSubset(rect_all));
  assert(rect_all.isSuper(rect_sub));
  assert(!rect_sub.isSuper(rect_all));
}

{
  var rect_sub = {1..n by 2 align 1};
  assert(rect_sub.isSubset(rect_all));
  assert(rect_all.isSuper(rect_sub));
  assert(!rect_sub.isSuper(rect_all));
}
