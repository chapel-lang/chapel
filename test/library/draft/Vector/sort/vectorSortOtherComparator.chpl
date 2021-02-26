//
// Make sure `vector.sort` is generic over multiple comparator types,
// not just "defaultComparator".
//
use Vector;
use Sort;

// Verify that the default comparator still works.
var vec1: vector(int) = [8, 7, 6, 5, 1, 2, 3, 4];
vec1.sort();
writeln(vec1);
vec1.clear();

// Try sorting the vector in reverse.
var vec2: vector(int) = [8, 7, 6, 5, 1, 2, 3, 4];
vec2.sort(Sort.reverseComparator);
writeln(vec2);
vec2.clear();

//
// Follow along with the Sort documentation and make "our own"
// comparator (this is just absval).
//
record myComparator {}
proc myComparator.compare(a, b) {
  return abs(a) - abs(b);
}

var absComparator: myComparator;

// Let's sort a vector of negative values with absval.
var vec3: vector(int) = [-8, -7, -6, -5, -4, -3, -2, -1];
vec3.sort(absComparator);
writeln(vec3);

