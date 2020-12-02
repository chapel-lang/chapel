use OrderedSet;
use Random;
use Sort;

config param testIters = 137;
var oset = new orderedSet(int, false, defaultComparator);
var a: [0..#testIters] int;
fillRandom(a);
for x in a {
  oset.add(x);
}

assert(oset.size == a.size);

for x in a {
  assert(oset.contains(x));
}

sort(a);
for (x, y) in zip(a, oset) {
  assert(x == y);
}
