// formerly test/associative/bharshbarg/domains/rectDom.chpl, subset.chpl

config const n = 1000;

var rect_all = {1..n};
var assoc_all : domain(int) = 1..n;

same(110, rect_all, rect_all);
same(120, assoc_all, assoc_all);
same(130, rect_all, assoc_all);

var rect_empty = {1..0};
var assoc_empty: domain(int);
same(140, rect_empty, assoc_empty);

check(310, rect_all,  {1..n by 5});
check(320, assoc_all, {1..n by 5});
check(330, {1..n, 1..n}, {1..n-1, 2..n-2});
check(350, rect_all,  {1..n/2});
check(360, assoc_all, {1..n/2});
check(370, rect_all,  {1..n by 2 align 1});
check(380, assoc_all, {1..n by 2 align 1});

var assoc_sub1 : domain(int) = 1..n by 5;
check(510, assoc_all, assoc_sub1);
check(520, rect_all,  assoc_sub1);

var assoc_sub2 : domain(int) = 1..n;
assoc_sub2 += n..#10;
check(530, assoc_sub2, assoc_all);
check(540, assoc_sub2, rect_all);

var assoc_sub3 : domain(int) = 1..n/2;
check(550, assoc_all, assoc_sub3);
check(560, rect_all,  assoc_sub3);

proc same(idx, d1, d2) {
  writeln(idx);
  assert(d1.contains(d2));
  assert(d2.contains(d1));
}

proc check(idx, super, sub) {
  writeln(idx);
  assert(super.contains(sub));
  assert(!sub.contains(super));
}
